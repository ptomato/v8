// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/builtins/builtins-utils-inl.h"
#include "src/builtins/builtins.h"
#include "src/objects/bigint.h"
#include "src/objects/js-temporal-objects-inl.h"

namespace v8 {
namespace internal {

#define TO_BE_IMPLEMENTED(id)   \
  BUILTIN_NO_RCS(id) {          \
    HandleScope scope(isolate); \
    UNIMPLEMENTED();            \
  }

#define TEMPORAL_NOW0(T)                                            \
  BUILTIN(TemporalNow##T) {                                         \
    HandleScope scope(isolate);                                     \
    RETURN_RESULT_OR_FAILURE(isolate, JSTemporal##T::Now(isolate)); \
  }

#define TEMPORAL_NOW2(T)                                                     \
  BUILTIN(TemporalNow##T) {                                                  \
    HandleScope scope(isolate);                                              \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate, JSTemporal##T::Now(isolate, args.atOrUndefined(isolate, 1), \
                                    args.atOrUndefined(isolate, 2)));        \
  }

#define TEMPORAL_NOW_ISO1(T)                                             \
  BUILTIN(TemporalNow##T##ISO) {                                         \
    HandleScope scope(isolate);                                          \
    RETURN_RESULT_OR_FAILURE(                                            \
        isolate,                                                         \
        JSTemporal##T::NowISO(isolate, args.atOrUndefined(isolate, 1))); \
  }

#define TEMPORAL_CONSTRUCTOR1(T)                                              \
  BUILTIN(Temporal##T##Constructor) {                                         \
    HandleScope scope(isolate);                                               \
    RETURN_RESULT_OR_FAILURE(                                                 \
        isolate,                                                              \
        JSTemporal##T::Constructor(isolate, args.target(), args.new_target(), \
                                   args.atOrUndefined(isolate, 1)));          \
  }

#define TEMPORAL_PROTOTYPE_METHOD0(T, METHOD, name)                          \
  BUILTIN(Temporal##T##Prototype##METHOD) {                                  \
    HandleScope scope(isolate);                                              \
    CHECK_RECEIVER(JSTemporal##T, obj, "Temporal." #T ".prototype." #name);  \
    RETURN_RESULT_OR_FAILURE(isolate, JSTemporal##T ::METHOD(isolate, obj)); \
  }

#define TEMPORAL_PROTOTYPE_METHOD1(T, METHOD, name)                            \
  BUILTIN(Temporal##T##Prototype##METHOD) {                                    \
    HandleScope scope(isolate);                                                \
    CHECK_RECEIVER(JSTemporal##T, obj, "Temporal." #T ".prototype." #name);    \
    RETURN_RESULT_OR_FAILURE(                                                  \
        isolate,                                                               \
        JSTemporal##T ::METHOD(isolate, obj, args.atOrUndefined(isolate, 1))); \
  }

#define TEMPORAL_PROTOTYPE_METHOD2(T, METHOD, name)                          \
  BUILTIN(Temporal##T##Prototype##METHOD) {                                  \
    HandleScope scope(isolate);                                              \
    CHECK_RECEIVER(JSTemporal##T, obj, "Temporal." #T ".prototype." #name);  \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate,                                                             \
        JSTemporal##T ::METHOD(isolate, obj, args.atOrUndefined(isolate, 1), \
                               args.atOrUndefined(isolate, 2)));             \
  }

#define TEMPORAL_PROTOTYPE_METHOD3(T, METHOD, name)                          \
  BUILTIN(Temporal##T##Prototype##METHOD) {                                  \
    HandleScope scope(isolate);                                              \
    CHECK_RECEIVER(JSTemporal##T, obj, "Temporal." #T ".prototype." #name);  \
    RETURN_RESULT_OR_FAILURE(                                                \
        isolate,                                                             \
        JSTemporal##T ::METHOD(isolate, obj, args.atOrUndefined(isolate, 1), \
                               args.atOrUndefined(isolate, 2),               \
                               args.atOrUndefined(isolate, 3)));             \
  }

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

#define TEMPORAL_GET(T, METHOD, field)                                       \
  BUILTIN(Temporal##T##Prototype##METHOD) {                                  \
    HandleScope scope(isolate);                                              \
    CHECK_RECEIVER(JSTemporal##T, obj, "Temporal." #T ".prototype." #field); \
    return obj->field();                                                     \
  }

static const char* temporal_constructor_names[kNumTemporalConstructors] = {
    "Duration",      "Instant",   "PlainDate",      "PlainDateTime",
    "PlainMonthDay", "PlainTime", "PlainYearMonth", "ZonedDateTime",
};
static_assert(
    sizeof(temporal_constructor_names) / sizeof(*temporal_constructor_names) ==
        kNumTemporalConstructors,
    "Temporal constructor names must match TemporalConstructorType enum");

static inline TemporalConstructorType StashedConstructorType(
    Isolate* i, const BuiltinArguments& args) {
  Handle<Object> ctor_id_val = JSObject::GetDataProperty(
      i, args.target(), i->factory()->temporal_ctor_id_symbol());
  DCHECK(IsSmi(*ctor_id_val));
  return static_cast<TemporalConstructorType>(Smi::cast(*ctor_id_val).value());
}

// TODO: Consider storing this as a SMI index as well.
static inline Handle<String> StashedPropertyName(Isolate* i,
                                                 const BuiltinArguments& args) {
  Handle<Object> prop_val = JSObject::GetDataProperty(
      i, args.target(), i->factory()->temporal_calendar_prop_symbol());
  DCHECK(IsString(*prop_val));
  return Object::NoSideEffectsToString(i, prop_val);
}

// NOTE: Outdated. In current spec this is replaced by
// Temporal.X.prototype.calendarId
BUILTIN(TemporalObjectCalendarGetter) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);

  auto getter_name = std::string{"Temporal."} +
                     temporal_constructor_names[ctor_type] +
                     ".prototype.calendar";

  switch (ctor_type) {
    case kPlainDate: {
      CHECK_RECEIVER(JSTemporalPlainDate, date, getter_name.c_str());
      return date->calendar();
    }

    // NOTE: Outdated. In current spec, PlainTime has no calendar
    case kPlainTime: {
      CHECK_RECEIVER(JSTemporalPlainTime, time, getter_name.c_str());
      return time->calendar();
    }

    case kPlainDateTime: {
      CHECK_RECEIVER(JSTemporalPlainDateTime, date_time, getter_name.c_str());
      return date_time->calendar();
    }

    case kPlainYearMonth: {
      CHECK_RECEIVER(JSTemporalPlainYearMonth, year_month, getter_name.c_str());
      return year_month->calendar();
    }

    case kPlainMonthDay: {
      CHECK_RECEIVER(JSTemporalPlainMonthDay, month_day, getter_name.c_str());
      return month_day->calendar();
    }

    case kZonedDateTime: {
      CHECK_RECEIVER(JSTemporalZonedDateTime, zdt, getter_name.c_str());
      return zdt->calendar();
    }

    default:
      UNREACHABLE();
  }
}

BUILTIN(TemporalObjectCalendarDelegateGetter) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);
  Handle<String> prop = StashedPropertyName(isolate, args);

  auto getter_name = std::string{"Temporal."} +
                     temporal_constructor_names[ctor_type] + ".prototype." +
                     prop->ToCString().get();

  Handle<JSReceiver> calendar;
  Handle<JSReceiver> arg;

  // 2. Perform ? RequireInternalSlot(dateLike, [[InitializedTemporal#T]]).
  // 3. Let calendar be dateLike.[[Calendar]].
  // 4. Return ? Calendar#M(calendar, dateLike).
  switch (ctor_type) {
    case kPlainDate: {
      CHECK_RECEIVER(JSTemporalPlainDate, date, getter_name.c_str());
      calendar = handle(date->calendar(), isolate);
      arg = date;
    } break;

    case kPlainDateTime: {
      CHECK_RECEIVER(JSTemporalPlainDateTime, date_time, getter_name.c_str());
      calendar = handle(date_time->calendar(), isolate);
      arg = date_time;
    } break;

    case kPlainYearMonth: {
      CHECK_RECEIVER(JSTemporalPlainYearMonth, year_month, getter_name.c_str());
      calendar = handle(year_month->calendar(), isolate);
      arg = year_month;
    } break;

    case kPlainMonthDay: {
      CHECK_RECEIVER(JSTemporalPlainMonthDay, month_day, getter_name.c_str());
      calendar = handle(month_day->calendar(), isolate);
      arg = month_day;
    } break;

    case kZonedDateTime: {
      // 1. Let zonedDateTime be the this value.
      // 2. Perform ? RequireInternalSlot(zonedDateTime,
      //    [[InitializedTemporalZonedDateTime]]).
      CHECK_RECEIVER(JSTemporalZonedDateTime, zdt, getter_name.c_str());
      // 3. Let timeZone be zonedDateTime.[[TimeZone]].
      Handle<JSReceiver> time_zone{zdt->time_zone(), isolate};
      // 4. Let instant be ?
      // CreateTemporalInstant(zonedDateTime.[[Nanoseconds]]).
      Handle<JSTemporalInstant> instant;
      Handle<BigInt> epoch_nanoseconds{zdt->nanoseconds(), isolate};
      ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
          isolate, instant,
          temporal::CreateTemporalInstant(isolate, epoch_nanoseconds));
      // 5. Let calendar be zonedDateTime.[[Calendar]].
      calendar = handle(zdt->calendar(), isolate);
      // 6. Let temporalDateTime be ? GetPlainDateTimeFor(timeZone, instant,
      //    calendar).
      ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
          isolate, arg,
          temporal::BuiltinTimeZoneGetPlainDateTimeFor(
              isolate, time_zone, instant, calendar, prop->ToCString().get()));
    } break;

