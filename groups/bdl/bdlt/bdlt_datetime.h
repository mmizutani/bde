// bdlt_datetime.h                                                    -*-C++-*-
#ifndef INCLUDED_BDLT_DATETIME
#define INCLUDED_BDLT_DATETIME

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a value-semantic type representing both date and time.
//
//@CLASSES:
//  bdlt::Datetime: combined date and time value (millisecond resolution)
//
//@SEE_ALSO: bdlt_date, bdlt_time, bdlt_datetimetz
//
//@DESCRIPTION: This component implements a value-semantic type,
// 'bdlt::Datetime', that represents the composition of the values of a
// 'bdlt::Date' object and a 'bdlt::Time' object.  In addition to the usual
// value-semantic complement of methods for getting and setting value, the
// class provides methods and operators for making relative adjustments to the
// value.  In particular, relative adjustments to the 'bdlt::Time' part (e.g.,
// the 'addHours' method) can carry over to changes in the 'bdlt::Date' part.
//
///Valid 'bdlt::Datetime' Values and Their Representations
///-------------------------------------------------------
// A combined "date+time" value can be expressed textually as
// "yyyy/mm/dd_hh:mm:ss.sss", where "yyyy/mm/dd" represents the "date" part of
// the value and "hh:mm:ss.sss" represents the "time" part.
//
// The "date" part of a 'bdlt::Datetime' value has a range of validity
// identical to a 'bdlt::Date' object -- i.e., valid dates (according to the
// proleptic Gregorian calendar) having years in the range '[1 .. 9999]'.  The
// set of valid "time" values for a 'bdlt::Datetime' object is, similarly, the
// same as the valid values for 'bdlt::Time', namely
// '[00:00:00.000 .. 23:59:59.999]'.  Note that the supported range of time
// does *not* allow for the injection of leap seconds.
//
// Additionally, the "time" part may have the value "24:00:00.000" (i.e., the
// default constructed value of 'bdlt::Time') if the "date" part has the value
// "0001/01/01" (i.e., the default constructed value of 'bdlt::Date');
// otherwise "24:00:00.000" is disallowed in the "time" part.  The (combined)
// value "0001/001/01_24:00:00.000" is the default constructed value of
// 'bdlt::Datetime'.
//
// As with the 'bdlt::Time' type, the "0001/01/01_24:00:00.000" behaves the
// same as the value "0001/01/01_00:00:00.000" with respect to the various
// manipulators, but results in undefined behavior for all relational
// comparison operators.
//
// Note that the default constructed value must not be used as a key for the
// standard associative containers, as it does not have defined behavior for
// 'operator<'.
//
///Attributes
///----------
// Conceptually, the two main attributes of 'bdlt::Datetime' are the
// constituent 'bdlt::Date' and 'bdlt::Time' values.  These attributes are
// given the special designation "part" in this component to distinguish them
// from the many other attributes (see below) which derive from these two
// parts.
//..
//  Name Type       Default      Range
//  ---- ---------- ------------ -------------------------------
//  date bdlt::Date 0001/01/01   [0001/01/01   ..    9999/12/31]
//  time bdlt::Time 24:00:00.000 [00:00:00.000 ..  23:59:59.999]
//..
// A 'bdlt::Datetime' object can be used in terms of its "date" and "time"
// parts or, if appropriate to an application, the object can be viewed as a
// single, integrated type having the combined individual attributes of
// 'bdlt::Date' and 'bdlt::Time'.  Accessors and manipulators are provided for
// each of these seven attributes:
//..
//  Name         Type  Default Range        Constraint
//  -----------  ----  ------- -----------  -----------------------------
//  year         int    1      [1 .. 9999]  none
//  month        int    1      [1 ..   12]  none
//  day          int    1      [1 ..   31]  must exist for year and month
//  hour         int   24      [0 ..   23]  none, except for '24'*
//  minute       int    0      [0 ..   59]  none
//  second       int    0      [0 ..   59]  none
//  millisecond  int    0      [0 ..  999]  none
//
//  * '24 == hour' only in the special case of "0001/001/01_24:00:00.000".
//..
// There are two additional "date" part attributes to 'bdlt::Datetime':
//..
//  Name      Type                  Default Range        Constraint
//  --------- --------------------- ------- ------------ ----------------------
//  dayOfYear int                   1       [  1 .. 366] 366 only on leap years
//  dayOfWeek bdlt::DayOfWeek::Enum SAT     [SUN .. SAT] tied to calendar day
//..
// where 'dayOfYear' tracks the current value of 'year/month/day' (and *vice*
// *versa*, and 'dayOfWeek' can be accessed but not explicitly set.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Basic Syntax
///- - - - - - - - - - - -
// Values represented by objects of type 'bdlt::Datetime' are used widely in
// practice.  The values of the individual attributes resulting from a
// default-constructed 'bdlt::Datetime' object, 'dt', are
// "0001/01/01_24:00:00.000":
//..
//  bdlt::Datetime dt;          assert( 1 == dt.date().year());
//                              assert( 1 == dt.date().month());
//                              assert( 1 == dt.date().day());
//                              assert(24 == dt.time().hour());
//                              assert( 0 == dt.time().minute());
//                              assert( 0 == dt.time().second());
//                              assert( 0 == dt.time().millisecond());
//..
// We can then set 'dt' to have a specific value, say, 8:43pm on January 6,
// 2013:
//..
//  dt.setDatetime(2013, 1, 6, 20, 43);
//                              assert(2013 == dt.date().year());
//                              assert(   1 == dt.date().month());
//                              assert(   6 == dt.date().day());
//                              assert(  20 == dt.time().hour());
//                              assert(  43 == dt.time().minute());
//                              assert(   0 == dt.time().second());
//                              assert(   0 == dt.time().millisecond());
//..
// Now suppose we add 6 hours and 9 seconds to this value.  There is more than
// one way to do it:
//..
//  bdlt::Datetime dt2(dt);
//  dt2.addHours(6);
//  dt2.addSeconds(9);
//                              assert(2013 == dt2.date().year());
//                              assert(   1 == dt2.date().month());
//                              assert(   7 == dt2.date().day());
//                              assert(   2 == dt2.time().hour());
//                              assert(  43 == dt2.time().minute());
//                              assert(   9 == dt2.time().second());
//                              assert(   0 == dt2.time().millisecond());
//
//  bdlt::Datetime dt3(dt);
//  dt3.addTime(6, 0, 9);
//                              assert(dt2 == dt3);
//..
// Notice that (in both cases) the date changed as a result of adding time;
// however, changing just the date never affects the time:
//..
//  dt3.addDays(10);
//                              assert(2013 == dt3.date().year());
//                              assert(   1 == dt3.date().month());
//                              assert(  17 == dt3.date().day());
//                              assert(   2 == dt3.time().hour());
//                              assert(  43 == dt3.time().minute());
//                              assert(   9 == dt3.time().second());
//                              assert(   0 == dt3.time().millisecond());
//..
// We can also add more than a day's worth of time:
//..
//  dt2.addHours(240);
//                              assert(dt3 == dt2);
//..
// The individual arguments can also be negative:
//..
//  dt2.addTime(-246, 0, -10, 1000);  // -246 h, -10 s, +1000 ms
//                              assert(dt == dt2);
//..
// Finally, we stream the value of 'dt2' to 'stdout':
//..
//  bsl::cout << dt2 << bsl::endl;
//..
// The streaming operator produces the following output on 'stdout':
//..
//  06JAN2013_20:43:00.000
//..
//
///Example 2: Creating a Schedule of Equal Time Intervals
/// - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Calculations involving date and time values are difficult to get correct
// manually; consequently, people tend to schedule events on natural time
// boundaries (e.g., on the hour) even if that is sub-optimal.  Having a class
// such as 'bdlt::Datetime' makes doing date and time calculations trivial.
//
// Suppose one wants to divide into an arbitrary interval such as the time
// between sunset and sunset into an arbitrary number (say 7) of equal
// intervals (perhaps to use as a duty roster for teams making astronomical
// observations).
//
// First, we create objects containing values for the start and end of the time
// interval:
//..
//  bdlt::Datetime  sunset(2014, 6, 26, 20, 31, 23); // New York City
//  bdlt::Datetime sunrise(2014, 6, 27,  5, 26, 51); // New York City
//..
// Then, we calculate the length of each shift in milliseconds (for good
// precision -- we may be synchronizing astronomical instruments).  Note that
// the difference of 'sunrise' and 'sunset' creates a temporary
// 'bdlt::DatetimeInterval' object:
//..
//  const int                numShifts = 7;
//  const bsls::Types::Int64 shiftLengthInMsec
//                                     = (sunrise - sunset).totalMilliseconds()
//                                     / numShifts;
//..
// Now, we calculate (and print to 'stdout') the beginning and end times for
// each shift:
//..
//  for (int i = 0; i <= numShifts; ++i) {
//      bdlt::Datetime startOfShift(sunset);
//      startOfShift.addMilliseconds(shiftLengthInMsec * i);
//      bsl::cout << startOfShift << bsl::endl;
//  }
//..
// Finally, we observe:
//..
//  26JUN2014_20:31:23.000
//  26JUN2014_21:47:52.714
//  26JUN2014_23:04:22.428
//  27JUN2014_00:20:52.142
//  27JUN2014_01:37:21.856
//  27JUN2014_02:53:51.570
//  27JUN2014_04:10:21.284
//  27JUN2014_05:26:50.998
//..
// Notice how our objects (since they manage both "date" and "time of day"
// parts of each point in time) seamlessly handle the transition between the
// two days.

