// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_OBJECTS_JS_TEMPORAL_OBJECTS_H_
#define V8_OBJECTS_JS_TEMPORAL_OBJECTS_H_

#include "src/execution/isolate.h"
#include "src/heap/factory.h"
#include "src/objects/objects.h"

// Has to be the last include (doesn't have include guards):
#include "src/objects/object-macros.h"

namespace v8 {
namespace internal {

enum TemporalConstructorType {
  kDuration,
  kInstant,
  kPlainDate,
  kPlainDateTime,
  kPlainTime,
  kZonedDateTime,
  kNumTemporalConstructors,
};

#include "torque-generated/src/objects/js-temporal-objects-tq.inc"

#define DECLARE_TEMPORAL_INLINE_GETTER_SETTER(field) \
  inline void set_##field(int32_t field);            \
  inline int32_t field() const;

#define DECLARE_TEMPORAL_TIME_INLINE_GETTER_SETTER()     \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_hour)        \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_minute)      \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_second)      \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_millisecond) \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_microsecond) \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_nanosecond)

#define DECLARE_TEMPORAL_DATE_INLINE_GETTER_SETTER() \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_year)    \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_month)   \
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(iso_day)

#define TEMPORAL_UNIMPLEMENTED(T)            \
  {                                          \
    printf("TBW %s\n", __PRETTY_FUNCTION__); \
    UNIMPLEMENTED();                         \
  }

class JSTemporalPlainDate;

class JSTemporalCalendar
    : public TorqueGeneratedJSTemporalCalendar<JSTemporalCalendar, JSObject> {
 public:
  // #sec-temporal.calendar
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalCalendar> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> identifier);

