// Copyright 2023 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/heap/heap-inl.h"
#include "src/heap/incremental-marking.h"
#include "src/heap/page-inl.h"
#include "src/heap/paged-spaces.h"

namespace v8 {
namespace internal {

// These checks are here to ensure that the lower 32 bits of any real heap
// object can't overlap with the lower 32 bits of cleared weak reference value
// and therefore it's enough to compare only the lower 32 bits of a
// Tagged<MaybeObject> in order to figure out if it's a cleared weak reference
// or not.
static_assert(kClearedWeakHeapObjectLower32 > 0);
static_assert(kClearedWeakHeapObjectLower32 < Page::kHeaderSize);

// static
constexpr Page::MainThreadFlags Page::kCopyOnFlipFlagsMask;

Page::Page(Heap* heap, BaseSpace* space, size_t size, Address area_start,
           Address area_end, VirtualMemory reservation,
           Executability executable)
    : MemoryChunk(heap, space, size, area_start, area_end,
                  std::move(reservation), executable, PageSize::kRegular) {}

void Page::AllocateFreeListCategories() {
  DCHECK_NULL(categories_);
  categories_ =
      new FreeListCategory*[owner()->free_list()->number_of_categories()]();
  for (int i = kFirstCategory; i <= owner()->free_list()->last_category();
       i++) {
    DCHECK_NULL(categories_[i]);
    categories_[i] = new FreeListCategory();
  }
}

void Page::InitializeFreeListCategories() {
  for (int i = kFirstCategory; i <= owner()->free_list()->last_category();
       i++) {
    categories_[i]->Initialize(static_cast<FreeListCategoryType>(i));
  }
}

void Page::ReleaseFreeListCategories() {
  if (categories_ != nullptr) {
    for (int i = kFirstCategory; i <= owner()->free_list()->last_category();
         i++) {
      if (categories_[i] != nullptr) {
        delete categories_[i];
        categories_[i] = nullptr;
      }
    }
    delete[] categories_;
    categories_ = nullptr;
  }
}

Page* Page::ConvertNewToOld(Page* old_page) {
  DCHECK(old_page);
  DCHECK(old_page->InNewSpace());
  old_page->ResetAgeInNewSpace();
  OldSpace* old_space = old_page->heap()->old_space();
  old_page->set_owner(old_space);
  old_page->ClearFlags(Page::kAllFlagsMask);
  Page* new_page = old_space->InitializePage(old_page);
  old_space->AddPromotedPage(new_page);
  return new_page;
}

size_t Page::AvailableInFreeList() {
  size_t sum = 0;
  ForAllFreeListCategories(
      [&sum](FreeListCategory* category) { sum += category->available(); });
  return sum;
}

void Page::MarkNeverAllocateForTesting() {
  DCHECK(this->owner_identity() != NEW_SPACE);
  DCHECK(!IsFlagSet(NEVER_ALLOCATE_ON_PAGE));
  SetFlag(NEVER_ALLOCATE_ON_PAGE);
  SetFlag(NEVER_EVACUATE);
  reinterpret_cast<PagedSpace*>(owner())->free_list()->EvictFreeListItems(this);
}

#ifdef DEBUG
namespace {
// Skips filler starting from the given filler until the end address.
// Returns the first address after the skipped fillers.
Address SkipFillers(PtrComprCageBase cage_base, Tagged<HeapObject> filler,
                    Address end) {
  Address addr = filler.address();
  while (addr < end) {
    filler = HeapObject::FromAddress(addr);
    CHECK(IsFreeSpaceOrFiller(filler, cage_base));
    addr = filler.address() + filler->Size(cage_base);
  }
  return addr;
}
}  // anonymous namespace
#endif  // DEBUG

size_t Page::ShrinkToHighWaterMark() {
  // Shrinking only makes sense outside of the CodeRange, where we don't care
  // about address space fragmentation.
  VirtualMemory* reservation = reserved_memory();
  if (!reservation->IsReserved()) return 0;

  // Shrink pages to high water mark. The water mark points either to a filler
  // or the area_end.
  Tagged<HeapObject> filler = HeapObject::FromAddress(HighWaterMark());
  if (filler.address() == area_end()) return 0;
  PtrComprCageBase cage_base(heap()->isolate());
  CHECK(IsFreeSpaceOrFiller(filler, cage_base));
  // Ensure that no objects were allocated in [filler, area_end) region.
  DCHECK_EQ(area_end(), SkipFillers(cage_base, filler, area_end()));
  // Ensure that no objects will be allocated on this page.
  DCHECK_EQ(0u, AvailableInFreeList());

  // Ensure that slot sets are empty. Otherwise the buckets for the shrunk
  // area would not be freed when deallocating this page.
  DCHECK_NULL(slot_set<OLD_TO_NEW>());
  DCHECK_NULL(slot_set<OLD_TO_NEW_BACKGROUND>());
  DCHECK_NULL(slot_set<OLD_TO_OLD>());

  size_t unused = RoundDown(static_cast<size_t>(area_end() - filler.address()),
                            MemoryAllocator::GetCommitPageSize());
  if (unused > 0) {
    DCHECK_EQ(0u, unused % MemoryAllocator::GetCommitPageSize());
    if (v8_flags.trace_gc_verbose) {
      PrintIsolate(heap()->isolate(), "Shrinking page %p: end %p -> %p\n",
                   reinterpret_cast<void*>(this),
                   reinterpret_cast<void*>(area_end()),
                   reinterpret_cast<void*>(area_end() - unused));
    }
    heap()->CreateFillerObjectAt(
        filler.address(),
        static_cast<int>(area_end() - filler.address() - unused));
    heap()->memory_allocator()->PartialFreeMemory(
        this, address() + size() - unused, unused, area_end() - unused);
    if (filler.address() != area_end()) {
      CHECK(IsFreeSpaceOrFiller(filler, cage_base));
      CHECK_EQ(filler.address() + filler->Size(cage_base), area_end());
    }
  }
  return unused;
}

void Page::CreateBlackArea(Address start, Address end) {
  DCHECK_NE(NEW_SPACE, owner_identity());
  DCHECK(heap()->incremental_marking()->black_allocation());
  DCHECK_EQ(Page::FromAddress(start), this);
  DCHECK_LT(start, end);
  DCHECK_EQ(Page::FromAddress(end - 1), this);
  marking_bitmap()->SetRange<AccessMode::ATOMIC>(
      MarkingBitmap::AddressToIndex(start),
      MarkingBitmap::LimitAddressToIndex(end));
  IncrementLiveBytesAtomically(static_cast<intptr_t>(end - start));
}

void Page::DestroyBlackArea(Address start, Address end) {
  DCHECK_NE(NEW_SPACE, owner_identity());
  DCHECK(heap()->incremental_marking()->black_allocation());
  DCHECK_EQ(Page::FromAddress(start), this);
  DCHECK_LT(start, end);
  DCHECK_EQ(Page::FromAddress(end - 1), this);
  marking_bitmap()->ClearRange<AccessMode::ATOMIC>(
      MarkingBitmap::AddressToIndex(start),
      MarkingBitmap::LimitAddressToIndex(end));
  IncrementLiveBytesAtomically(-static_cast<intptr_t>(end - start));
}

}  // namespace internal
}  // namespace v8