#ifndef INCLUDED_BDLSCM_VERSION
#include <bdlscm_version.h>
#endif

#ifndef INCLUDED_BDLT_DATE
#include <bdlt_date.h>
#endif

#ifndef INCLUDED_BDLT_DATETIMEINTERVAL
#include <bdlt_datetimeinterval.h>
#endif

#ifndef INCLUDED_BDLT_DAYOFWEEK
#include <bdlt_dayofweek.h>
#endif

#ifndef INCLUDED_BDLT_TIME
#include <bdlt_time.h>
#endif

#ifndef INCLUDED_BDLT_TIMEUNITRATIO
#include <bdlt_timeunitratio.h>
#endif

#ifndef INCLUDED_BSLMF_INTEGRALCONSTANT
#include <bslmf_integralconstant.h>
#endif

#ifndef INCLUDED_BSLMF_ISTRIVIALLYCOPYABLE
#include <bslmf_istriviallycopyable.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

#ifndef INCLUDED_BSLS_TYPES
#include <bsls_types.h>
#endif

#ifndef INCLUDED_BSL_IOSFWD
#include <bsl_iosfwd.h>
#endif

namespace BloombergLP {
namespace bdlt {

                        // ==============
                        // class Datetime
                        // ==============

class Datetime {
    // This class implements a complex-constrained value-semantic type
    // representing the composition of values of a 'Date' object and a 'Time'
    // object.  Valid date values for the "date" part of a 'Datetime' object
    // are the same as those described for 'Date' objects; similarly, valid
    // time values for the "time" part of a 'Datetime' object are the same as
    // those described for a 'Time' object with the exception that the default
    // 'Time' value, "24:00:00.000"' is allowed only if the 'Date' part has the
    // default time value, "0001/01/01".  Relational operators are disallowed
    // 'Datetime' objects having the default constructed value (as they are for
    // 'Time' objects having the default constructed value).