    default:
      UNREACHABLE();
  }

  // NOTE: Outdated. In current spec, return value from calendar method must be
  // validated if the calendar is not builtin.
  RETURN_RESULT_OR_FAILURE(
      isolate, temporal::InvokeCalendarMethod(isolate, calendar, prop, arg));
}

template <class T>
static inline int32_t GetTimeUnit(Handle<T> receiver, const char* prop) {
  if (strcmp(prop, "hour") == 0) return receiver->iso_hour();
  if (strcmp(prop, "minute") == 0) return receiver->iso_minute();
  if (strcmp(prop, "second") == 0) return receiver->iso_second();
  if (strcmp(prop, "millisecond") == 0) return receiver->iso_millisecond();
  if (strcmp(prop, "microsecond") == 0) return receiver->iso_microsecond();
  DCHECK_EQ(0, strcmp(prop, "nanosecond"));
  return receiver->iso_nanosecond();
}

BUILTIN(TemporalObjectTimeUnitGetter) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);
  Handle<String> prop = StashedPropertyName(isolate, args);

  std::unique_ptr<char[]> prop_name = prop->ToCString();
  auto getter_name = std::string{"Temporal."} +
                     temporal_constructor_names[ctor_type] + ".prototype." +
                     prop_name.get();

  int32_t result;

  switch (ctor_type) {
    case kPlainTime: {
      CHECK_RECEIVER(JSTemporalPlainTime, time, getter_name.c_str());
      result = GetTimeUnit(time, prop_name.get());
    } break;

    case kPlainDateTime: {
      CHECK_RECEIVER(JSTemporalPlainDateTime, date_time, getter_name.c_str());
      result = GetTimeUnit(date_time, prop_name.get());
    } break;

    case kZonedDateTime: {
      // 1. Let zonedDateTime be the this value.
      // 2. Perform ? RequireInternalSlot(zonedDateTime,
      //    [[InitializedTemporalZonedDateTime]]).
      CHECK_RECEIVER(JSTemporalZonedDateTime, zdt, getter_name.c_str());
      // 3. Let timeZone be zonedDateTime.[[TimeZone]].
      Handle<JSReceiver> time_zone{zdt->time_zone(), isolate};
      // 4. Let instant be ?
      // CreateTemporalInstant(zonedDateTime.[[Nanoseconds]]).
      Handle<JSTemporalInstant> instant;
      Handle<BigInt> epoch_nanoseconds{zdt->nanoseconds(), isolate};
      ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
          isolate, instant,
          temporal::CreateTemporalInstant(isolate, epoch_nanoseconds));
      // 5. Let calendar be zonedDateTime.[[Calendar]].
      Handle<JSReceiver> calendar{zdt->calendar(), isolate};
      // 6. Let temporalDateTime be ? GetPlainDateTimeFor(timeZone, instant,
      //    calendar).
      Handle<JSTemporalPlainDateTime> date_time;
      ASSIGN_RETURN_FAILURE_ON_EXCEPTION(
          isolate, date_time,
          temporal::BuiltinTimeZoneGetPlainDateTimeFor(
              isolate, time_zone, instant, calendar, prop->ToCString().get()));
      result = GetTimeUnit(date_time, prop_name.get());
    } break;

    default:
      UNREACHABLE();
  }

  return Smi::FromInt(result);
}

