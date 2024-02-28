// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/builtins/builtins-utils-inl.h"
#include "src/builtins/builtins.h"
#include "src/objects/bigint.h"
#include "src/objects/js-temporal-objects-inl.h"
#include "src/objects/js-temporal-objects.h"

namespace v8 {
namespace internal {

#define TEMPORAL_METHOD1(T, METHOD)                                       \
  BUILTIN(Temporal##T##METHOD) {                                          \
    HandleScope scope(isolate);                                           \
    RETURN_RESULT_OR_FAILURE(                                             \
        isolate,                                                          \
        JSTemporal##T ::METHOD(isolate, args.atOrUndefined(isolate, 1))); \
  }

#define TEMPORAL_METHOD2(T, METHOD)                                     \
  BUILTIN(Temporal##T##METHOD) {                                        \
    HandleScope scope(isolate);                                         \
    RETURN_RESULT_OR_FAILURE(                                           \
        isolate,                                                        \
        JSTemporal##T ::METHOD(isolate, args.atOrUndefined(isolate, 1), \
                               args.atOrUndefined(isolate, 2)));        \
  }

static const char* temporal_constructor_names[] = {
    "Calendar",       "Duration",      "Instant",       "Now",
    "PlainDate",      "PlainDateTime", "PlainMonthDay", "PlainTime",
    "PlainYearMonth", "TimeZone",      "ZonedDateTime",
};
static_assert(
    sizeof(temporal_constructor_names) / sizeof(*temporal_constructor_names) ==
        kNumTemporalConstructors,
    "Temporal constructor names must match TemporalConstructorType enum");

static TemporalConstructorType StashedConstructorType(
    Isolate* i, const BuiltinArguments& args) {
  Handle<Object> ctor_id_val = JSObject::GetDataProperty(
      i, args.target(), i->factory()->temporal_ctor_id_symbol());
  DCHECK(IsSmi(*ctor_id_val));
  return static_cast<TemporalConstructorType>(Smi::cast(*ctor_id_val).value());
}

BUILTIN(TemporalConstructorDispatcher) {
  HandleScope scope{isolate};

  switch (StashedConstructorType(isolate, args)) {
    case kPlainDate:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalPlainDate::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),    // iso_year
                       args.atOrUndefined(isolate, 2),    // iso_month
                       args.atOrUndefined(isolate, 3),    // iso_day
                       args.atOrUndefined(isolate, 4)));  // calendar_like
    case kPlainTime:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalPlainTime::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),    // hour
                       args.atOrUndefined(isolate, 2),    // minute
                       args.atOrUndefined(isolate, 3),    // second
                       args.atOrUndefined(isolate, 4),    // millisecond
                       args.atOrUndefined(isolate, 5),    // microsecond
                       args.atOrUndefined(isolate, 6)));  // nanosecond
    case kPlainDateTime:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalPlainDateTime::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),     // iso_year
                       args.atOrUndefined(isolate, 2),     // iso_month
                       args.atOrUndefined(isolate, 3),     // iso_day
                       args.atOrUndefined(isolate, 4),     // hour
                       args.atOrUndefined(isolate, 5),     // minute
                       args.atOrUndefined(isolate, 6),     // second
                       args.atOrUndefined(isolate, 7),     // millisecond
                       args.atOrUndefined(isolate, 8),     // microsecond
                       args.atOrUndefined(isolate, 9),     // nanosecond
                       args.atOrUndefined(isolate, 10)));  // calendar_like
    case kPlainYearMonth:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalPlainYearMonth::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),    // iso_year
                       args.atOrUndefined(isolate, 2),    // iso_month
                       args.atOrUndefined(isolate, 3),    // calendar_like
                       args.atOrUndefined(isolate, 4)));  // reference_iso_day
    case kPlainMonthDay:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalPlainMonthDay::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),    // iso_month
                       args.atOrUndefined(isolate, 2),    // iso_day
                       args.atOrUndefined(isolate, 3),    // calendar_like
                       args.atOrUndefined(isolate, 4)));  // reference_iso_year
    case kZonedDateTime:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalZonedDateTime::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),    // epoch_nanoseconds
                       args.atOrUndefined(isolate, 2),    // time_zone_like
                       args.atOrUndefined(isolate, 3)));  // calendar_like
    case kDuration:
      RETURN_RESULT_OR_FAILURE(
          isolate, JSTemporalDuration::Constructor(
                       isolate, args.target(), args.new_target(),
                       args.atOrUndefined(isolate, 1),     // years
                       args.atOrUndefined(isolate, 2),     // months
                       args.atOrUndefined(isolate, 3),     // weeks
                       args.atOrUndefined(isolate, 4),     // days
                       args.atOrUndefined(isolate, 5),     // hours
                       args.atOrUndefined(isolate, 6),     // minutes
                       args.atOrUndefined(isolate, 7),     // seconds
                       args.atOrUndefined(isolate, 8),     // milliseconds
                       args.atOrUndefined(isolate, 9),     // microseconds
                       args.atOrUndefined(isolate, 10)));  // nanoseconds
    case kInstant:
      RETURN_RESULT_OR_FAILURE(isolate,
                               JSTemporalInstant::Constructor(
                                   isolate, args.target(), args.new_target(),
                                   args.atOrUndefined(isolate, 1)));
    case kCalendar:
      RETURN_RESULT_OR_FAILURE(isolate,
                               JSTemporalCalendar::Constructor(
                                   isolate, args.target(), args.new_target(),
                                   args.atOrUndefined(isolate, 1)));
    case kTimeZone:
      RETURN_RESULT_OR_FAILURE(isolate,
                               JSTemporalTimeZone::Constructor(
                                   isolate, args.target(), args.new_target(),
                                   args.atOrUndefined(isolate, 1)));
    case kNow:
    case kNumTemporalConstructors:
      UNREACHABLE();
  }
}