    // PRIVATE TYPES
    enum {
        k_MILLISECONDS_PER_DAY    = TimeUnitRatio::k_MS_PER_D_32,
        k_MILLISECONDS_PER_HOUR   = TimeUnitRatio::k_MS_PER_H_32,
        k_MILLISECONDS_PER_MINUTE = TimeUnitRatio::k_MS_PER_M_32,
        k_MILLISECONDS_PER_SECOND = TimeUnitRatio::k_MS_PER_S_32
    };

    // CLASS DATA
    const static Date s_defaultDate;  // "0001/01/01"
    const static Time s_defaultTime;  // "24:00:00.000"
    const static Time s_zeroTime;     // "00:00:00.000"

    // DATA
    Date d_date;  // "date" part of "date+time" value
    Time d_time;  // "time" part of "date+time" value

    // PRIVATE MANIPULATORS
    void setTimeToZeroIfDefault();
        // Set the "time" part of this object to 'Time(0, 0, 0, 0)' if this
        // object has the default constructed value initially, and leave
        // unchanged otherwise.

    // FRIENDS
    friend DatetimeInterval operator-(const Datetime&, const Datetime&);

  public:
    // CLASS METHODS
    static bool isValid(int year,
                        int month,
                        int day,
                        int hour        = 0,
                        int minute      = 0,
                        int second      = 0,
                        int millisecond = 0);
        // Return 'true' if the specified 'year', 'month', and 'day', and the
        // optionally specified 'hour', 'minute', 'second', and 'millisecond'
        // attribute values represent a valid 'Datetime' value, and 'false'
        // otherwise.  Unspecified trailing optional parameters are set to 0.
        // A 'Datetime' value is valid if
        // 'true == bdlt::Date::isValidYearMonthDay(year, month, day)', and
        // 'true== bdlt::Time::isValid(hour, minute, second, millisecond)', and
        // '24 != hour' unless '1 == year && 1 == month && 1 == day'.

    static bool isValid(bdlt::Date date, bdlt::Time time);
        // Return 'true' if the specified 'date' and 'time' represent a valid
        // 'Datetime' value, and 'false' otherwise.  'date' and 'time'
        // represent a valid 'Datetime' value unless
        // 'Time() == time && Date() != date'.

                                  // Aspects

    static int maxSupportedBdexVersion(int versionSelector);
        // Return the maximum valid BDEX format version, as indicated by the
        // specified 'versionSelector', to be passed to the 'bdexStreamOut'
        // method.  Note that it is highly recommended that 'versionSelector'
        // be formatted as "YYYYMMDD", a date representation.  Also note that
        // 'versionSelector' should be a *compile*-time-chosen value that
        // selects a format version supported by both externalizer and
        // unexternalizer.  See the 'bslx' package-level documentation for more
        // information on BDEX streaming of value-semantic types and
        // containers.

    // CREATORS
    Datetime();
        // Create a 'Datetime' object having the "date" and "time" parts of its
        // value set to their respective default-constructed values:
        // "0001/01/01_24:00:00.000" (as defined in the 'bdlt_date' and
        // 'bdlt_time' components).

    Datetime(const Date& date);                                     // IMPLICIT
        // Create a 'Datetime' object having the "date" part of its value set
        // to the specified 'date' and the "time" part of its value set to
        // "00:00:00.000".

    Datetime(const Date& date, const Time& time);
        // Create a 'Datetime' object having the "date" part of its value set
        // to the specified 'date' and the "time" part of its value set to the
        // specified 'time'.  The behavior is undefined if
        // 'Time() == time && Date() != date)' (see 'isValid').

    Datetime(int year,
             int month,
             int day,
             int hour        = 0,
             int minute      = 0,
             int second      = 0,
             int millisecond = 0);
        // Create a 'Datetime' object having the "date" part equal to a 'Date'
        // object having the specified 'year', 'month', and 'day' attribute
        // values, and a "time" part equal to a 'Time' object having the
        // optionally specified 'hour', 'minute', 'second', and 'millisecond'
        // attribute values.  Unspecified trailing optional parameters are set
        // to 0.  The behavior is undefined unless the seven attributes
        // (collectively) comprise a valid 'Datetime' value (see 'isValid').

    Datetime(const Datetime& original);
        // Create a 'Datetime' object having the value of the specified
        // 'original' object.

//! ~Datetime() = default;
        // Destroy this 'Datetime' object.

    // MANIPULATORS
    Datetime& operator=(const Datetime& rhs);
        // Assign to this object the value of the specified 'rhs' object, and
        // return a reference providing modifiable access to this object.

    Datetime& operator+=(const DatetimeInterval& rhs);
        // Add to this object the value of the specified 'rhs' object, and
        // return a reference providing modifiable access to this object.  If
        // '24 == hour()', set the 'hour' attribute to 0 before performing the
        // addition.  The behavior is undefined unless the resulting value is
        // valid for 'Datetime' (see 'isValid').

    Datetime& operator-=(const DatetimeInterval& rhs);
        // Subtract from this object the value of the specified 'rhs' object,
        // and return a reference to this modifiable object.  If
        // '24 == hour()', set the 'hour' attribute to 0 before performing the
        // subtraction.  The behavior is undefined unless the resulting value
        // is valid for 'Datetime' (see 'isValid').

    void setDatetime(int year,
                     int month,
                     int day,
                     int hour        = 0,
                     int minute      = 0,
                     int second      = 0,
                     int millisecond = 0);
        // Set the "date" part of this object equal to a 'Date' object having
        // the specified 'year', 'month', and 'day' attribute values and set
        // the "time" part of this object equal to a 'Time' object having the
        // optionally specified 'hour', 'minute', 'second', and 'millisecond'
        // attribute values.  Unspecified trailing optional parameters are set
        // to 0.  The behavior is undefined unless the seven attribute values
        // (collectively) comprise a valid 'Datetime' value (see 'isValid').