template <class T>
static inline Tagged<Object> GetExactTime(Isolate* isolate, Handle<T> receiver,
                                          const char* prop) {
  Handle<BigInt> epoch_nanoseconds{receiver->nanoseconds(), isolate};
  if (strcmp(prop, "epochNanoseconds") == 0) return *epoch_nanoseconds;

  uint64_t scale;
  bool is_bigint = false;
  if (strcmp(prop, "epochMicroseconds") == 0) {
    scale = 1000;
    is_bigint = true;
  } else if (strcmp(prop, "epochMilliseconds") == 0)
    scale = 1'000'000;
  else {
    DCHECK_EQ(0, strcmp(prop, "epochSeconds"));
    scale = 1'000'000'000;
  }

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

BUILTIN(TemporalObjectExactTimeGetter) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);
  Handle<String> prop = StashedPropertyName(isolate, args);

  std::unique_ptr<char[]> prop_name = prop->ToCString();
  auto getter_name = std::string{"Temporal."} +
                     temporal_constructor_names[ctor_type] + ".prototype." +
                     prop_name.get();

  Handle<Object> result;

  switch (ctor_type) {
    case kInstant: {
      CHECK_RECEIVER(JSTemporalInstant, instant, getter_name.c_str());
      return GetExactTime(isolate, instant, prop_name.get());
    }

    case kZonedDateTime: {
      CHECK_RECEIVER(JSTemporalZonedDateTime, zdt, getter_name.c_str());
      return GetExactTime(isolate, zdt, prop_name.get());
    }

    default:
      UNREACHABLE();
  }
}