static Handle<String> StashedPropertyName(Isolate* i,
                                          const BuiltinArguments& args) {
  Handle<Object> prop_val = JSObject::GetDataProperty(
      i, args.target(), i->factory()->temporal_calendar_prop_symbol());
  DCHECK(IsString(*prop_val));
  return Object::NoSideEffectsToString(i, prop_val);
}

BUILTIN(TemporalStaticMethodDispatcher) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);
  Handle<String> prop = StashedPropertyName(isolate, args);

  std::unique_ptr<char[]> prop_name = prop->ToCString();
  auto method_name = std::string{"Temporal."} +
                     temporal_constructor_names[ctor_type] + "." +
                     prop_name.get();

  Factory* factory = isolate->factory();

#define DISPATCH_STRCMP_NOW(type, cc_name)                             \
  if (strcmp(prop_name.get(), #cc_name) == 0) {                        \
    RETURN_RESULT_OR_FAILURE(                                          \
        isolate,                                                       \
        JSTemporal##type::Now(isolate, args.atOrUndefined(isolate, 1), \
                              args.atOrUndefined(isolate, 2)));        \
  }
#define DISPATCH_STRCMP_NOW_ISO(type, cc_name)                              \
  if (strcmp(prop_name.get(), #cc_name "ISO") == 0) {                       \
    RETURN_RESULT_OR_FAILURE(                                               \
        isolate,                                                            \
        JSTemporal##type::NowISO(isolate, args.atOrUndefined(isolate, 1))); \
  }
#define DISPATCH_METHOD1(type, sc_name, cc_name)                              \
  if (prop->Equals(*factory->cc_name##_string())) {                           \
    RETURN_RESULT_OR_FAILURE(                                                 \
        isolate,                                                              \
        JSTemporal##type ::sc_name(isolate, args.atOrUndefined(isolate, 1))); \
  }
#define DISPATCH_METHOD2(type, sc_name, cc_name)                            \
  if (prop->Equals(*factory->cc_name##_string())) {                         \
    RETURN_RESULT_OR_FAILURE(                                               \
        isolate,                                                            \
        JSTemporal##type ::sc_name(isolate, args.atOrUndefined(isolate, 1), \
                                   args.atOrUndefined(isolate, 2)));        \
  }
#define DISPATCH_STRCMP_METHOD1(type, sc_name, cc_name)                       \
  if (strcmp(prop_name.get(), #cc_name) == 0) {                               \
    RETURN_RESULT_OR_FAILURE(                                                 \
        isolate,                                                              \
        JSTemporal##type ::sc_name(isolate, args.atOrUndefined(isolate, 1))); \
  }

  switch (ctor_type) {
    case kNow:
      if (prop->Equals(*factory->timeZone_string())) {
        RETURN_RESULT_OR_FAILURE(isolate, JSTemporalTimeZone::Now(isolate));
      }
      if (prop->Equals(*factory->plainDate_string())) {
        RETURN_RESULT_OR_FAILURE(
            isolate,
            JSTemporalPlainDate::Now(isolate, args.atOrUndefined(isolate, 1),
                                     args.atOrUndefined(isolate, 2)));
      }
      if (strcmp(prop_name.get(), "instant") == 0) {
        RETURN_RESULT_OR_FAILURE(isolate, JSTemporalInstant::Now(isolate));
      }
      DISPATCH_STRCMP_NOW(PlainDateTime, plainDateTime);
      DISPATCH_STRCMP_NOW_ISO(PlainDateTime, plainDateTime);
      DISPATCH_STRCMP_NOW_ISO(PlainDate, plainDate);
      DISPATCH_STRCMP_NOW_ISO(PlainTime, plainTime);
      DISPATCH_STRCMP_NOW(ZonedDateTime, zonedDateTime);
      DISPATCH_STRCMP_NOW_ISO(ZonedDateTime, zonedDateTime);
      UNREACHABLE();
    case kPlainDate:
      DISPATCH_METHOD2(PlainDate, From, from);
      DISPATCH_METHOD2(PlainDate, Compare, compare);
      UNREACHABLE();
    case kPlainTime:
      DISPATCH_METHOD2(PlainTime, From, from);
      DISPATCH_METHOD2(PlainTime, Compare, compare);
      UNREACHABLE();
    case kPlainDateTime:
      DISPATCH_METHOD2(PlainDateTime, From, from);
      DISPATCH_METHOD2(PlainDateTime, Compare, compare);
      UNREACHABLE();
    case kPlainYearMonth:
      DISPATCH_METHOD2(PlainYearMonth, From, from);
      DISPATCH_METHOD2(PlainYearMonth, Compare, compare);
      UNREACHABLE();
    case kPlainMonthDay:
      DISPATCH_METHOD2(PlainMonthDay, From, from);
      UNREACHABLE();
    case kZonedDateTime:
      DISPATCH_METHOD2(ZonedDateTime, From, from);
      DISPATCH_METHOD2(ZonedDateTime, Compare, compare);
      UNREACHABLE();
    case kDuration:
      DISPATCH_METHOD1(Duration, From, from);
      if (prop->Equals(*factory->compare_string())) {
        RETURN_RESULT_OR_FAILURE(
            isolate,
            JSTemporalDuration::Compare(isolate, args.atOrUndefined(isolate, 1),
                                        args.atOrUndefined(isolate, 2),
                                        args.atOrUndefined(isolate, 3)));
      }
      UNREACHABLE();
    case kInstant:
      DISPATCH_METHOD1(Instant, From, from);
      DISPATCH_METHOD2(Instant, Compare, compare);
      DISPATCH_STRCMP_METHOD1(Instant, FromEpochSeconds, fromEpochSeconds);
      DISPATCH_STRCMP_METHOD1(Instant, FromEpochMilliseconds,
                              fromEpochMilliseconds);
      DISPATCH_STRCMP_METHOD1(Instant, FromEpochMicroseconds,
                              fromEpochMicroseconds);
      DISPATCH_STRCMP_METHOD1(Instant, FromEpochNanoseconds,
                              fromEpochNanoseconds);
      UNREACHABLE();
    case kCalendar:
      if (prop->Equals(*factory->from_string())) {
        RETURN_RESULT_OR_FAILURE(isolate,
                                 temporal::ToTemporalCalendar(
                                     isolate, args.atOrUndefined(isolate, 1),
                                     "Temporal.Calendar.from"));
      }
      UNREACHABLE();
    case kTimeZone:
      if (prop->Equals(*factory->from_string())) {
        RETURN_RESULT_OR_FAILURE(isolate,
                                 temporal::ToTemporalTimeZone(
                                     isolate, args.atOrUndefined(isolate, 1),
                                     "Temporal.TimeZone.from"));
      }
      UNREACHABLE();
    case kNumTemporalConstructors:
      UNREACHABLE();
  }

#undef DISPATCH_STRCMP_NOW
#undef DISPATCH_STRCMP_NOW_ISO
#undef DISPATCH_METHOD1
#undef DISPATCH_METHOD2
#undef DISPATCH_STRCMP_METHOD1
}

static Tagged<Object> GetExactTime(Isolate* isolate,
                                   Handle<BigInt> epoch_nanoseconds,
                                   uint64_t scale, bool is_bigint) {
  Handle<BigInt> value;
  ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
      isolate, value,
      BigInt::Divide(isolate, epoch_nanoseconds,
                     BigInt::FromUint64(isolate, scale)));

  if (is_bigint) return *value;

  Handle<Object> number = BigInt::ToNumber(isolate, value);
  DCHECK(std::isfinite(Object::Number(*number)));
  return *number;
}

BUILTIN(TemporalPrototypeMethodDispatcher) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);
  Handle<String> prop = StashedPropertyName(isolate, args);

  std::unique_ptr<char[]> prop_name = prop->ToCString();
  auto method_name = std::string{"Temporal."} +
                     temporal_constructor_names[ctor_type] + ".prototype." +
                     prop_name.get();

  Factory* factory = isolate->factory();

#define DISPATCH_DIRECT_GETTER(field, cc_name) \
  if (prop->Equals(*factory->cc_name##_string())) return obj->field();
#define DISPATCH_ISO_GETTER(field)                \
  if (prop->Equals(*factory->field##_string())) { \
    return Smi::FromInt(obj->iso_##field());      \
  }
#define DISPATCH_SCALED_EXACT_TIME_GETTER(cc_name, scale, is_bigint)   \
  if (prop->Equals(*factory->cc_name##_string())) {                    \
    Handle<BigInt> epoch_nanoseconds{obj->nanoseconds(), isolate};     \
    return GetExactTime(isolate, epoch_nanoseconds, scale, is_bigint); \
  }
#define DISPATCH_METHOD0(type, sc_name, cc_name)                       \
  if (prop->Equals(*factory->cc_name##_string())) {                    \
    RETURN_RESULT_OR_FAILURE(isolate,                                  \
                             JSTemporal##type::sc_name(isolate, obj)); \
  }
#define DISPATCH_METHOD1(type, sc_name, cc_name)                             \
  if (prop->Equals(*factory->cc_name##_string())) {                          \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate, JSTemporal##type::sc_name(isolate, obj,                     \
                                           args.atOrUndefined(isolate, 1))); \
  }
#define DISPATCH_METHOD2(type, sc_name, cc_name)                             \
  if (prop->Equals(*factory->cc_name##_string())) {                          \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate, JSTemporal##type::sc_name(isolate, obj,                     \
                                           args.atOrUndefined(isolate, 1),   \
                                           args.atOrUndefined(isolate, 2))); \
  }
#define DISPATCH_METHOD3(type, sc_name, cc_name)                             \
  if (prop->Equals(*factory->cc_name##_string())) {                          \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate, JSTemporal##type::sc_name(isolate, obj,                     \
                                           args.atOrUndefined(isolate, 1),   \
                                           args.atOrUndefined(isolate, 2),   \
                                           args.atOrUndefined(isolate, 3))); \
  }
#define DISPATCH_STRCMP_METHOD0(type, sc_name, cc_name)                \
  if (strcmp(prop_name.get(), #cc_name) == 0) {                        \
    RETURN_RESULT_OR_FAILURE(isolate,                                  \
                             JSTemporal##type::sc_name(isolate, obj)); \
  }
#define DISPATCH_STRCMP_METHOD1(type, sc_name, cc_name)                      \
  if (strcmp(prop_name.get(), #cc_name) == 0) {                              \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate, JSTemporal##type::sc_name(isolate, obj,                     \
                                           args.atOrUndefined(isolate, 1))); \
  }
#define DISPATCH_STRCMP_METHOD2(type, sc_name, cc_name)                      \
  if (strcmp(prop_name.get(), #cc_name) == 0) {                              \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate, JSTemporal##type::sc_name(isolate, obj,                     \
                                           args.atOrUndefined(isolate, 1),   \
                                           args.atOrUndefined(isolate, 2))); \
  }
#define DISPATCH_VALUEOF(type)                                             \
  if (prop->Equals(*factory->valueOf_string())) {                          \
    const char* msg = "use Temporal." #type ".compare for comparison.";    \
    THROW_NEW_ERROR_RETURN_FAILURE(                                        \
        isolate,                                                           \
        NewTypeError(MessageTemplate::kDoNotUse,                           \
                     isolate->factory()->NewStringFromAsciiChecked(        \
                         method_name.c_str()),                             \
                     isolate->factory()->NewStringFromAsciiChecked(msg))); \
  }

  // Perform cheap pointer comparisons first, then more expensive string
  // comparisons for strings that are not already interned for other reasons.
  // Of course we could intern those strings as well in heap-symbols.h, but as
  // a proof of concept let's prefer to trade runtime performance away to gain
  // size savings.

  switch (ctor_type) {
    case kPlainDate: {
      CHECK_RECEIVER(JSTemporalPlainDate, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(calendar, calendar);
      DISPATCH_METHOD1(PlainDate, ToString, toString);
      DISPATCH_METHOD2(PlainDate, Add, add);
      DISPATCH_METHOD2(PlainDate, With, with);
      DISPATCH_METHOD0(PlainDate, ToJSON, toJSON);
      if (prop->Equals(*factory->year_string()) ||
#ifdef V8_INTL_SUPPORT
          prop->Equals(*factory->era_string()) ||
          prop->Equals(*factory->eraYear_string()) ||
#endif
          prop->Equals(*factory->month_string()) ||
          prop->Equals(*factory->monthCode_string()) ||
          prop->Equals(*factory->day_string()) ||
          prop->Equals(*factory->dayOfWeek_string()) ||
          prop->Equals(*factory->dayOfYear_string()) ||
          prop->Equals(*factory->weekOfYear_string()) ||
          prop->Equals(*factory->daysInWeek_string()) ||
          prop->Equals(*factory->daysInMonth_string()) ||
          prop->Equals(*factory->daysInYear_string()) ||
          prop->Equals(*factory->monthsInYear_string()) ||
          prop->Equals(*factory->inLeapYear_string())) {
        Handle<JSReceiver> calendar{obj->calendar(), isolate};
        RETURN_RESULT_OR_FAILURE(isolate, temporal::InvokeCalendarMethod(
                                              isolate, calendar, prop, obj));
      }
      DISPATCH_VALUEOF(PlainDate);
      DISPATCH_STRCMP_METHOD0(PlainDate, ToPlainYearMonth, toPlainYearMonth);
      DISPATCH_STRCMP_METHOD0(PlainDate, ToPlainMonthDay, toPlainMonthDay);
      DISPATCH_STRCMP_METHOD2(PlainDate, Subtract, subtract);
      DISPATCH_STRCMP_METHOD1(PlainDate, WithCalendar, withCalendar);
      DISPATCH_STRCMP_METHOD0(PlainDate, GetISOFields, getISOFields);
      DISPATCH_STRCMP_METHOD2(PlainDate, Since, since);
      DISPATCH_STRCMP_METHOD2(PlainDate, Until, until);
      DISPATCH_STRCMP_METHOD1(PlainDate, ToPlainDateTime, toPlainDateTime);
      DISPATCH_STRCMP_METHOD1(PlainDate, ToZonedDateTime, toZonedDateTime);
      DISPATCH_STRCMP_METHOD1(PlainDate, Equals, equals);
      DISPATCH_STRCMP_METHOD2(PlainDate, ToLocaleString, toLocaleString);
      UNREACHABLE();
    }
    case kPlainTime: {
      CHECK_RECEIVER(JSTemporalPlainTime, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(calendar, calendar);
      DISPATCH_ISO_GETTER(hour);
      DISPATCH_ISO_GETTER(minute);
      DISPATCH_ISO_GETTER(second);
      DISPATCH_ISO_GETTER(millisecond);
      DISPATCH_ISO_GETTER(microsecond);
      DISPATCH_ISO_GETTER(nanosecond);
      DISPATCH_METHOD1(PlainTime, ToString, toString);
      DISPATCH_METHOD1(PlainTime, Add, add);
      DISPATCH_METHOD2(PlainTime, With, with);
      DISPATCH_METHOD0(PlainTime, ToJSON, toJSON);
      DISPATCH_VALUEOF(PlainTime);
      DISPATCH_STRCMP_METHOD1(PlainTime, ToZonedDateTime, toZonedDateTime);
      DISPATCH_STRCMP_METHOD1(PlainTime, Equals, equals);
      DISPATCH_STRCMP_METHOD1(PlainTime, Subtract, subtract);
      DISPATCH_STRCMP_METHOD0(PlainTime, GetISOFields, getISOFields);
      DISPATCH_STRCMP_METHOD1(PlainTime, Round, round);
      DISPATCH_STRCMP_METHOD2(PlainTime, Since, since);
      DISPATCH_STRCMP_METHOD1(PlainTime, ToPlainDateTime, toPlainDateTime);
      DISPATCH_STRCMP_METHOD2(PlainTime, ToLocaleString, toLocaleString);
      DISPATCH_STRCMP_METHOD2(PlainTime, Until, until);
      UNREACHABLE();
    }
    case kPlainDateTime: {
      CHECK_RECEIVER(JSTemporalPlainDateTime, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(calendar, calendar);
      DISPATCH_ISO_GETTER(hour);
      DISPATCH_ISO_GETTER(minute);
      DISPATCH_ISO_GETTER(second);
      DISPATCH_ISO_GETTER(millisecond);
      DISPATCH_ISO_GETTER(microsecond);
      DISPATCH_ISO_GETTER(nanosecond);
      DISPATCH_METHOD1(PlainDateTime, ToString, toString);
      DISPATCH_METHOD2(PlainDateTime, Add, add);
      DISPATCH_METHOD2(PlainDateTime, With, with);
      DISPATCH_METHOD0(PlainDateTime, ToJSON, toJSON);
      if (prop->Equals(*factory->year_string()) ||
#ifdef V8_INTL_SUPPORT
          prop->Equals(*factory->era_string()) ||
          prop->Equals(*factory->eraYear_string()) ||
#endif
          prop->Equals(*factory->month_string()) ||
          prop->Equals(*factory->monthCode_string()) ||
          prop->Equals(*factory->day_string()) ||
          prop->Equals(*factory->dayOfWeek_string()) ||
          prop->Equals(*factory->dayOfYear_string()) ||
          prop->Equals(*factory->weekOfYear_string()) ||
          prop->Equals(*factory->daysInWeek_string()) ||
          prop->Equals(*factory->daysInMonth_string()) ||
          prop->Equals(*factory->daysInYear_string()) ||
          prop->Equals(*factory->monthsInYear_string()) ||
          prop->Equals(*factory->inLeapYear_string())) {
        Handle<JSReceiver> calendar{obj->calendar(), isolate};
        RETURN_RESULT_OR_FAILURE(isolate, temporal::InvokeCalendarMethod(
                                              isolate, calendar, prop, obj));
      }
      DISPATCH_VALUEOF(PlainDateTime);
      DISPATCH_STRCMP_METHOD1(PlainDateTime, WithCalendar, withCalendar);
      DISPATCH_STRCMP_METHOD1(PlainDateTime, WithPlainTime, withPlainTime);
      DISPATCH_STRCMP_METHOD1(PlainDateTime, Equals, equals);
      DISPATCH_STRCMP_METHOD0(PlainDateTime, ToPlainYearMonth,
                              toPlainYearMonth);
      DISPATCH_STRCMP_METHOD0(PlainDateTime, ToPlainMonthDay, toPlainMonthDay);
      DISPATCH_STRCMP_METHOD2(PlainDateTime, ToZonedDateTime, toZonedDateTime);
      DISPATCH_STRCMP_METHOD0(PlainDateTime, GetISOFields, getISOFields);
      DISPATCH_STRCMP_METHOD1(PlainDateTime, WithPlainDate, withPlainDate);
      DISPATCH_STRCMP_METHOD1(PlainDateTime, Round, round);
      DISPATCH_STRCMP_METHOD2(PlainDateTime, Since, since);
      DISPATCH_STRCMP_METHOD2(PlainDateTime, Subtract, subtract);
      DISPATCH_STRCMP_METHOD0(PlainDateTime, ToPlainDate, toPlainDate);
      DISPATCH_STRCMP_METHOD0(PlainDateTime, ToPlainTime, toPlainTime);
      DISPATCH_STRCMP_METHOD2(PlainDateTime, ToLocaleString, toLocaleString);
      DISPATCH_STRCMP_METHOD2(PlainDateTime, Until, until);
      UNREACHABLE();
    }
    case kPlainYearMonth: {
      CHECK_RECEIVER(JSTemporalPlainYearMonth, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(calendar, calendar);
      DISPATCH_METHOD1(PlainYearMonth, ToString, toString);
      DISPATCH_METHOD2(PlainYearMonth, Add, add);
      DISPATCH_METHOD2(PlainYearMonth, With, with);
      DISPATCH_METHOD0(PlainYearMonth, ToJSON, toJSON);
      if (prop->Equals(*factory->year_string()) ||
#ifdef V8_INTL_SUPPORT
          prop->Equals(*factory->era_string()) ||
          prop->Equals(*factory->eraYear_string()) ||
#endif
          prop->Equals(*factory->month_string()) ||
          prop->Equals(*factory->monthCode_string()) ||
          prop->Equals(*factory->daysInMonth_string()) ||
          prop->Equals(*factory->daysInYear_string()) ||
          prop->Equals(*factory->monthsInYear_string()) ||
          prop->Equals(*factory->inLeapYear_string())) {
        Handle<JSReceiver> calendar{obj->calendar(), isolate};
        RETURN_RESULT_OR_FAILURE(isolate, temporal::InvokeCalendarMethod(
                                              isolate, calendar, prop, obj));
      }
      DISPATCH_VALUEOF(PlainYearMonth);
      DISPATCH_STRCMP_METHOD2(PlainYearMonth, Subtract, subtract);
      DISPATCH_STRCMP_METHOD1(PlainYearMonth, Equals, equals);
      DISPATCH_STRCMP_METHOD1(PlainYearMonth, ToPlainDate, toPlainDate);
      DISPATCH_STRCMP_METHOD0(PlainYearMonth, GetISOFields, getISOFields);
      DISPATCH_STRCMP_METHOD2(PlainYearMonth, Since, since);
      DISPATCH_STRCMP_METHOD2(PlainYearMonth, ToLocaleString, toLocaleString);
      DISPATCH_STRCMP_METHOD2(PlainYearMonth, Until, until);
      UNREACHABLE();
    }
    case kPlainMonthDay: {
      CHECK_RECEIVER(JSTemporalPlainMonthDay, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(calendar, calendar);
      DISPATCH_METHOD1(PlainMonthDay, ToString, toString);
      DISPATCH_METHOD2(PlainMonthDay, With, with);
      DISPATCH_METHOD0(PlainMonthDay, ToJSON, toJSON);
      if (prop->Equals(*factory->monthCode_string()) ||
          prop->Equals(*factory->day_string())) {
        Handle<JSReceiver> calendar{obj->calendar(), isolate};
        RETURN_RESULT_OR_FAILURE(isolate, temporal::InvokeCalendarMethod(
                                              isolate, calendar, prop, obj));
      }
      DISPATCH_VALUEOF(PlainMonthDay);
      DISPATCH_STRCMP_METHOD1(PlainMonthDay, Equals, equals);
      DISPATCH_STRCMP_METHOD1(PlainMonthDay, ToPlainDate, toPlainDate);
      DISPATCH_STRCMP_METHOD0(PlainMonthDay, GetISOFields, getISOFields);
      DISPATCH_STRCMP_METHOD2(PlainMonthDay, ToLocaleString, toLocaleString);
      UNREACHABLE();
    }
    case kZonedDateTime: {
      CHECK_RECEIVER(JSTemporalZonedDateTime, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(calendar, calendar);
      DISPATCH_DIRECT_GETTER(time_zone, timeZone);
      DISPATCH_DIRECT_GETTER(nanoseconds, epochNanoseconds);
      DISPATCH_SCALED_EXACT_TIME_GETTER(epochMicroseconds, 1000, true);
      DISPATCH_SCALED_EXACT_TIME_GETTER(epochMilliseconds, 1'000'000, false);
      DISPATCH_SCALED_EXACT_TIME_GETTER(epochSeconds, 1'000'000'000, false);
      DISPATCH_METHOD1(ZonedDateTime, ToString, toString);
      DISPATCH_METHOD2(ZonedDateTime, Add, add);
      DISPATCH_METHOD2(ZonedDateTime, With, with);
      DISPATCH_METHOD0(ZonedDateTime, OffsetNanoseconds, offsetNanoseconds);
      DISPATCH_METHOD0(ZonedDateTime, Offset, offset);
      DISPATCH_METHOD0(ZonedDateTime, HoursInDay, hoursInDay);
      DISPATCH_METHOD0(ZonedDateTime, ToJSON, toJSON);
      if (prop->Equals(*factory->year_string()) ||
#ifdef V8_INTL_SUPPORT
          prop->Equals(*factory->era_string()) ||
          prop->Equals(*factory->eraYear_string()) ||
#endif
          prop->Equals(*factory->month_string()) ||
          prop->Equals(*factory->monthCode_string()) ||
          prop->Equals(*factory->day_string()) ||
          prop->Equals(*factory->dayOfWeek_string()) ||
          prop->Equals(*factory->dayOfYear_string()) ||
          prop->Equals(*factory->weekOfYear_string()) ||
          prop->Equals(*factory->daysInWeek_string()) ||
          prop->Equals(*factory->daysInMonth_string()) ||
          prop->Equals(*factory->daysInYear_string()) ||
          prop->Equals(*factory->monthsInYear_string()) ||
          prop->Equals(*factory->inLeapYear_string())) {
        // 3. Let timeZone be zonedDateTime.[[TimeZone]].
        Handle<JSReceiver> time_zone{obj->time_zone(), isolate};
        // 4. Let instant be ?
        // CreateTemporalInstant(zonedDateTime.[[Nanoseconds]]).
        Handle<JSTemporalInstant> instant;
        Handle<BigInt> epoch_nanoseconds{obj->nanoseconds(), isolate};
        ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
            isolate, instant,
            temporal::CreateTemporalInstant(isolate, epoch_nanoseconds));
        // 5. Let calendar be zonedDateTime.[[Calendar]].
        Handle<JSReceiver> calendar{obj->calendar(), isolate};
        // 6. Let temporalDateTime be ? GetPlainDateTimeFor(timeZone, instant,
        //    calendar).
        Handle<JSTemporalPlainDateTime> arg;
        ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
            isolate, arg,
            temporal::BuiltinTimeZoneGetPlainDateTimeFor(
                isolate, time_zone, instant, calendar,
                prop->ToCString().get()));

        RETURN_RESULT_OR_FAILURE(isolate, temporal::InvokeCalendarMethod(
                                              isolate, calendar, prop, arg));
      }
      if (prop->Equals(*factory->hour_string()) ||
          prop->Equals(*factory->minute_string()) ||
          prop->Equals(*factory->second_string()) ||
          prop->Equals(*factory->millisecond_string()) ||
          prop->Equals(*factory->microsecond_string()) ||
          prop->Equals(*factory->nanosecond_string())) {
        // 3. Let timeZone be zonedDateTime.[[TimeZone]].
        Handle<JSReceiver> time_zone{obj->time_zone(), isolate};
        // 4. Let instant be ?
        // CreateTemporalInstant(zonedDateTime.[[Nanoseconds]]).
        Handle<JSTemporalInstant> instant;
        Handle<BigInt> epoch_nanoseconds{obj->nanoseconds(), isolate};
        ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
            isolate, instant,
            temporal::CreateTemporalInstant(isolate, epoch_nanoseconds));
        // 5. Let calendar be zonedDateTime.[[Calendar]].
        Handle<JSReceiver> calendar{obj->calendar(), isolate};
        // 6. Let temporalDateTime be ? GetPlainDateTimeFor(timeZone, instant,
        //    calendar).
        Handle<JSTemporalPlainDateTime> arg;
        ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
            isolate, arg,
            temporal::BuiltinTimeZoneGetPlainDateTimeFor(
                isolate, time_zone, instant, calendar,
                prop->ToCString().get()));

        {
          Handle<JSTemporalPlainDateTime> obj = arg;  // shadow outer 'obj'
          DISPATCH_ISO_GETTER(hour);
          DISPATCH_ISO_GETTER(minute);
          DISPATCH_ISO_GETTER(second);
          DISPATCH_ISO_GETTER(millisecond);
          DISPATCH_ISO_GETTER(microsecond);
          DISPATCH_ISO_GETTER(nanosecond);
        }
        UNREACHABLE();
      }
      DISPATCH_VALUEOF(ZonedDateTime);
      DISPATCH_STRCMP_METHOD1(ZonedDateTime, Equals, equals);
      DISPATCH_STRCMP_METHOD1(ZonedDateTime, WithCalendar, withCalendar);
      DISPATCH_STRCMP_METHOD1(ZonedDateTime, WithPlainDate, withPlainDate);
      DISPATCH_STRCMP_METHOD1(ZonedDateTime, WithPlainTime, withPlainTime);
      DISPATCH_STRCMP_METHOD1(ZonedDateTime, WithTimeZone, withTimeZone);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, ToPlainYearMonth,
                              toPlainYearMonth);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, ToPlainMonthDay, toPlainMonthDay);
      DISPATCH_STRCMP_METHOD1(ZonedDateTime, Round, round);
      DISPATCH_STRCMP_METHOD2(ZonedDateTime, Subtract, subtract);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, GetISOFields, getISOFields);
      DISPATCH_STRCMP_METHOD2(ZonedDateTime, Since, since);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, StartOfDay, startOfDay);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, ToInstant, toInstant);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, ToPlainDate, toPlainDate);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, ToPlainTime, toPlainTime);
      DISPATCH_STRCMP_METHOD0(ZonedDateTime, ToPlainDateTime, toPlainDateTime);
      DISPATCH_STRCMP_METHOD2(ZonedDateTime, ToLocaleString, toLocaleString);
      DISPATCH_STRCMP_METHOD2(ZonedDateTime, Until, until);
      UNREACHABLE();
    }
    case kDuration: {
      CHECK_RECEIVER(JSTemporalDuration, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(years, years);
      DISPATCH_DIRECT_GETTER(months, months);
      DISPATCH_DIRECT_GETTER(weeks, weeks);
      DISPATCH_DIRECT_GETTER(days, days);
      DISPATCH_DIRECT_GETTER(hours, hours);
      DISPATCH_DIRECT_GETTER(minutes, minutes);
      DISPATCH_DIRECT_GETTER(seconds, seconds);
      DISPATCH_DIRECT_GETTER(milliseconds, milliseconds);
      DISPATCH_DIRECT_GETTER(microseconds, microseconds);
      DISPATCH_DIRECT_GETTER(nanoseconds, nanoseconds);
      DISPATCH_METHOD1(Duration, ToString, toString);
      DISPATCH_METHOD0(Duration, Sign, sign);
      DISPATCH_METHOD0(Duration, Blank, blank);
      DISPATCH_METHOD2(Duration, Add, add);
      DISPATCH_METHOD1(Duration, Total, total);
      DISPATCH_METHOD1(Duration, With, with);
      DISPATCH_METHOD0(Duration, ToJSON, toJSON);
      DISPATCH_VALUEOF(Duration);
      DISPATCH_STRCMP_METHOD1(Duration, Round, round);
      DISPATCH_STRCMP_METHOD0(Duration, Negated, negated);
      DISPATCH_STRCMP_METHOD0(Duration, Abs, abs);
      DISPATCH_STRCMP_METHOD2(Duration, Subtract, subtract);
      DISPATCH_STRCMP_METHOD2(Duration, ToLocaleString, toLocaleString);
      UNREACHABLE();
    }
    case kInstant: {
      CHECK_RECEIVER(JSTemporalInstant, obj, method_name.c_str());
      DISPATCH_DIRECT_GETTER(nanoseconds, epochNanoseconds);
      DISPATCH_SCALED_EXACT_TIME_GETTER(epochMicroseconds, 1000, true);
      DISPATCH_SCALED_EXACT_TIME_GETTER(epochMilliseconds, 1'000'000, false);
      DISPATCH_SCALED_EXACT_TIME_GETTER(epochSeconds, 1'000'000'000, false);
      DISPATCH_METHOD1(Instant, ToString, toString);
      DISPATCH_METHOD1(Instant, Add, add);
      DISPATCH_METHOD0(Instant, ToJSON, toJSON);
      DISPATCH_VALUEOF(Instant);
      DISPATCH_STRCMP_METHOD1(Instant, Equals, equals);
      DISPATCH_STRCMP_METHOD1(Instant, Round, round);
      DISPATCH_STRCMP_METHOD2(Instant, Since, since);
      DISPATCH_STRCMP_METHOD1(Instant, Subtract, subtract);
      DISPATCH_STRCMP_METHOD2(Instant, ToLocaleString, toLocaleString);
      DISPATCH_STRCMP_METHOD1(Instant, ToZonedDateTime, toZonedDateTime);
      DISPATCH_STRCMP_METHOD1(Instant, ToZonedDateTimeISO, toZonedDateTimeISO);
      DISPATCH_STRCMP_METHOD2(Instant, Until, until);
      UNREACHABLE();
    }
    case kCalendar: {
      CHECK_RECEIVER(JSTemporalCalendar, obj, method_name.c_str());
      // #sec-temporal.calendar.prototype.tostring
      if (prop->Equals(*factory->toString_string())) {
        // 3. Return calendar.[[Identifier]].
        RETURN_RESULT_OR_FAILURE(
            isolate,
            JSTemporalCalendar::ToString(isolate, obj, method_name.c_str()));
      }
      // #sec-temporal.calendar.prototype.tojson/id
      if (prop->Equals(*factory->id_string()) ||
          prop->Equals(*factory->toJSON_string())) {
        // 3. Return ? ToString(calendar).
        RETURN_RESULT_OR_FAILURE(isolate, Object::ToString(isolate, obj));
      }
      DISPATCH_METHOD3(Calendar, DateAdd, dateAdd);
      DISPATCH_METHOD2(Calendar, DateFromFields, dateFromFields);
      DISPATCH_METHOD3(Calendar, DateUntil, dateUntil);
      DISPATCH_METHOD1(Calendar, Day, day);
      DISPATCH_METHOD1(Calendar, DaysInMonth, daysInMonth);
      DISPATCH_METHOD1(Calendar, DaysInWeek, daysInWeek);
      DISPATCH_METHOD1(Calendar, DaysInYear, daysInYear);
      DISPATCH_METHOD1(Calendar, DayOfWeek, dayOfWeek);
      DISPATCH_METHOD1(Calendar, DayOfYear, dayOfYear);
      DISPATCH_METHOD1(Calendar, InLeapYear, inLeapYear);
      DISPATCH_METHOD2(Calendar, MergeFields, mergeFields);
      DISPATCH_METHOD1(Calendar, Month, month);
      DISPATCH_METHOD1(Calendar, MonthCode, monthCode);
      DISPATCH_METHOD2(Calendar, MonthDayFromFields, monthDayFromFields);
      DISPATCH_METHOD1(Calendar, MonthsInYear, monthsInYear);
      DISPATCH_METHOD1(Calendar, Year, year);
      DISPATCH_METHOD2(Calendar, YearMonthFromFields, yearMonthFromFields);
      DISPATCH_METHOD1(Calendar, WeekOfYear, weekOfYear);
#ifdef V8_INTL_SUPPORT
      DISPATCH_METHOD1(Calendar, Era, era);
      DISPATCH_METHOD1(Calendar, EraYear, eraYear);
#endif  // V8_INTL_SUPPORT
      UNREACHABLE();
    }
    case kTimeZone: {
      CHECK_RECEIVER(JSTemporalTimeZone, obj, method_name.c_str());
      // #sec-temporal.timezone.prototype.tostring
      if (prop->Equals(*factory->toString_string())) {
        // 3. Return timeZone.[[Identifier]].
        RETURN_RESULT_OR_FAILURE(
            isolate,
            JSTemporalTimeZone::ToString(isolate, obj, method_name.c_str()));
      }
      // #sec-temporal.timezone.prototype.tojson/id
      if (prop->Equals(*factory->id_string()) ||
          prop->Equals(*factory->toJSON_string())) {
        HandleScope scope(isolate);
        // 3. Return ? ToString(timeZone).
        RETURN_RESULT_OR_FAILURE(isolate, Object::ToString(isolate, obj));
      }
      DISPATCH_METHOD1(TimeZone, GetOffsetNanosecondsFor,
                       getOffsetNanosecondsFor);
      DISPATCH_STRCMP_METHOD2(TimeZone, GetInstantFor, getInstantFor);
      DISPATCH_STRCMP_METHOD1(TimeZone, GetNextTransition, getNextTransition);
      DISPATCH_STRCMP_METHOD1(TimeZone, GetOffsetStringFor, getOffsetStringFor);
      DISPATCH_STRCMP_METHOD2(TimeZone, GetPlainDateTimeFor,
                              getPlainDateTimeFor);
      DISPATCH_STRCMP_METHOD1(TimeZone, GetPossibleInstantsFor,
                              getPossibleInstantsFor);
      DISPATCH_STRCMP_METHOD1(TimeZone, GetPreviousTransition,
                              getPreviousTransition);
      UNREACHABLE();
    }
    case kNow:
    case kNumTemporalConstructors:
      UNREACHABLE();
  }

#undef DISPATCH_DIRECT_GETTER
#undef DISPATCH_ISO_GETTER
#undef DISPATCH_SCALED_EXACT_TIME_GETTER
#undef DISPATCH_METHOD0
#undef DISPATCH_METHOD1
#undef DISPATCH_METHOD2
#undef DISPATCH_METHOD3
#undef DISPATCH_STRCMP_METHOD0
#undef DISPATCH_STRCMP_METHOD1
#undef DISPATCH_STRCMP_METHOD2
#undef DISPATCH_VALUEOF
}

}  // namespace internal
}  // namespace v8