    int setDatetimeIfValid(int year,
                           int month,
                           int day,
                           int hour        = 0,
                           int minute      = 0,
                           int second      = 0,
                           int millisecond = 0);
        // Set the "date" part of this object equal to a 'Date' object having
        // the specified 'year', 'month', and 'day' attribute values, and set
        // the "time" part of this object equal to a 'Time' object having the
        // optionally specified 'hour', 'minute', 'second', and 'millisecond'
        // attribute values, *if* the seven attribute values (collectively)
        // comprise a valid 'Datetime' value (see 'isValid').  Unspecified
        // trailing optional parameters are set to 0.  Return 0 on success, and
        // a non-zero value, with no effect, otherwise.

    void setDate(const Date& date);
        // Set the "date" part of this object's value to the specified 'date'.
        // If '24 == hour()', set the 'hour' attribute to 0.  Note that this
        // method does not affect the "time" part of this object's value unless
        // '24 == hour()' initially.

    void setYearDay(int year, int dayOfYear);
        // Set the "date" part of this object's value equal to a 'Date' object
        // having the the specified 'year' and 'dayOfYear' attribute values.
        // If '24 == hour()', set the 'hour' attribute to 0.  The behavior is
        // undefined unless 'year' and 'dayOfYear' comprise a valid 'Date'
        // value (i.e., 'true == Date::isValidYearDay(year, dayOfYear)'.  Note
        // that this method does not affect the "time" part of this object's
        // value unless '24 == hour()' initially.

    void setYearMonthDay(int year, int month, int day);
        // Set the "date" part of this object's value equal to a 'Date' object
        // having the specified 'year', 'month', and 'day' attribute values.
        // If '24 == hour()', set the 'hour' attribute to 0.  The behavior is
        // undefined unless 'year', 'month', and 'day' comprise a valid 'Date'
        // value (i.e., 'true == Date::isValidYearMonthDay(year, month, day)').
        // Note that this method does not affect the "time" part of this
        // object's value unless '24 == hour()' initially.

    void setTime(const Time& time);
        // Set the "time" part of this object's value to the specified 'time'.
        // The behavior is undefined if
        // 'Time() == time() && Date() != date())'.  Note that this method does
        // not affect the "date" part of this object's value.

    void setTime(int hour,
                 int minute      = 0,
                 int second      = 0,
                 int millisecond = 0);
        // Set the "time" part of this object's value a 'Time' object having
        // the specified 'hour' attribute values and the optionally specified
        // 'minute', 'second', and 'millisecond' attribute values.  Unspecified
        // trailing optional parameters are set to 0.  The behavior is
        // undefined unless 'hour', 'minute', 'second', and 'millisecond'
        // comprise a valid 'Time' value (i.e.,
        // 'true == Time::isValid(hour, minute, second, millisecond)', or
        // undefined if '24 == hour && Date() != date())'.  Note that this
        // method does not affect the "date" part of this object's value.

    void setHour(int hour);
        // Set the 'hour' attribute of the "time" part of this object's value
        // to the specified 'hour'.  If '24 == hour()', set the all other
        // "time" attributes to 0.  The behavior is undefined unless
        // '0 <= hour <= 24', or undefined if
        // '24 == hour && Date() != date())'.  Note that this method does not
        // affect the "date" part of this object's value.

    void setMinute(int minute);
        // Set the 'minute' attribute of the "time" part of this object's value
        // to the specified 'minute'.  If '24 == hour()', set the 'hour'
        // attribute to 0.  The behavior is undefined unless
        // '0 <= minute <= 59'.  Note that this method does not affect the
        // "date" part of this object's value.

    void setSecond(int second);
        // Set the 'second' attribute of the "time" part of this object's value
        // to the specified 'second'.  If '24 == hour()', set the 'hour'
        // attribute to 0.  The behavior is undefined unless
        // '0 <= second <= 59'.  Note that this method does not affect the
        // "date" part of this object's value.

    void setMillisecond(int millisecond);
        // Set the 'millisecond' attribute of the "time" part of this object's
        // value to the specified 'millisecond'.  If '24 == hour()', set the
        // 'hour' attribute to 0.  The behavior is undefined unless
        // '0 <= millisecond <= 999'.  Note that this method does not affect
        // the "date" part of this object's value.

    void addDays(int days);
        // Add the specified number of 'days' to the value of this object.
        // 'days' may be positive, 0, or negative.  If '24 == hour()', set the
        // 'hour' attribute to 0 before performing the addition.  The behavior
        // is undefined unless the resulting value would be in the valid range
        // for a 'Datetime' object.  Note that this method does not affect the
        // "time" part of this object's value unless '24 == hour' initially.

    void addTime(bsls::Types::Int64 hours,
                 bsls::Types::Int64 minutes      = 0,
                 bsls::Types::Int64 seconds      = 0,
                 bsls::Types::Int64 milliseconds = 0);
        // Add the specified number of 'hours', and the optionally specified
        // number of 'minutes', 'seconds', and 'milliseconds', to the value of
        // this object, adjusting the "date" part of this object's value
        // accordingly.  Unspecified trailing optional parameters are set to 0.
        // If '24 == hour()', the 'hour' attribute is set to 0 before
        // performing the addition.  The behavior is undefined unless the
        // arguments represent a valid 'DatetimeInterval' value and the
        // resulting value is in the valid range for a 'Datetime' object.  Note
        // that each argument may independently be positive, negative, or 0.

    void addHours(bsls::Types::Int64 hours);
        // Add the specified number of 'hours' to the value of this object,
        // adjusting the "date" part of the value accordingly.  If
        // '24 == hour()', the 'hour' attribute is set to 0 before performing
        // the addition.  The behavior is undefined unless the resulting value
        // would be in the valid range for a 'Datetime' object.  Note that
        // 'hours' can be positive, negative, or 0.