  // #sec-temporal.calendar.prototype.year
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Year(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.dateadd
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> DateAdd(
      Isolate* isolate, int calendar_index, Handle<Object> date,
      Handle<Object> durations, Handle<Object> options);

  // #sec-temporal.calendar.prototype.daysinyear
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> DaysInYear(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.dayofweek
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> DayOfWeek(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.dayofyear
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> DayOfYear(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.monthsinyear
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> MonthsInYear(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.inleapyear
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> InLeapYear(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.dateuntil
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> DateUntil(
      Isolate* isolate, int calendar_index, Handle<Object> one,
      Handle<Object> two, Handle<Object> options);

  // #sec-temporal.calendar.prototype.daysinmonth
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> DaysInMonth(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.daysinweek
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> DaysInWeek(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.datefromfields
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> DateFromFields(
      Isolate* isolate, int calendar_index, Handle<Object> fields,
      Handle<Object> options);

  // #sec-temporal.calendar.prototype.fields
  V8_WARN_UNUSED_RESULT
  static MaybeHandle<FixedArray> Fields(Isolate* isolate, int calendar_index,
      Handle<Object> iterable);

  // #sec-temporal.calendar.prototype.mergefields
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSReceiver> MergeFields(
      Isolate* isolate, int calendar_index, Handle<Object> fields,
      Handle<Object> additional_fields);

  // #sec-temporal.calendar.prototype.monthcode
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> MonthCode(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.month
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Month(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.day
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Day(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.weekofyear
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> WeekOfYear(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  // #sec-temporal.calendar.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, int calendar_index, const char* method_name);

#ifdef V8_INTL_SUPPORT
  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> Era(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);

  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> EraYear(
      Isolate* isolate, int calendar_index, Handle<Object> temporal_date_like);
#endif  // V8_INTL_SUPPORT

  DECL_PRINTER(JSTemporalCalendar)

  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_CALENDAR_FLAGS()

  DECL_INT_ACCESSORS(calendar_index)

  TQ_OBJECT_CONSTRUCTORS(JSTemporalCalendar)
};

class JSTemporalDuration
    : public TorqueGeneratedJSTemporalDuration<JSTemporalDuration, JSObject> {
 public:
  // #sec-temporal.duration
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> years,
      Handle<Object> months, Handle<Object> weeks, Handle<Object> days,
      Handle<Object> hours, Handle<Object> minutes, Handle<Object> seconds,
      Handle<Object> milliseconds, Handle<Object> microseconds,
      Handle<Object> nanoseconds);

  // #sec-temporal.duration.compare
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Compare(Isolate* isolate,
                                                        Handle<Object> one,
                                                        Handle<Object> two,
                                                        Handle<Object> options);

  // #sec-temporal.duration.from
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> From(
      Isolate* isolate, Handle<Object> item);

  // #sec-get-temporal.duration.prototype.sign
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Sign(
      Isolate* isolate, Handle<JSTemporalDuration> duration);

  // #sec-get-temporal.duration.prototype.blank
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> Blank(
      Isolate* isolate, Handle<JSTemporalDuration> duration);

  // #sec-temporal.duration.prototype.negated
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Negated(
      Isolate* isolate, Handle<JSTemporalDuration> duration);

  // #sec-temporal.duration.prototype.abs
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Abs(
      Isolate* isolate, Handle<JSTemporalDuration> duration);

  // #sec-temporal.duration.prototype.add
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Add(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> other);

  // #sec-temporal.duration.prototype.subtract
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Subtract(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> other);

  // #sec-temporal.duration.prototype.round
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Round(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> round_to_obj);

  // #sec-temporal.duration.prototype.total
  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> Total(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> total_of);

  // #sec-temporal.duration.prototype.tojson
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToJSON(
      Isolate* isolate, Handle<JSTemporalDuration> duration);

  // #sec-temporal.duration.prototype.tolocalestring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToLocaleString(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> locales, Handle<Object> options);

  // #sec-temporal.duration.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> options);

  // #sec-temporal.duration.prototype.with
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> With(
      Isolate* isolate, Handle<JSTemporalDuration> duration,
      Handle<Object> temporal_duration_like);

  DECL_PRINTER(JSTemporalDuration)

  TQ_OBJECT_CONSTRUCTORS(JSTemporalDuration)
};

class JSTemporalInstant
    : public TorqueGeneratedJSTemporalInstant<JSTemporalInstant, JSObject> {
 public:
  // #sec-temporal-instant-constructor
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> epoch_nanoseconds);

  // #sec-temporal.now.instant
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> Now(
      Isolate* isolate);

  // #sec-temporal.instant.fromepochseconds
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> FromEpochSeconds(
      Isolate* isolate, Handle<Object> epoch_seconds);

  // #sec-temporal.instant.fromepochmilliseconds
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant>
  FromEpochMilliseconds(Isolate* isolate, Handle<Object> epoch_milliseconds);

  // #sec-temporal.instant.fromepochmicroseconds
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant>
  FromEpochMicroseconds(Isolate* isolate, Handle<Object> epoch_microseconds);

  // #sec-temporal.instant.fromepochnanoeconds
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant>
  FromEpochNanoseconds(Isolate* isolate, Handle<Object> epoch_nanoseconds);

  // #sec-temporal.instant.prototype.round
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> Round(
      Isolate* isolate, Handle<JSTemporalInstant> instant,
      Handle<Object> round_to);

  // #sec-temporal.instant.from
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> From(
      Isolate* isolate, Handle<Object> item);

  // #sec-temporal.instant.prototype.tozoneddatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  ToZonedDateTime(Isolate* isolate, Handle<JSTemporalInstant> instant,
                  Handle<Object> item);

  // #sec-temporal.instant.prototype.tozoneddatetimeiso
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  ToZonedDateTimeISO(Isolate* isolate, Handle<JSTemporalInstant> instant,
                     Handle<Object> item);

  // #sec-temporal.instant.compare
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Compare(Isolate* isolate,
                                                        Handle<Object> one,
                                                        Handle<Object> two);

  // #sec-temporal.instant.prototype.equals
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> Equals(
      Isolate* isolate, Handle<JSTemporalInstant> instant,
      Handle<Object> other);

  // #sec-temporal.instant.prototype.add
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> Add(
      Isolate* isolate, Handle<JSTemporalInstant> instant,
      Handle<Object> temporal_duration_like);

  // #sec-temporal.instant.prototype.subtract
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> Subtract(
      Isolate* isolate, Handle<JSTemporalInstant> instant,
      Handle<Object> temporal_duration_like);

  // #sec-temporal.instant.prototype.tojson
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToJSON(
      Isolate* isolate, Handle<JSTemporalInstant> instant);

  // #sec-temporal.instant.prototype.tolocalestring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToLocaleString(
      Isolate* isolate, Handle<JSTemporalInstant> instant,
      Handle<Object> locales, Handle<Object> options);

  // #sec-temporal.instant.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, Handle<JSTemporalInstant> instant,
      Handle<Object> options);

  // #sec-temporal.instant.prototype.until
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Until(
      Isolate* isolate, Handle<JSTemporalInstant> instant, Handle<Object> other,
      Handle<Object> options);

  // #sec-temporal.instant.prototype.since
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Since(
      Isolate* isolate, Handle<JSTemporalInstant> instant, Handle<Object> other,
      Handle<Object> options);

  DECL_PRINTER(JSTemporalInstant)

  TQ_OBJECT_CONSTRUCTORS(JSTemporalInstant)
};

class JSTemporalPlainDate
    : public TorqueGeneratedJSTemporalPlainDate<JSTemporalPlainDate, JSObject> {
 public:
  // #sec-temporal-createtemporaldate
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> iso_year,
      Handle<Object> iso_month, Handle<Object> iso_day,
      Handle<Object> calendar_like);

  // #sec-temporal.plaindate.compare
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Compare(Isolate* isolate,
                                                        Handle<Object> one,
                                                        Handle<Object> two);

  // #sec-temporal.plaindate.prototype.equals
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> Equals(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> other);

  // #sec-temporal.plaindate.prototype.withcalendar
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> WithCalendar(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> calendar_like);

  // #sec-temporal.plaindate.prototype.toplaindatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  ToPlainDateTime(Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
                  Handle<Object> temporal_time);

  // #sec-temporal.plaindate.prototype.with
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> With(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.plaindate.from
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> From(
      Isolate* isolate, Handle<Object> item, Handle<Object> options);

  // #sec-temporal.plaindate.prototype.add
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> Add(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.plaindate.prototype.subtract
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> Subtract(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.plaindate.prototype.until
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Until(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.plaindate.prototype.since
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Since(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.plaindate.prototype.getisofields
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSReceiver> GetISOFields(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date);

  // #sec-temporal.plaindate.prototype.tozoneddatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  ToZonedDateTime(Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
                  Handle<Object> item);

  // #sec-temporal.now.plaindate
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> Now(
      Isolate* isolate, Handle<Object> calendar_like,
      Handle<Object> temporal_time_zone_like);

  // #sec-temporal.now.plaindateiso
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> NowISO(
      Isolate* isolate, Handle<Object> temporal_time_zone_like);

  // #sec-temporal.plaindate.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> options);

  // #sec-temporal.plaindate.prototype.tojson
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToJSON(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date);

  // #sec-temporal.plaindate.prototype.tolocalestring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToLocaleString(
      Isolate* isolate, Handle<JSTemporalPlainDate> plain_date,
      Handle<Object> locales, Handle<Object> options);

  DECL_PRINTER(JSTemporalPlainDate)

  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_YEAR_MONTH_DAY()

  DECLARE_TEMPORAL_DATE_INLINE_GETTER_SETTER()

  TQ_OBJECT_CONSTRUCTORS(JSTemporalPlainDate)
};

class JSTemporalPlainDateTime
    : public TorqueGeneratedJSTemporalPlainDateTime<JSTemporalPlainDateTime,
                                                    JSObject> {
 public:
  // #sec-temporal-createtemporaldatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> iso_year,
      Handle<Object> iso_month, Handle<Object> iso_day, Handle<Object> hour,
      Handle<Object> minute, Handle<Object> second, Handle<Object> millisecond,
      Handle<Object> microsecond, Handle<Object> nanosecond,
      Handle<Object> calendar_like);

  // #sec-temporal.plaindatetime.prototype.withplaintime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  WithPlainTime(Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
                Handle<Object> temporal_time_like);

  // #sec-temporal.plaindatetime.prototype.withcalendar
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  WithCalendar(Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
               Handle<Object> calendar_like);

  // #sec-temporal.plaindatetime.from
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> From(
      Isolate* isolate, Handle<Object> item, Handle<Object> options);

  // #sec-temporal.plaindatetime.compare
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Compare(Isolate* isolate,
                                                        Handle<Object> one,
                                                        Handle<Object> two);

  // #sec-temporal.plaindatetime.prototype.equals
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> Equals(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> plain_date,
      Handle<Object> other);

  // #sec-temporal.plaintime.prototype.tozoneddatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  ToZonedDateTime(Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
                  Handle<Object> temporal_time_zone_like,
                  Handle<Object> options_obj);

  // #sec-temporal.plaindatetime.prototype.withplaindate
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  WithPlainDate(Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
                Handle<Object> temporal_date_date_like);

  // #sec-temporal.plaindatetime.prototype.getisofields
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSReceiver> GetISOFields(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time);

  // #sec-temporal.plaindatetime.prototype.with
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> With(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> temporal_date_time_like, Handle<Object> options);

  // #sec-temporal.plaindatetime.prototype.tojson
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToJSON(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time);

  // #sec-temporal.plaindatetime.prototype.tolocalestring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToLocaleString(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> locales, Handle<Object> options);

  // #sec-temporal.plaindatetime.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> options);

  // #sec-temporal.plaindatetime.prototype.round
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> Round(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> round_to);

  // #sec-temporal.plaindatetime.prototype.until
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Until(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.plaindatetime.prototype.since
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Since(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.plaindatetime.prototype.add
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> Add(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.plaindatetime.prototype.subtract
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> Subtract(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.now.plaindatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> Now(
      Isolate* isolate, Handle<Object> calendar_like,
      Handle<Object> temporal_time_zone_like);

  // #sec-temporal.now.plaindatetimeiso
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime> NowISO(
      Isolate* isolate, Handle<Object> temporal_time_zone_like);

  // #sec-temporal.plaindatetime.prototype.toplaindate
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> ToPlainDate(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time);

  // #sec-temporal.plaindatetime.prototype.toplaintime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> ToPlainTime(
      Isolate* isolate, Handle<JSTemporalPlainDateTime> date_time);

  DECL_PRINTER(JSTemporalPlainDateTime)

  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_YEAR_MONTH_DAY()
  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_HOUR_MINUTE_SECOND()
  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_SECOND_PARTS()

  DECLARE_TEMPORAL_DATE_INLINE_GETTER_SETTER()
  DECLARE_TEMPORAL_TIME_INLINE_GETTER_SETTER()

  TQ_OBJECT_CONSTRUCTORS(JSTemporalPlainDateTime)
};

class JSTemporalPlainTime
    : public TorqueGeneratedJSTemporalPlainTime<JSTemporalPlainTime, JSObject> {
 public:
  // #sec-temporal-plaintime-constructor
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> hour, Handle<Object> minute,
      Handle<Object> second, Handle<Object> millisecond,
      Handle<Object> microsecond, Handle<Object> nanosecond);

  // #sec-temporal.plaintime.compare
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Compare(Isolate* isolate,
                                                        Handle<Object> one,
                                                        Handle<Object> two);

  // #sec-temporal.plaintime.prototype.equals
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> Equals(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_date,
      Handle<Object> other);

  // #sec-temporal.plaintime.from
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> From(
      Isolate* isolate, Handle<Object> item, Handle<Object> options);

  // #sec-temporal.plaintime.prototype.tozoneddatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  ToZonedDateTime(Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
                  Handle<Object> item);

  // #sec-temporal.plaintime.prototype.add
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> Add(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> temporal_duration_like);

  // #sec-temporal.plaintime.prototype.subtract
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> Subtract(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> temporal_duration_like);

  // #sec-temporal.plaintime.prototype.until
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Until(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.plaintime.prototype.since
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Since(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.plaintime.prototype.round
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> Round(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> round_to);

  // #sec-temporal.plaintime.prototype.getisofields
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSReceiver> GetISOFields(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time);

  // #sec-temporal.plaintime.prototype.toplaindatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  ToPlainDateTime(Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
                  Handle<Object> temporal_date);

  // #sec-temporal.plaintime.prototype.with
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> With(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> temporal_time_like, Handle<Object> options);

  // #sec-temporal.now.plaintimeiso
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> NowISO(
      Isolate* isolate, Handle<Object> temporal_time_zone_like);

  // #sec-temporal.plaintime.prototype.tojson
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToJSON(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time);

  // #sec-temporal.plaintime.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> options);

  // #sec-temporal.plaintime.prototype.tolocalestring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToLocaleString(
      Isolate* isolate, Handle<JSTemporalPlainTime> plain_time,
      Handle<Object> locales, Handle<Object> options);

  DECL_PRINTER(JSTemporalPlainTime)

  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_HOUR_MINUTE_SECOND()
  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_SECOND_PARTS()

  DECLARE_TEMPORAL_TIME_INLINE_GETTER_SETTER()

  TQ_OBJECT_CONSTRUCTORS(JSTemporalPlainTime)
};

struct TimeZoneDataRecord;

class JSTemporalTimeZone
    : public TorqueGeneratedJSTemporalTimeZone<JSTemporalTimeZone, JSObject> {
 public:
  // #sec-temporal.now.timezone
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalTimeZone> Now(
      Isolate* isolate);

  // #sec-temporal.timezone
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalTimeZone> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> identifier);

  // #sec-temporal.timezone.prototype.getinstantfor
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> GetInstantFor(
      Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
      Handle<Object> dateTime, Handle<Object> options);

  // #sec-temporal.timezone.prototype.getplaindatetimefor
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  GetPlainDateTimeFor(Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
                      Handle<Object> instance, Handle<Object> calendar_like);

  // #sec-temporal.timezone.prototype.getnexttransition
  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> GetNextTransition(
      Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
      Handle<Object> starting_point);

  // #sec-temporal.timezone.prototype.getprevioustransition
  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> GetPreviousTransition(
      Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
      Handle<Object> starting_point);

  // #sec-temporal.timezone.prototype.getpossibleinstantsfor
  V8_WARN_UNUSED_RESULT static MaybeHandle<FixedArray> GetPossibleInstantsFor(
      Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
      Handle<Object> date_time);

  // #sec-temporal.timezone.prototype.getoffsetnanosecondsfor
  V8_WARN_UNUSED_RESULT static Maybe<int64_t> GetOffsetNanosecondsFor(
      Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
      Handle<Object> instance);

  // #sec-temporal.timezone.prototype.getoffsetstringfor
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> GetOffsetStringFor(
      Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
      Handle<Object> instance);

  // #sec-temporal.timezone.prototype.tostring
  static MaybeHandle<Object> ToString(Isolate* isolate,
                                      const TimeZoneDataRecord& time_zone_rec,
                                      const char* method_name);

  DECL_PRINTER(JSTemporalTimeZone)

  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_TIME_ZONE_FLAGS()
  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_TIME_ZONE_SUB_MILLISECONDS()

  DECL_BOOLEAN_ACCESSORS(is_offset)
  DECL_INT_ACCESSORS(offset_milliseconds_or_time_zone_index)

  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(offset_milliseconds)
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(offset_sub_milliseconds)

  int32_t time_zone_index() const;
  static constexpr int32_t kUTCTimeZoneIndex = 0;

  int64_t offset_nanoseconds() const;
  void set_offset_nanoseconds(int64_t offset_nanoseconds);

  MaybeHandle<String> id(Isolate* isolate) const;

  TQ_OBJECT_CONSTRUCTORS(JSTemporalTimeZone)
};

class JSTemporalZonedDateTime
    : public TorqueGeneratedJSTemporalZonedDateTime<JSTemporalZonedDateTime,
                                                    JSObject> {
 public:
  // #sec-temporal.zoneddatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> Constructor(
      Isolate* isolate, Handle<JSFunction> target,
      Handle<HeapObject> new_target, Handle<Object> epoch_nanoseconds,
      Handle<Object> time_zone_like, Handle<Object> calendar_like);

  // #sec-temporal.zoneddatetime.from
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> From(
      Isolate* isolate, Handle<Object> item, Handle<Object> options);

  // #sec-temporal.zoneddatetime.compare
  V8_WARN_UNUSED_RESULT static MaybeHandle<Smi> Compare(Isolate* isolate,
                                                        Handle<Object> one,
                                                        Handle<Object> two);

  // #sec-temporal.zoneddatetime.prototype.equals
  V8_WARN_UNUSED_RESULT static MaybeHandle<Oddball> Equals(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> other);

  // #sec-temporal.zoneddatetime.prototype.with
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> With(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> temporal_zoned_date_time_like, Handle<Object> options);

  // #sec-temporal.zoneddatetime.prototype.withcalendar
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  WithCalendar(Isolate* isolate,
               Handle<JSTemporalZonedDateTime> zoned_date_time,
               Handle<Object> calendar_like);

  // #sec-temporal.zoneddatetime.prototype.withplaindate
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  WithPlainDate(Isolate* isolate,
                Handle<JSTemporalZonedDateTime> zoned_date_time,
                Handle<Object> plain_date_like);

  // #sec-temporal.zoneddatetime.prototype.withplaintime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  WithPlainTime(Isolate* isolate,
                Handle<JSTemporalZonedDateTime> zoned_date_time,
                Handle<Object> plain_time_like);

  // #sec-temporal.zoneddatetime.prototype.withtimezone
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime>
  WithTimeZone(Isolate* isolate,
               Handle<JSTemporalZonedDateTime> zoned_date_time,
               Handle<Object> time_zone_like);

  // #sec-get-temporal.zoneddatetime.prototype.hoursinday
  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> HoursInDay(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.round
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> Round(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> round_to);

  // #sec-temporal.zoneddatetime.prototype.until
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Until(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> date_time,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.zoneddatetime.prototype.since
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalDuration> Since(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> date_time,
      Handle<Object> other, Handle<Object> options);

  // #sec-temporal.zoneddatetime.prototype.add
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> Add(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.zoneddatetime.prototype.subtract
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> Subtract(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> temporal_duration_like, Handle<Object> options);

  // #sec-temporal.zoneddatetime.prototype.getisofields
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSReceiver> GetISOFields(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.now.zoneddatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> Now(
      Isolate* isolate, Handle<Object> calendar_like,
      Handle<Object> temporal_time_zone_like);

  // #sec-temporal.now.zoneddatetimeiso
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> NowISO(
      Isolate* isolate, Handle<Object> temporal_time_zone_like);

  // #sec-get-temporal.zoneddatetime.prototype.offsetnanoseconds
  V8_WARN_UNUSED_RESULT static MaybeHandle<Object> OffsetNanoseconds(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-get-temporal.zoneddatetime.prototype.offset
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> Offset(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.startofday
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalZonedDateTime> StartOfDay(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.toinstant
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalInstant> ToInstant(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.toplaindate
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDate> ToPlainDate(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.toplaintime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainTime> ToPlainTime(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.toplaindatetime
  V8_WARN_UNUSED_RESULT static MaybeHandle<JSTemporalPlainDateTime>
  ToPlainDateTime(Isolate* isolate,
                  Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.tojson
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToJSON(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time);

  // #sec-temporal.zoneddatetime.prototype.tolocalestring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToLocaleString(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> locales, Handle<Object> options);

  // #sec-temporal.zoneddatetime.prototype.tostring
  V8_WARN_UNUSED_RESULT static MaybeHandle<String> ToString(
      Isolate* isolate, Handle<JSTemporalZonedDateTime> zoned_date_time,
      Handle<Object> options);

  DECL_PRINTER(JSTemporalZonedDateTime)

  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_TIME_ZONE_FLAGS()
  DEFINE_TORQUE_GENERATED_JS_TEMPORAL_TIME_ZONE_SUB_MILLISECONDS()

  DECL_BOOLEAN_ACCESSORS(time_zone_is_offset)
  DECL_INT_ACCESSORS(time_zone_offset_milliseconds_or_time_zone_index)

  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(time_zone_offset_milliseconds)
  DECLARE_TEMPORAL_INLINE_GETTER_SETTER(time_zone_offset_sub_milliseconds)

  int32_t time_zone_index() const;

  TQ_OBJECT_CONSTRUCTORS(JSTemporalZonedDateTime)
};

struct TimeZoneDataRecord {
  int64_t is_offset : 1;
  int64_t offset_ms_or_index : 28;
  int64_t offset_sub_ms : 21;

  TimeZoneDataRecord()
      : is_offset(0),
        offset_ms_or_index(JSTemporalTimeZone::kUTCTimeZoneIndex),
        offset_sub_ms(0) {}
  explicit TimeZoneDataRecord(Handle<JSTemporalTimeZone> time_zone);
  explicit TimeZoneDataRecord(Handle<JSTemporalZonedDateTime> zdt);

  Handle<String> ToIdentifier(Isolate* isolate) const;

  int32_t index() const {
    DCHECK(!is_offset);
    return offset_ms_or_index;
  }

  int64_t offset_ns() const {
    DCHECK(is_offset);
    return offset_ms_or_index * 1000000 + offset_sub_ms;
  }

  bool operator==(const TimeZoneDataRecord& other) const {
    return is_offset == other.is_offset &&
           offset_ms_or_index == other.offset_ms_or_index &&
           offset_sub_ms == other.offset_sub_ms;
  }
};

namespace temporal {

struct DateRecord {
  int32_t year;
  int32_t month;
  int32_t day;
};

struct TimeRecord {
  int32_t hour;
  int32_t minute;
  int32_t second;
  int32_t millisecond;
  int32_t microsecond;
  int32_t nanosecond;
};

struct DateTimeRecord {
  DateRecord date;
  TimeRecord time;
};

// #sec-temporal-createtemporaldatetime
V8_WARN_UNUSED_RESULT MaybeHandle<JSTemporalPlainDateTime>
CreateTemporalDateTime(Isolate* isolate, const DateTimeRecord& date_time,
                       int calendar_index);

// #sec-temporal-createtemporalcalendar
MaybeHandle<JSTemporalCalendar> CreateTemporalCalendar(Isolate* isolate,
                                                       int index);
MaybeHandle<JSTemporalCalendar> CreateTemporalCalendar(
    Isolate* isolate, Handle<String> identifier);

// #sec-temporal-createtemporaltimezone
MaybeHandle<JSTemporalTimeZone> CreateTemporalTimeZone(
    Isolate* isolate, Handle<String> identifier);

// #sec-temporal-createtemporalinstant
V8_WARN_UNUSED_RESULT MaybeHandle<JSTemporalInstant> CreateTemporalInstant(
    Isolate* isolate, Handle<JSFunction> target, Handle<HeapObject> new_target,
    Handle<BigInt> epoch_nanoseconds);
V8_WARN_UNUSED_RESULT MaybeHandle<JSTemporalInstant> CreateTemporalInstant(
    Isolate* isolate, Handle<BigInt> epoch_nanoseconds);

// #sec-temporal-calendaryear
#define DECLARE_CALENDAR_ABSTRACT_OPERATION(Name)           \
  V8_WARN_UNUSED_RESULT MaybeHandle<Object> Calendar##Name( \
      Isolate* isolate, Handle<JSReceiver> calendar,        \
      Handle<JSReceiver> date_like);
DECLARE_CALENDAR_ABSTRACT_OPERATION(Year)
DECLARE_CALENDAR_ABSTRACT_OPERATION(Month)
DECLARE_CALENDAR_ABSTRACT_OPERATION(MonthCode)
DECLARE_CALENDAR_ABSTRACT_OPERATION(Day)
DECLARE_CALENDAR_ABSTRACT_OPERATION(DayOfWeek)
DECLARE_CALENDAR_ABSTRACT_OPERATION(DayOfYear)
DECLARE_CALENDAR_ABSTRACT_OPERATION(WeekOfYear)
DECLARE_CALENDAR_ABSTRACT_OPERATION(DaysInWeek)
DECLARE_CALENDAR_ABSTRACT_OPERATION(DaysInMonth)
DECLARE_CALENDAR_ABSTRACT_OPERATION(DaysInYear)
DECLARE_CALENDAR_ABSTRACT_OPERATION(MonthsInYear)
DECLARE_CALENDAR_ABSTRACT_OPERATION(InLeapYear)

#ifdef V8_INTL_SUPPORT
DECLARE_CALENDAR_ABSTRACT_OPERATION(Era)
DECLARE_CALENDAR_ABSTRACT_OPERATION(EraYear)
#endif  //  V8_INTL_SUPPORT

#undef DECLARE_CALENDAR_ABSTRACT_OPERATION

// #sec-temporal-getiso8601calendar
Handle<JSTemporalCalendar> GetISO8601Calendar(Isolate* isolate);

// Internal Helper Functions
int32_t CalendarIndex(Isolate* isolate, Handle<String> id);
Handle<String> CalendarIdentifier(Isolate* isolate, int32_t index);

// #sec-temporal-builtintimezonegetplaindatetimefor
V8_WARN_UNUSED_RESULT MaybeHandle<JSTemporalPlainDateTime>
BuiltinTimeZoneGetPlainDateTimeFor(Isolate* isolate,
                                   const TimeZoneDataRecord& time_zone_rec,
                                   Handle<JSTemporalInstant> instant,
                                   int calendar_index, const char* method_name);

V8_WARN_UNUSED_RESULT MaybeHandle<Object> InvokeCalendarMethod(
    Isolate* isolate, int calendar_index, Handle<String> name,
    Handle<JSReceiver> temporal_like);

V8_WARN_UNUSED_RESULT MaybeHandle<JSTemporalCalendar> ToTemporalCalendar(
    Isolate* isolate, Handle<Object> temporal_calendar_like,
    const char* method_name);

V8_WARN_UNUSED_RESULT MaybeHandle<JSTemporalTimeZone> ToTemporalTimeZone(
    Isolate* isolate, Handle<Object> temporal_time_zone_like,
    const char* method_name);

V8_WARN_UNUSED_RESULT MaybeHandle<Oddball> IsInvalidTemporalCalendarField(
    Isolate* isolate, Handle<String> string, Handle<FixedArray> field_names);

MaybeHandle<JSTemporalInstant> BuiltinTimeZoneGetInstantForCompatible(
    Isolate* isolate, const TimeZoneDataRecord& time_zone_rec,
    Handle<JSTemporalPlainDateTime> date_time, const char* method_name);

// For Intl.DurationFormat

// #sec-temporal-time-duration-records
struct TimeDurationRecord {
  double days;
  double hours;
  double minutes;
  double seconds;
  double milliseconds;
  double microseconds;
  double nanoseconds;

  // #sec-temporal-createtimedurationrecord
  static Maybe<TimeDurationRecord> Create(Isolate* isolate, double days,
                                          double hours, double minutes,
                                          double seconds, double milliseconds,
                                          double microseconds,
                                          double nanoseconds);
};

// #sec-temporal-duration-records
// Cannot reuse DateDurationRecord here due to duplicate days.
struct DurationRecord {
  double years;
  double months;
  double weeks;
  TimeDurationRecord time_duration;
  // #sec-temporal-createdurationrecord
  static Maybe<DurationRecord> Create(Isolate* isolate, double years,
                                      double months, double weeks, double days,
                                      double hours, double minutes,
                                      double seconds, double milliseconds,
                                      double microseconds, double nanoseconds);
};

// #sec-temporal-topartialduration
Maybe<DurationRecord> ToPartialDuration(
    Isolate* isolate, Handle<Object> temporal_duration_like_obj,
    const DurationRecord& input);

// #sec-temporal-isvalidduration
bool IsValidDuration(Isolate* isolate, const DurationRecord& dur);

}  // namespace temporal
}  // namespace internal
}  // namespace v8
#include "src/objects/object-macros-undef.h"
#endif  // V8_OBJECTS_JS_TEMPORAL_OBJECTS_H_