BUILTIN(TemporalObjectValueOf) {
  HandleScope scope{isolate};

  TemporalConstructorType ctor_type = StashedConstructorType(isolate, args);

  auto msg1 = std::string{"Temporal."} + temporal_constructor_names[ctor_type] +
              ".prototype.valueOf";
  auto msg2 = std::string{"use Temporal."} +
              temporal_constructor_names[ctor_type] +
              ".compare for comparison.";

  THROW_NEW_ERROR_RETURN_FAILURE(
      isolate,
      NewTypeError(
          MessageTemplate::kDoNotUse,
          isolate->factory()->NewStringFromAsciiChecked(msg1.c_str()),
          isolate->factory()->NewStringFromAsciiChecked(msg2.c_str())));
}

// Now
TEMPORAL_NOW0(TimeZone)
TEMPORAL_NOW0(Instant)
TEMPORAL_NOW2(PlainDateTime)
TEMPORAL_NOW_ISO1(PlainDateTime)
TEMPORAL_NOW2(PlainDate)
TEMPORAL_NOW_ISO1(PlainDate)

// There is NO Temporal.now.plainTime
// See https://github.com/tc39/proposal-temporal/issues/1540
TEMPORAL_NOW_ISO1(PlainTime)
TEMPORAL_NOW2(ZonedDateTime)
TEMPORAL_NOW_ISO1(ZonedDateTime)