    void addMinutes(bsls::Types::Int64 minutes);
        // Add the specified number of 'minutes' to the value of this object,
        // adjusting the "date" part of the value accordingly.  'minutes' may
        // be positive, 0, or negative.  If '24 == hour()', the 'hour'
        // attribute is set to 0 before performing the addition.  The behavior
        // is undefined unless the resulting value would be in the valid range
        // for a 'Datetime' object.  Note that 'minutes' can be positive,
        // negative, or 0.

    void addSeconds(bsls::Types::Int64 seconds);
        // Add the specified number of 'seconds' to the value of this object,
        // adjusting the "date" part of the value accordingly.  'seconds' may
        // be positive, 0, or negative.  If '24 == hour()', the 'hour'
        // attribute is set to 0 before performing the addition.  The behavior
        // is undefined unless the resulting value would be in the valid range
        // for a 'Datetime' object.  Note that 'seconds' can be positive,
        // negative, or 0.

    void addMilliseconds(bsls::Types::Int64 milliseconds);
        // Add the specified number of 'milliseconds' to the value of this
        // object, adjusting the "date" part of the value accordingly.
        // 'milliseconds' may be positive, 0, or negative.  If '24 == hour()',
        // the 'hour' attribute is set to 0 before performing the addition.
        // The behavior undefined unless the resulting value would be in the
        // valid range for a 'Datetime' object.  Note that 'milliseconds' can
        // be positive, negative, or 0.

                                  // Aspects

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format, and return a
        // reference to 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'version' is not supported, this object
        // is unaltered and 'stream' is invalidated, but otherwise unmodified.
        // If 'version' is supported but 'stream' becomes invalid during this
        // operation, this object has an undefined, but valid, state.  Note
        // that no version is read from 'stream'.  See the 'bslx' package-level
        // documentation for more information on BDEX streaming of
        // value-semantic types and containers.

    // ACCESSORS
    Date date() const;
        // Return the value of the "date" part of this object.

    Time time() const;
        // Return the value of the "time" part of this object.

    int year() const;
        // Return the value of the 'year' attribute of this object.

    int month() const;
        // Return the value of the 'month' attribute of this object.

    int day() const;
        // Return the value of the 'day' (of the month) attribute of this
        // object.

    int dayOfYear() const;
        // Return the value of the 'dayOfYear' attribute of this object.

    DayOfWeek::Enum dayOfWeek() const;
        // Return the value of the 'dayOfWeek' attribute associated with the
        // 'day' (of the month) attribute of this object.

    int hour() const;
        // Return the value of the 'hour' attribute of this object.

    int minute() const;
        // Return the value of the 'minute' attribute of this object.

    int second() const;
        // Return the value of the 'second' attribute of this object.

    int millisecond() const;
        // Return the value of the 'millisecond' attribute of this object.

    int printToBuffer(char *result, int numBytes) const;
        // Efficiently write to the specified 'result' buffer no more than
        // 'numBytes' of a representation of the value of this object.  Return
        // the number of characters (not including the null character) that
        // would have been written if the limit due to 'numBytes' were not
        // imposed.  'result' is null-terminated unless 'numBytes' is 0.  The
        // behavior is undefined unless '0 <= numBytes' and 'result' refers to
        // at least 'numBytes' contiguous bytes.  Note that the return value is
        // greater than or equal to 'numBytes' if the output representation was
        // truncated to avoid 'result' overrun.

                                  // Aspects

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object, using the specified 'version'
        // format, to the specified output 'stream', and return a reference to
        // 'stream'.  If 'stream' is initially invalid, this operation has no
        // effect.  If 'version' is not supported, 'stream' is invalidated, but
        // otherwise unmodified.  Note that 'version' is not written to
        // 'stream'.  See the 'bslx' package-level documentation for more
        // information on BDEX streaming of value-semantic types and
        // containers.

    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Write the value of this object to the specified output 'stream' in a
        // human-readable format, and return a reference to 'stream'.
        // Optionally specify an initial indentation 'level', whose absolute
        // value is incremented recursively for nested objects.  If 'level' is
        // specified, optionally specify 'spacesPerLevel', whose absolute value
        // indicates the number of spaces per indentation level for this and
        // all of its nested objects.  If 'level' is negative, suppress
        // indentation of the first line.  If 'spacesPerLevel' is negative,
        // format the entire output on one line, suppressing all but the
        // initial indentation (as governed by 'level').  If 'stream' is not
        // valid on entry, this operation has no effect.  Note that this
        // human-readable format is not fully specified, and can change without
        // notice.


};

// FREE OPERATORS
Datetime operator+(const Datetime&         lhs,
                   const DatetimeInterval& rhs);
    // Return a 'Datetime' value that is the sum of the specified 'lhs' (a
    // 'Datetime') and the specified 'rhs' (a 'DatetimeInterval').  If
    // '24 == lhs.hour', the result is the same as if the 'hour' attribute is
    // 0.  The behavior is undefined unless the resulting value is a valid
    // 'Datetime' value.

Datetime operator+(const DatetimeInterval& lhs,
                   const Datetime&         rhs);
    // Return a 'Datetime' value that is the sum of the specified 'lhs' (a
    // 'DatetimeInterval') and the specified 'rhs' (a 'Datetime').  If the
    // 'hour' attribute of either operand is 24, the result is the same as if
    // that 'hour' attribute is 0.  The behavior is undefined unless the
    // resulting value is a valid 'Datetime' value.

Datetime operator-(const Datetime&         lhs,
                   const DatetimeInterval& rhs);
    // Return a 'Datetime' value that is the difference between the specified
    // 'lhs' (a 'Datetime') and the specified 'rhs' (a 'DatetimeInterval').  If
    // '24 == lhs.hour', the result is the same as if the 'hour' attribute is
    // 0.  The behavior is undefined unless the resulting value is a valid
    // 'Datetime' value.

DatetimeInterval operator-(const Datetime& lhs,
                           const Datetime& rhs);
    // Return a 'Datetime' value that is the difference between the specified
    // 'lhs' (a 'Datetime') and the specified 'rhs' (a 'Datetime').  If the
    // 'hour' attribute of either operand is 24, the result is the same as if
    // that 'hour' attribute is 0.  The behavior is undefined unless the
    // resulting value is a valid 'Datetime' value.

bool operator==(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
    // value, and 'false' otherwise.  Two 'Datetime' objects have the same
    // value if they have the same values for their "date" and "time" parts
    // respectively (i.e.,
    // 'lhs.date() == rhs.date() && lhs.time() == rhs.time()').

bool operator!=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' 'Datetime' objects do not
    // have the same value, and 'false' otherwise.  Two 'Datetime' objects do
    // not have the same value if they do not have the same values for either
    // of their "date" or "time" parts respectively (i.e.,
    // 'lhs.date() != rhs.date() || lhs.time() != rhs.time()').

bool operator<(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' object has a value less than the
    // value of the specified 'rhs' object, and 'false' otherwise.  A
    // 'Datetime' object 'a' has a value less than the value of another
    // 'Datetime' object 'b' if 'a.date() < b.date()', or if the
    // 'a.date() == b.date() &&  a.time() < b.time()'.  The behavior is
    // undefined if '24 == rhs.hour()' or '24 == lhs.hour()'.

bool operator<=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' object has a value less than or
    // equal to the value of the specified 'rhs' object, and 'false' otherwise.
    // The behavior is undefined if '24 == rhs.hour()' or '24 == lhs.hour()'.

bool operator>(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' object has a value greater than the
    // value of the specified 'rhs' object, and 'false' otherwise.  A
    // 'Datetime' object 'a' has a value greater than the value of another
    // 'Datetime' object 'b' if 'a.date() > b.date()', or if the
    // 'a.date() == b.date() &&  a.time() > b.time()'.  The behavior is
    // undefined if '24 == rhs.hour()' or '24 == lhs.hour()'.

bool operator>=(const Datetime& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' object has a value greater than or
    // equal to the value of the specified 'rhs' object, and 'false' otherwise.
    // The behavior is undefined if '24 == rhs.hour()' or '24 == lhs.hour()'.

bsl::ostream& operator<<(bsl::ostream& stream, const Datetime& object);
    // Write the value of the specified 'object' object to the specified output
    // 'stream' in a single-line format, and return a reference to 'stream'.
    // If 'stream' is not valid on entry, this operation has no effect.  Note
    // that this human-readable format is not fully specified, can change
    // without notice, and is logically equivalent to:
    //..
    //  print(stream, 0, -1);
    //..

// ============================================================================
//                            INLINE DEFINITIONS
// ============================================================================

                               // --------------
                               // class Datetime
                               // --------------

// PRIVATE MANIPULATORS
inline
void Datetime::setTimeToZeroIfDefault()
{
    if (s_defaultTime == d_time && s_defaultDate == d_date) {
        d_time = s_zeroTime;
    }
}

// CLASS METHODS
inline
bool Datetime::isValid(int year,
                       int month,
                       int day,
                       int hour,
                       int minute,
                       int second,
                       int millisecond)
{
    const bool partsValid = Date::isValidYearMonthDay(year, month, day)
                        &&  Time::isValid(hour, minute, second, millisecond);

    if (!partsValid) {
        return false;                                                 // RETURN
    }

    if (24 == hour && !(1 == year && 1 == month && 1 == day)) {
        return false;                                                 // RETURN
    }

    return true;
}

inline
bool Datetime::isValid(bdlt::Date date, bdlt::Time time)
{
    if (bdlt::Time() == time && bdlt::Date() != date) {
        return false;                                                 // RETURN
    }

    return true;
}

                                  // Aspects

inline
int Datetime::maxSupportedBdexVersion(int /* versionSelector */)
{
    return 1;
}

// CREATORS
inline
Datetime::Datetime()
{
}

inline
Datetime::Datetime(const Date& date)
: d_date(date)
, d_time(0, 0, 0, 0)
{
}

inline
Datetime::Datetime(const Date& date, const Time& time)
: d_date(date)
, d_time(time)
{
    // Contract narrowed for open-source release.

    BSLS_ASSERT_SAFE(Datetime::isValid(date, time));

}

inline
Datetime::Datetime(int year,
                   int month,
                   int day,
                   int hour,
                   int minute,
                   int second,
                   int millisecond)
: d_date(year, month, day)
, d_time(hour, minute, second, millisecond)
{
    // Contract narrowed for open-source release.

    BSLS_ASSERT_SAFE(Datetime::isValid(year,
                                       month,
                                       day,
                                       hour,
                                       minute,
                                       second,
                                       millisecond));
}

inline
Datetime::Datetime(const Datetime& original)
: d_date(original.d_date)
, d_time(original.d_time)
{
}

// MANIPULATORS
inline
Datetime& Datetime::operator=(const Datetime& rhs)
{
    d_date = rhs.d_date;
    d_time = rhs.d_time;
    return *this;
}

inline
Datetime& Datetime::operator+=(const DatetimeInterval& rhs)
{
    BSLS_ASSERT_SAFE(rhs <= Datetime(9999, 12, 31, 23, 59, 59, 999) - *this);
    BSLS_ASSERT_SAFE(rhs >= Datetime(0001,  1,  1,  0,  0,  0,   0) - *this);

    d_date += d_time.addInterval(rhs);
    return *this;
}

inline
Datetime& Datetime::operator-=(const DatetimeInterval& rhs)
{
    BSLS_ASSERT_SAFE(-rhs <= Datetime(9999, 12, 31, 23, 59, 59, 999) - *this);
    BSLS_ASSERT_SAFE(-rhs >= Datetime(0001,  1,  1,  0,  0,  0,   0) - *this);

    d_date += d_time.addInterval(-rhs);
    return *this;
}

inline
void Datetime::setDatetime(int year,
                           int month,
                           int day,
                           int hour,
                           int minute,
                           int second,
                           int millisecond)
{
    // Contract narrowed for open-source release.

    BSLS_ASSERT_SAFE(Datetime::isValid(year,
                                       month,
                                       day,
                                       hour,
                                       minute,
                                       second,
                                       millisecond));

    d_date.setYearMonthDay(year, month, day);
    d_time.setTime(hour, minute, second, millisecond);
}

inline
int Datetime::setDatetimeIfValid(int year,
                                 int month,
                                 int day,
                                 int hour,
                                 int minute,
                                 int second,
                                 int millisecond)
{
    enum { k_SUCCESS = 0, k_FAILURE = -1 };

    if (Datetime::isValid(year,
                          month,
                          day,
                          hour,
                          minute,
                          second,
                          millisecond)) {
        setDatetime(year, month, day, hour, minute, second, millisecond);
        return k_SUCCESS;                                             // RETURN
    }

    return k_FAILURE;
}

inline
void Datetime::setDate(const Date& date)
{
    setTimeToZeroIfDefault();

    d_date = date;
}

inline
void Datetime::setYearDay(int year, int dayOfYear)
{
    BSLS_ASSERT_SAFE(Date::isValidYearDay(year, dayOfYear));

    setTimeToZeroIfDefault();

    d_date.setYearDay(year, dayOfYear);
}

inline
void Datetime::setYearMonthDay(int year, int month, int day)
{
    BSLS_ASSERT_SAFE(Date::isValidYearMonthDay(year, month, day));

    setTimeToZeroIfDefault();

    d_date.setYearMonthDay(year, month, day);
}

inline
void Datetime::setTime(const Time& time)
{
    // Contract narrowed for open-source release.

    BSLS_ASSERT_SAFE(isValid(d_date, time));

    d_time = time;
}

inline
void Datetime::setTime(int hour, int minute, int second, int millisecond)
{
    BSLS_ASSERT_SAFE(Time::isValid(hour, minute, second, millisecond));
    // Contract narrowed for open-source release.

    BSLS_ASSERT_SAFE(isValid(d_date, Time(hour, minute, second, millisecond)));

    d_time.setTime(hour, minute, second, millisecond);
}

inline
void Datetime::setHour(int hour)
{
    BSLS_ASSERT_SAFE(0 <= hour);  BSLS_ASSERT_SAFE(hour <= 24);

    // Contract narrowed for open-source release.

    BSLS_ASSERT_SAFE(24 != hour || Date() == d_date);

    d_time.setHour(hour);
}

inline
void Datetime::setMinute(int minute)
{
    BSLS_ASSERT_SAFE(0 <= minute);  BSLS_ASSERT_SAFE(minute <= 59);

    d_time.setMinute(minute);
}

inline
void Datetime::setSecond(int second)
{
    BSLS_ASSERT_SAFE(0 <= second);  BSLS_ASSERT_SAFE(second <= 59);

    d_time.setSecond(second);
}

inline
void Datetime::setMillisecond(int millisecond)
{
    BSLS_ASSERT_SAFE(0 <= millisecond);  BSLS_ASSERT_SAFE(millisecond <= 999);

    d_time.setMillisecond(millisecond);
}

inline
void Datetime::addDays(int days)
{
    BSLS_ASSERT_SAFE(0 == Date(d_date).addDaysIfValid(days));

    setTimeToZeroIfDefault();

    d_date += days;
}

inline
void Datetime::addTime(bsls::Types::Int64 hours,
                       bsls::Types::Int64 minutes,
                       bsls::Types::Int64 seconds,
                       bsls::Types::Int64 milliseconds)
{
    const DatetimeInterval delta(0,
                                 hours,
                                 minutes,
                                 seconds,
                                 milliseconds);

    BSLS_ASSERT_SAFE(delta <= Datetime(9999, 12, 31, 23, 59, 59, 999) - *this);
    BSLS_ASSERT_SAFE(delta >= Datetime(0001,  1,  1,  0,  0,  0,   0) - *this);

    d_date += d_time.addInterval(delta);
}


inline
void Datetime::addHours(bsls::Types::Int64 hours)
{
    const bsls::Types::Int64 totalMsec = hours * k_MILLISECONDS_PER_HOUR;

    const int normMsec  = static_cast<int>(totalMsec % k_MILLISECONDS_PER_DAY);
    const int wholeDays = static_cast<int>(totalMsec / k_MILLISECONDS_PER_DAY);

    const int numDays   = wholeDays + d_time.addMilliseconds(normMsec);

    BSLS_ASSERT_SAFE(0 == Date(d_date).addDaysIfValid(numDays));

    d_date += numDays;
}

inline
void Datetime::addMinutes(bsls::Types::Int64 minutes)
{
    const bsls::Types::Int64 totalMsec = minutes * k_MILLISECONDS_PER_MINUTE;

    const int normMsec  = static_cast<int>(totalMsec % k_MILLISECONDS_PER_DAY);
    const int wholeDays = static_cast<int>(totalMsec / k_MILLISECONDS_PER_DAY);

    const int numDays   = wholeDays + d_time.addMilliseconds(normMsec);

    BSLS_ASSERT_SAFE(0 == Date(d_date).addDaysIfValid(numDays));

    d_date += numDays;
}

inline
void Datetime::addSeconds(bsls::Types::Int64 seconds)
{
    const bsls::Types::Int64 totalMsec = seconds * k_MILLISECONDS_PER_SECOND;

    const int normMsec  = static_cast<int>(totalMsec % k_MILLISECONDS_PER_DAY);
    const int wholeDays = static_cast<int>(totalMsec / k_MILLISECONDS_PER_DAY);

    const int numDays   = wholeDays + d_time.addMilliseconds(normMsec);

    BSLS_ASSERT_SAFE(0 == Date(d_date).addDaysIfValid(numDays));

    d_date += numDays;
}

inline
void Datetime::addMilliseconds(bsls::Types::Int64 milliseconds)
{
    const int normMsec  = static_cast<int>(
                                        milliseconds % k_MILLISECONDS_PER_DAY);
    const int wholeDays = static_cast<int>(
                                        milliseconds / k_MILLISECONDS_PER_DAY);

    const int numDays   = wholeDays + d_time.addMilliseconds(normMsec);

    BSLS_ASSERT_SAFE(0 == Date(d_date).addDaysIfValid(numDays));

    d_date += numDays;
}

                                  // Aspects

template <class STREAM>
STREAM& Datetime::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) { // switch on the schema version
          case 1: {
            Date dateTmp;
            Time timeTmp;
            dateTmp.bdexStreamIn(stream, 1);
            timeTmp.bdexStreamIn(stream, 1);

            if (stream) {
                d_date = dateTmp;
                d_time = timeTmp;
            }
            else {
                stream.invalidate();
            }
          } break;
          default: {
            stream.invalidate();  // unrecognized version number
          }
        }
    }
    return stream;
}

// ACCESSORS
inline
Date Datetime::date() const
{
    return d_date;
}

inline
Time Datetime::time() const
{
    return d_time;
}

inline
int Datetime::year() const
{
    return d_date.year();
}

inline
int Datetime::month() const
{
    return d_date.month();
}

inline
int Datetime::day() const
{
    return d_date.day();
}

inline
int Datetime::dayOfYear() const
{
    return d_date.dayOfYear();
}

inline
DayOfWeek::Enum Datetime::dayOfWeek() const
{
    return d_date.dayOfWeek();
}

inline
int Datetime::hour() const
{
    return d_time.hour();
}

inline
int Datetime::minute() const
{
    return d_time.minute();
}

inline
int Datetime::second() const
{
    return d_time.second();
}

inline
int Datetime::millisecond() const
{
    return d_time.millisecond();
}

                                  // Aspects

template <class STREAM>
STREAM& Datetime::bdexStreamOut(STREAM& stream, int version) const
{
    if (stream) {
        switch (version) { // switch on the schema version
          case 1: {
            d_date.bdexStreamOut(stream, 1);
            d_time.bdexStreamOut(stream, 1);
          } break;
          default: {
            stream.invalidate();  // unrecognized version number
          }
        }
    }
    return stream;
}


}  // close package namespace

// FREE OPERATORS
inline
bdlt::Datetime bdlt::operator+(const Datetime&         lhs,
                               const DatetimeInterval& rhs)
{
    Datetime result(lhs);
    return result += rhs;
}

inline
bdlt::Datetime bdlt::operator+(const DatetimeInterval& lhs,
                               const Datetime&         rhs)
{
    Datetime result(rhs);
    return result += lhs;
}

inline
bdlt::Datetime bdlt::operator-(const Datetime&         lhs,
                               const DatetimeInterval& rhs)
{
    Datetime result(lhs);
    return result -= rhs;
}

inline
bdlt::DatetimeInterval bdlt::operator-(const Datetime& lhs,
                                       const Datetime& rhs)
{
    DatetimeInterval timeInterval1(lhs.d_date - rhs.d_date, 0, 0);
    DatetimeInterval timeInterval2 = lhs.d_time - rhs.d_time;
    timeInterval1 += timeInterval2;

    return timeInterval1;
}

inline
bool bdlt::operator==(const Datetime& lhs, const Datetime& rhs)
{
    return lhs.date() == rhs.date() && lhs.time() == rhs.time();
}

inline
bool bdlt::operator!=(const Datetime& lhs, const Datetime& rhs)
{
    return lhs.date() != rhs.date() || lhs.time() != rhs.time();
}

inline
bool bdlt::operator<(const Datetime& lhs, const Datetime& rhs)
{
    BSLS_ASSERT_SAFE(Time() != lhs.time());
    BSLS_ASSERT_SAFE(Time() != rhs.time());

    return lhs.date() <  rhs.date()
       || (lhs.date() == rhs.date() && lhs.time() < rhs.time());
}

inline
bool bdlt::operator<=(const Datetime& lhs, const Datetime& rhs)
{
    BSLS_ASSERT_SAFE(Time() != lhs.time());
    BSLS_ASSERT_SAFE(Time() != rhs.time());

    return lhs.date() <  rhs.date()
       || (lhs.date() == rhs.date() && lhs.time() <= rhs.time());
}

inline
bool bdlt::operator>(const Datetime& lhs, const Datetime& rhs)
{
    BSLS_ASSERT_SAFE(Time() != lhs.time());
    BSLS_ASSERT_SAFE(Time() != rhs.time());

    return lhs.date() >  rhs.date()
       || (lhs.date() == rhs.date() && lhs.time() > rhs.time());
}

inline
bool bdlt::operator>=(const Datetime& lhs, const Datetime& rhs)
{
    BSLS_ASSERT_SAFE(Time() != lhs.time());
    BSLS_ASSERT_SAFE(Time() != rhs.time());

    return lhs.date() >  rhs.date()
       || (lhs.date() == rhs.date() && lhs.time() >= rhs.time());
}

}  // close enterprise namespace

namespace bsl {

// TRAITS
template <>
struct is_trivially_copyable<BloombergLP::bdlt::Datetime> : bsl::true_type {
    // This template specialization for 'is_trivially_copyable' indicates that
    // 'bdlt::Datetime' is a trivially copyable type.
};

}  // close namespace bsl

#endif

// ----------------------------------------------------------------------------
// Copyright 2014 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