// PlainDate
BUILTIN(TemporalPlainDateConstructor) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(
      isolate, JSTemporalPlainDate::Constructor(
                   isolate, args.target(), args.new_target(),
                   args.atOrUndefined(isolate, 1),    // iso_year
                   args.atOrUndefined(isolate, 2),    // iso_month
                   args.atOrUndefined(isolate, 3),    // iso_day
                   args.atOrUndefined(isolate, 4)));  // calendar_like
}
TEMPORAL_METHOD2(PlainDate, From)
TEMPORAL_METHOD2(PlainDate, Compare)
TEMPORAL_PROTOTYPE_METHOD0(PlainDate, ToPlainYearMonth, toPlainYearMonth)
TEMPORAL_PROTOTYPE_METHOD0(PlainDate, ToPlainMonthDay, toPlainMonthDay)
TEMPORAL_PROTOTYPE_METHOD2(PlainDate, Add, add)
TEMPORAL_PROTOTYPE_METHOD2(PlainDate, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD1(PlainDate, WithCalendar, withCalendar)
TEMPORAL_PROTOTYPE_METHOD2(PlainDate, With, with)
TEMPORAL_PROTOTYPE_METHOD0(PlainDate, GetISOFields, getISOFields)
TEMPORAL_PROTOTYPE_METHOD2(PlainDate, Since, since)
TEMPORAL_PROTOTYPE_METHOD2(PlainDate, Until, until)
TEMPORAL_PROTOTYPE_METHOD1(PlainDate, ToPlainDateTime, toPlainDateTime)
TEMPORAL_PROTOTYPE_METHOD1(PlainDate, ToZonedDateTime, toZonedDateTime)
TEMPORAL_PROTOTYPE_METHOD1(PlainDate, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD0(PlainDate, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD2(PlainDate, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(PlainDate, ToString, toString)

// PlainTime
BUILTIN(TemporalPlainTimeConstructor) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(isolate,
                           JSTemporalPlainTime::Constructor(
                               isolate, args.target(), args.new_target(),
                               args.atOrUndefined(isolate, 1),    // hour
                               args.atOrUndefined(isolate, 2),    // minute
                               args.atOrUndefined(isolate, 3),    // second
                               args.atOrUndefined(isolate, 4),    // millisecond
                               args.atOrUndefined(isolate, 5),    // microsecond
                               args.atOrUndefined(isolate, 6)));  // nanosecond
}
TEMPORAL_METHOD2(PlainTime, From)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, ToZonedDateTime, toZonedDateTime)
TEMPORAL_METHOD2(PlainTime, Compare)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, Add, add)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD0(PlainTime, GetISOFields, getISOFields)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, Round, round)
TEMPORAL_PROTOTYPE_METHOD2(PlainTime, Since, since)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, ToPlainDateTime, toPlainDateTime)
TEMPORAL_PROTOTYPE_METHOD0(PlainTime, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD2(PlainTime, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(PlainTime, ToString, toString)
TEMPORAL_PROTOTYPE_METHOD2(PlainTime, Until, until)
TEMPORAL_PROTOTYPE_METHOD2(PlainTime, With, with)

// PlainDateTime
BUILTIN(TemporalPlainDateTimeConstructor) {
  HandleScope scope(isolate);
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
}
TEMPORAL_PROTOTYPE_METHOD1(PlainDateTime, WithCalendar, withCalendar)
TEMPORAL_PROTOTYPE_METHOD1(PlainDateTime, WithPlainTime, withPlainTime)
TEMPORAL_METHOD2(PlainDateTime, From)
TEMPORAL_METHOD2(PlainDateTime, Compare)
TEMPORAL_PROTOTYPE_METHOD1(PlainDateTime, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD0(PlainDateTime, ToPlainYearMonth, toPlainYearMonth)
TEMPORAL_PROTOTYPE_METHOD0(PlainDateTime, ToPlainMonthDay, toPlainMonthDay)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, ToZonedDateTime, toZonedDateTime)
TEMPORAL_PROTOTYPE_METHOD0(PlainDateTime, GetISOFields, getISOFields)
TEMPORAL_PROTOTYPE_METHOD1(PlainDateTime, WithPlainDate, withPlainDate)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, With, with)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, Add, add)
TEMPORAL_PROTOTYPE_METHOD1(PlainDateTime, Round, round)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, Since, since)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD0(PlainDateTime, ToPlainDate, toPlainDate)
TEMPORAL_PROTOTYPE_METHOD0(PlainDateTime, ToPlainTime, toPlainTime)
TEMPORAL_PROTOTYPE_METHOD0(PlainDateTime, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(PlainDateTime, ToString, toString)
TEMPORAL_PROTOTYPE_METHOD2(PlainDateTime, Until, until)

// PlainYearMonth
BUILTIN(TemporalPlainYearMonthConstructor) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(
      isolate, JSTemporalPlainYearMonth::Constructor(
                   isolate, args.target(), args.new_target(),
                   args.atOrUndefined(isolate, 1),    // iso_year
                   args.atOrUndefined(isolate, 2),    // iso_month
                   args.atOrUndefined(isolate, 3),    // calendar_like
                   args.atOrUndefined(isolate, 4)));  // reference_iso_day
}
TEMPORAL_METHOD2(PlainYearMonth, From)
TEMPORAL_METHOD2(PlainYearMonth, Compare)
TEMPORAL_PROTOTYPE_METHOD2(PlainYearMonth, Add, add)
TEMPORAL_PROTOTYPE_METHOD2(PlainYearMonth, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD1(PlainYearMonth, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD2(PlainYearMonth, With, with)
TEMPORAL_PROTOTYPE_METHOD1(PlainYearMonth, ToPlainDate, toPlainDate)
TEMPORAL_PROTOTYPE_METHOD0(PlainYearMonth, GetISOFields, getISOFields)
TEMPORAL_PROTOTYPE_METHOD2(PlainYearMonth, Since, since)
TEMPORAL_PROTOTYPE_METHOD2(PlainYearMonth, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD0(PlainYearMonth, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD1(PlainYearMonth, ToString, toString)
TEMPORAL_PROTOTYPE_METHOD2(PlainYearMonth, Until, until)

// PlainMonthDay
BUILTIN(TemporalPlainMonthDayConstructor) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(
      isolate, JSTemporalPlainMonthDay::Constructor(
                   isolate, args.target(), args.new_target(),
                   args.atOrUndefined(isolate, 1),    // iso_month
                   args.atOrUndefined(isolate, 2),    // iso_day
                   args.atOrUndefined(isolate, 3),    // calendar_like
                   args.atOrUndefined(isolate, 4)));  // reference_iso_year
}
TEMPORAL_METHOD2(PlainMonthDay, From)
TEMPORAL_PROTOTYPE_METHOD1(PlainMonthDay, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD2(PlainMonthDay, With, with)
TEMPORAL_PROTOTYPE_METHOD1(PlainMonthDay, ToPlainDate, toPlainDate)
TEMPORAL_PROTOTYPE_METHOD0(PlainMonthDay, GetISOFields, getISOFields)
TEMPORAL_PROTOTYPE_METHOD0(PlainMonthDay, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD2(PlainMonthDay, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(PlainMonthDay, ToString, toString)

// ZonedDateTime

BUILTIN(TemporalZonedDateTimeConstructor) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(
      isolate, JSTemporalZonedDateTime::Constructor(
                   isolate, args.target(), args.new_target(),
                   args.atOrUndefined(isolate, 1),    // epoch_nanoseconds
                   args.atOrUndefined(isolate, 2),    // time_zone_like
                   args.atOrUndefined(isolate, 3)));  // calendar_like
}
TEMPORAL_METHOD2(ZonedDateTime, From)
TEMPORAL_METHOD2(ZonedDateTime, Compare)
TEMPORAL_GET(ZonedDateTime, TimeZone, time_zone)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, HoursInDay, hoursInDay)
TEMPORAL_PROTOTYPE_METHOD2(ZonedDateTime, With, with)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, WithCalendar, withCalendar)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, WithPlainDate, withPlainDate)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, WithPlainTime, withPlainTime)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, WithTimeZone, withTimeZone)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToPlainYearMonth, toPlainYearMonth)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToPlainMonthDay, toPlainMonthDay)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, Round, round)
TEMPORAL_PROTOTYPE_METHOD2(ZonedDateTime, Add, add)
TEMPORAL_PROTOTYPE_METHOD2(ZonedDateTime, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, GetISOFields, getISOFields)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, OffsetNanoseconds, offsetNanoseconds)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, Offset, offset)
TEMPORAL_PROTOTYPE_METHOD2(ZonedDateTime, Since, since)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, StartOfDay, startOfDay)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToInstant, toInstant)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToPlainDate, toPlainDate)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToPlainTime, toPlainTime)
TEMPORAL_PROTOTYPE_METHOD0(ZonedDateTime, ToPlainDateTime, toPlainDateTime)
TEMPORAL_PROTOTYPE_METHOD2(ZonedDateTime, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(ZonedDateTime, ToString, toString)
TEMPORAL_PROTOTYPE_METHOD2(ZonedDateTime, Until, until)

// Duration
BUILTIN(TemporalDurationConstructor) {
  HandleScope scope(isolate);
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
}

BUILTIN(TemporalDurationCompare) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(isolate, JSTemporalDuration::Compare(
                                        isolate, args.atOrUndefined(isolate, 1),
                                        args.atOrUndefined(isolate, 2),
                                        args.atOrUndefined(isolate, 3)));
}
TEMPORAL_METHOD1(Duration, From)
TEMPORAL_GET(Duration, Years, years)
TEMPORAL_GET(Duration, Months, months)
TEMPORAL_GET(Duration, Weeks, weeks)
TEMPORAL_GET(Duration, Days, days)
TEMPORAL_GET(Duration, Hours, hours)
TEMPORAL_GET(Duration, Minutes, minutes)
TEMPORAL_GET(Duration, Seconds, seconds)
TEMPORAL_GET(Duration, Milliseconds, milliseconds)
TEMPORAL_GET(Duration, Microseconds, microseconds)
TEMPORAL_GET(Duration, Nanoseconds, nanoseconds)
TEMPORAL_PROTOTYPE_METHOD1(Duration, Round, round)
TEMPORAL_PROTOTYPE_METHOD1(Duration, Total, total)
TEMPORAL_PROTOTYPE_METHOD1(Duration, With, with)
TEMPORAL_PROTOTYPE_METHOD0(Duration, Sign, sign)
TEMPORAL_PROTOTYPE_METHOD0(Duration, Blank, blank)
TEMPORAL_PROTOTYPE_METHOD0(Duration, Negated, negated)
TEMPORAL_PROTOTYPE_METHOD0(Duration, Abs, abs)
TEMPORAL_PROTOTYPE_METHOD2(Duration, Add, add)
TEMPORAL_PROTOTYPE_METHOD2(Duration, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD0(Duration, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD2(Duration, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(Duration, ToString, toString)

// Instant
TEMPORAL_CONSTRUCTOR1(Instant)
TEMPORAL_METHOD1(Instant, FromEpochSeconds)
TEMPORAL_METHOD1(Instant, FromEpochMilliseconds)
TEMPORAL_METHOD1(Instant, FromEpochMicroseconds)
TEMPORAL_METHOD1(Instant, FromEpochNanoseconds)
TEMPORAL_METHOD1(Instant, From)
TEMPORAL_METHOD2(Instant, Compare)
TEMPORAL_PROTOTYPE_METHOD1(Instant, Equals, equals)
TEMPORAL_PROTOTYPE_METHOD1(Instant, Add, add)
TEMPORAL_PROTOTYPE_METHOD1(Instant, Round, round)
TEMPORAL_PROTOTYPE_METHOD2(Instant, Since, since)
TEMPORAL_PROTOTYPE_METHOD1(Instant, Subtract, subtract)
TEMPORAL_PROTOTYPE_METHOD0(Instant, ToJSON, toJSON)
TEMPORAL_PROTOTYPE_METHOD2(Instant, ToLocaleString, toLocaleString)
TEMPORAL_PROTOTYPE_METHOD1(Instant, ToString, toString)
TEMPORAL_PROTOTYPE_METHOD1(Instant, ToZonedDateTime, toZonedDateTime)
TEMPORAL_PROTOTYPE_METHOD1(Instant, ToZonedDateTimeISO, toZonedDateTimeISO)
TEMPORAL_PROTOTYPE_METHOD2(Instant, Until, until)

// Calendar
TEMPORAL_CONSTRUCTOR1(Calendar)

// #sec-get-temporal.calendar.prototype.id
BUILTIN(TemporalCalendarPrototypeId) {
  HandleScope scope(isolate);
  // 1. Let calendar be the this value.
  // 2. Perform ? RequireInternalSlot(calendar,
  // [[InitializedTemporalCalendar]]).
  CHECK_RECEIVER(JSTemporalCalendar, calendar,
                 "Temporal.Calendar.prototype.id");
  // 3. Return ? ToString(calendar).
  RETURN_RESULT_OR_FAILURE(isolate, Object::ToString(isolate, calendar));
}

// #sec-temporal.calendar.prototype.tojson
BUILTIN(TemporalCalendarPrototypeToJSON) {
  HandleScope scope(isolate);
  // 1. Let calendar be the this value.
  // 2. Perform ? RequireInternalSlot(calendar,
  // [[InitializedTemporalCalendar]]).
  CHECK_RECEIVER(JSTemporalCalendar, calendar,
                 "Temporal.Calendar.prototype.toJSON");
  // 3. Return ? ToString(calendar).
  RETURN_RESULT_OR_FAILURE(isolate, Object::ToString(isolate, calendar));
}

// #sec-temporal.calendar.prototype.tostring
BUILTIN(TemporalCalendarPrototypeToString) {
  HandleScope scope(isolate);
  const char* method_name = "Temporal.Calendar.prototype.toString";
  // 1. Let calendar be the this value.
  // 2. Perform ? RequireInternalSlot(calendar,
  // [[InitializedTemporalCalendar]]).
  CHECK_RECEIVER(JSTemporalCalendar, calendar, method_name);
  // 3. Return calendar.[[Identifier]].
  RETURN_RESULT_OR_FAILURE(
      isolate, JSTemporalCalendar::ToString(isolate, calendar, method_name));
}

TEMPORAL_PROTOTYPE_METHOD3(Calendar, DateAdd, dateAdd)
TEMPORAL_PROTOTYPE_METHOD2(Calendar, DateFromFields, dateFromFields)
TEMPORAL_PROTOTYPE_METHOD3(Calendar, DateUntil, dateUntil)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, Day, day)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, DaysInMonth, daysInMonth)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, DaysInWeek, daysInWeek)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, DaysInYear, daysInYear)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, DayOfWeek, dayOfWeek)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, DayOfYear, dayOfYear)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, InLeapYear, inLeapYear)
TEMPORAL_PROTOTYPE_METHOD2(Calendar, MergeFields, mergeFields)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, Month, month)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, MonthCode, monthCode)
TEMPORAL_PROTOTYPE_METHOD2(Calendar, MonthDayFromFields, monthDayFromFields)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, MonthsInYear, monthsInYear)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, Year, year)
TEMPORAL_PROTOTYPE_METHOD2(Calendar, YearMonthFromFields, yearMonthFromFields)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, WeekOfYear, weekOfYear)
// #sec-temporal.calendar.from
BUILTIN(TemporalCalendarFrom) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(isolate, temporal::ToTemporalCalendar(
                                        isolate, args.atOrUndefined(isolate, 1),
                                        "Temporal.Calendar.from"));
}

// TimeZone
TEMPORAL_CONSTRUCTOR1(TimeZone)
TEMPORAL_PROTOTYPE_METHOD2(TimeZone, GetInstantFor, getInstantFor)
TEMPORAL_PROTOTYPE_METHOD1(TimeZone, GetNextTransition, getNextTransition)
TEMPORAL_PROTOTYPE_METHOD1(TimeZone, GetOffsetNanosecondsFor,
                           getOffsetNanosecondsFor)
TEMPORAL_PROTOTYPE_METHOD1(TimeZone, GetOffsetStringFor, getOffsetStringFor)
TEMPORAL_PROTOTYPE_METHOD2(TimeZone, GetPlainDateTimeFor, getPlainDateTimeFor)
TEMPORAL_PROTOTYPE_METHOD1(TimeZone, GetPossibleInstantsFor,
                           getPossibleInstantFor)
TEMPORAL_PROTOTYPE_METHOD1(TimeZone, GetPreviousTransition,
                           getPreviousTransition)

// #sec-get-temporal.timezone.prototype.id
BUILTIN(TemporalTimeZonePrototypeId) {
  HandleScope scope(isolate);
  // 1. Let timeZone be the this value.
  // 2. Perform ? RequireInternalSlot(timeZone,
  // [[InitializedTemporalTimeZone]]).
  CHECK_RECEIVER(JSTemporalTimeZone, time_zone,
                 "Temporal.TimeZone.prototype.id");
  // 3. Return ? ToString(timeZone).
  RETURN_RESULT_OR_FAILURE(isolate, Object::ToString(isolate, time_zone));
}

// #sec-temporal.timezone.prototype.tojson
BUILTIN(TemporalTimeZonePrototypeToJSON) {
  HandleScope scope(isolate);
  // 1. Let timeZone be the this value.
  // 2. Perform ? RequireInternalSlot(timeZone,
  // [[InitializedTemporalTimeZone]]).
  CHECK_RECEIVER(JSTemporalTimeZone, time_zone,
                 "Temporal.TimeZone.prototype.toJSON");
  // 3. Return ? ToString(timeZone).
  RETURN_RESULT_OR_FAILURE(isolate, Object::ToString(isolate, time_zone));
}

// #sec-temporal.timezone.prototype.tostring
BUILTIN(TemporalTimeZonePrototypeToString) {
  HandleScope scope(isolate);
  const char* method_name = "Temporal.TimeZone.prototype.toString";
  // 1. Let timeZone be the this value.
  // 2. Perform ? RequireInternalSlot(timeZone,
  // [[InitializedTemporalTimeZone]]).
  CHECK_RECEIVER(JSTemporalTimeZone, time_zone, method_name);
  // 3. Return timeZone.[[Identifier]].
  RETURN_RESULT_OR_FAILURE(
      isolate, JSTemporalTimeZone::ToString(isolate, time_zone, method_name));
}

// #sec-temporal.timezone.from
BUILTIN(TemporalTimeZoneFrom) {
  HandleScope scope(isolate);
  RETURN_RESULT_OR_FAILURE(isolate, temporal::ToTemporalTimeZone(
                                        isolate, args.atOrUndefined(isolate, 1),
                                        "Temporal.TimeZone.from"));
}

#ifdef V8_INTL_SUPPORT
// Temporal.Calendar.prototype.era/eraYear
TEMPORAL_PROTOTYPE_METHOD1(Calendar, Era, era)
TEMPORAL_PROTOTYPE_METHOD1(Calendar, EraYear, eraYEar)
#endif  // V8_INTL_SUPPORT
}  // namespace internal
}  // namespace v8
