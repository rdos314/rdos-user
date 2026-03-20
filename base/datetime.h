/*#######################################################################
# RDOS operating system
# Copyright (C) 1988-2025, Leif Ekblad
#
# MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# The author of this program may be contacted at leif@rdos.net
#
# datetime.h
# Date & time class
#
########################################################################*/

#ifndef _DATETIME_H
#define _DATETIME_H

#ifndef __RDOS__
#include <time.h>
#endif

/**
 * Converts a given date and time into a binary representation of the number of
 * seconds since the Unix epoch (January 1, 1970, 00:00:00 UTC).
 *
 * @param year The year of the date.
 * @param month The month of the date (1-12).
 * @param day The day of the month (1-31).
 * @param hour The hour of the day (0-23).
 * @param min The minute of the hour (0-59).
 * @param sec The second of the minute (0-59).
 * @return The binary representation of the given date and time as the number
 *         of seconds since the Unix epoch.
 */
long TimeToBinary(int year, int month, int day, int hour, int min, int sec);

/**
 * Converts a binary representation of the number of seconds since the Unix epoch
 * (January 1, 1970, 00:00:00 UTC) into a human-readable date and time.
 *
 * @param time The binary representation of the number of seconds since the Unix epoch.
 * @param year Pointer to an integer where the year will be stored.
 * @param month Pointer to an integer where the month (1-12) will be stored.
 * @param day Pointer to an integer where the day of the month (1-31) will be stored.
 * @param hour Pointer to an integer where the hour of the day (0-23) will be stored.
 * @param min Pointer to an integer where the minutes (0-59) will be stored.
 * @param sec Pointer to an integer where the seconds (0-59) will be stored.
 */
void BinaryToTime(long time, int *year, int *month, int *day, int *hour, int *min, int *sec);

/**
 * Sets the system time to the specified binary representation of the time
 * in seconds since the Unix epoch (January 1, 1970, 00:00:00 UTC).
 *
 * On non-RDOS systems, sets the local time offset based on the difference
 * between the given time and the current system time. On RDOS systems, converts
 * the binary time to its constituent date and time components and updates the
 * system time using RDOS-specific functions.
 *
 * @param time The binary representation of the time, in seconds since the
 *        Unix epoch.
 */
void SetTime(long time);

/**
 * Retrieves the current time as the number of seconds since the Unix epoch
 * (January 1, 1970, 00:00:00 UTC). The method adapts to the specific underlying
 * operating system to obtain the time.
 *
 * @return The current time represented as the number of seconds since the Unix epoch.
 */
long GetTime();

/**
 * Retrieves the current system time as the number of seconds since the Unix epoch
 * (January 1, 1970, 00:00:00 UTC).
 *
 * Depending on the platform, the implementation either:
 * - For RDOS: Decodes the system time from hardware ticks and converts it into
 *   a binary representation of seconds via the TimeToBinary function.
 * - For other platforms: Uses the `clock_gettime` function to retrieve the current time.
 *
 * @return The current system time represented as the number of seconds since
 *         the Unix epoch.
 */
long GetSystemTime();

/**
 * Pauses the execution of the current thread for a specified duration in milliseconds.
 *
 * @param ms The amount of time, in milliseconds, for which the thread should be paused.
 */
void WaitMilli(int ms);

/**
 * Represents a date and time with year, month, day, hour, minute, and second components.
 *
 * This class provides functionality for manipulating, comparing, and obtaining
 * various representations of a specific date and time. Operations can include
 * calculating differences, formatting, or adjusting values.
 *
 * The class ensures that the date and time values remain valid within their
 * expected ranges during manipulation. Behaviors with respect to time zones
 * or daylight saving may depend on implementation details.
 */
class TDateTime
{
    /**
     * Constructs a TDateTime object and initializes the internal date and time
     * information based on the current local time settings of the system.
     *
     * For systems running the RDOS operating system, the constructor retrieves
     * time using specific RDOS API calls. For other operating systems, it uses
     * standard system time structures. After fetching the raw time data, the
     * constructor converts it into a structured date and time representation.
     *
     * @return A new instance of the TDateTime class initialized to the current
     *         local date and time.
     */
public:
    TDateTime();

    /**
     * Constructs a TDateTime object by copying the values from another TDateTime instance.
     *
     * @param source The TDateTime instance to copy from.
     * @return A new TDateTime object with the same date and time values as the source.
     */
    TDateTime(const TDateTime &Source);

    /**
     * Constructs a TDateTime object representing a specific point in time
     * defined by the given "most significant bits" (MSB) and "least significant bits" (LSB).
     *
     * On RDOS systems, the parameters are stored directly. On other systems,
     * they are converted into a timespec structure representing the corresponding
     * time in seconds and nanoseconds since the Unix epoch.
     *
     * @param Msb The most significant 32 bits of the time representation.
     * @param Lsb The least significant 32 bits of the time representation.
     * @return An instance of the TDateTime object initialized with the provided time.
     */
    TDateTime(unsigned long Msb, unsigned long Lsb);

    /**
     * Constructs a TDateTime object from a raw 64-bit value representing either
     * an RDOS or Linux timestamp.
     *
     * @param Raw A 64-bit integer value where the most significant bits (MSB)
     *            and least significant bits (LSB) encode the date and time.
     *            On RDOS, these values directly influence the internal fields.
     *            On Linux, the value is converted into a `timespec` structure.
     * @return A TDateTime object initialized with the given raw timestamp.
     */
    TDateTime(unsigned long long Raw);

    /**
     * Constructs a TDateTime object from a long double value representing
     * a date and time in fractional format, where the integer part corresponds
     * to the most significant part of the time representation and the fractional
     * part corresponds to the least significant part.
     *
     * @param real A long double value representing the date and time. The value
     *             should consist of an integer part and a fractional part, where
     *             the integer part represents the most significant part of the
     *             time representation, and the fractional part represents the
     *             least significant part.
     * @return A TDateTime object initialized with the given date and time
     *         representation.
     */
    TDateTime(long double real);

    /**
     * Initializes a TDateTime object with the specified year, month, and day.
     * The time is set to 00:00:00.000000 by default.
     *
     * @param Year The year to initialize the TDateTime object with.
     * @param Month The month to initialize the TDateTime object with (1-12).
     * @param Day The day of the month to initialize the TDateTime object with (1-31).
     * @return A constructed and initialized TDateTime object.
     */
    TDateTime(int Year, int Month, int Day);

    /**
     * Constructs a new TDateTime object with the specified date and time components.
     *
     * @param Year The year of the date.
     * @param Month The month of the date (1-12).
     * @param Day The day of the month (1-31).
     * @param Hour The hour of the day (0-23).
     * @param Min The minute of the hour (0-59).
     * @param Sec The second of the minute (0-59).
     * @return A new instance of the TDateTime class initialized with the given date and time.
     */
    TDateTime(int Year, int Month, int Day, int Hour, int Min, int Sec);

    /**
     * Constructs a TDateTime instance with the specified date, time, and fractional
     * second components.
     *
     * @param Year The year of the date.
     * @param Month The month of the date (1-12).
     * @param Day The day of the month (1-31).
     * @param Hour The hour of the day (0-23).
     * @param Min The minute of the hour (0-59).
     * @param Sec The second of the minute (0-59).
     * @param Milli The millisecond component (0-999).
     * @param Micro The microsecond component (0-999).
     * @return A new instance of TDateTime initialized with the specified date and
     *         time components.
     */
    TDateTime(int Year, int Month, int Day, int Hour, int Min, int Sec, int ms, int us);

    /**
     * Assigns the values of another TDateTime object to this instance.
     *
     * This operator copies all date and time components from the source TDateTime
     * object to the current instance. It supports copying high precision fields
     * like milliseconds and microseconds, along with platform-specific data for
     * time representation.
     *
     * @param src The source TDateTime object to copy from.
     * @return A reference to the updated instance of this TDateTime object.
     */
    const TDateTime &operator=(const TDateTime &src);

    /**
     * Converts the TDateTime object into a long double representation, where the
     * integer part represents the most significant bits (MSB) of the date and time,
     * and the fractional part represents the least significant bits (LSB).
     * This representation allows for precise storage and calculations of date and time values.
     *
     * @return A long double representing the TDateTime object with its MSB as the integer part
     *         and its LSB as the fractional part.
     */
    operator long double () const;

    /**
     * Sets the date and time of the TDateTime object using a provided Linux timestamp.
     * The Linux timestamp represents the number of seconds that have elapsed since
     * the Unix epoch (January 1, 1970, 00:00:00 UTC).
     *
     * @param val The Linux timestamp in seconds to set as the value of the TDateTime object.
     */
    void SetLinuxTimestamp(long long val);

    /**
     * Sets the internal date and time representation using a given Linux millisecond
     * timestamp, which represents the number of milliseconds since the Unix epoch
     * (January 1, 1970, 00:00:00 UTC).
     *
     * @param val The Linux millisecond timestamp to set the internal date and time.
     */
    void SetLinuxMilliTimestamp(long long val);

    /**
     * Updates the current instance of TDateTime to represent the current system
     * date and time. The method utilizes platform-specific mechanisms to fetch
     * the current time, calculates local time adjustments if necessary, and
     * converts the retrieved raw time into structured date and time components.
     */
    void SetCurrent();

    /**
     * Retrieves the most significant part of the internal timestamp representation.
     *
     * On RDOS systems, this method returns the stored most significant part directly.
     * On non-RDOS systems, it converts the internal timestamp from the Linux `timespec`
     * format into the corresponding RDOS representation and returns the most significant part.
     *
     * @return The most significant part of the internal timestamp.
     */
    unsigned long GetMsb() const;

    /**
     * Retrieves the least significant bits (LSB) of the current date and time
     * as represented in the internal format of the TDateTime object.
     *
     * On systems using RDOS, the value is directly extracted from the FLsb member.
     * On other systems, it is calculated by converting the internal timestamp
     * into an RDOS-compatible format.
     *
     * @return The LSB part of the timestamp, representing date and time
     *         in the internal format of the TDateTime object.
     */
    unsigned long GetLsb() const;

    /**
     * Sets the raw date and time values using the provided most significant bits
     * (Msb) and least significant bits (Lsb).
     *
     * On an RDOS platform, the Msb and Lsb values are stored directly. On other
     * platforms, they are converted to a `struct timespec` format representing the
     * equivalent timestamp.
     *
     * @param Msb The most significant bits of the raw date and time value.
     * @param Lsb The least significant bits of the raw date and time value.
     */
    void SetRaw(unsigned long Msb, unsigned long Lsb);

    /**
     * Determines whether the current TDateTime instance has expired based on the
     * system's current date and time.
     *
     * This method checks the stored time components of the object against the
     * current system time. If the current time is greater than or equal to the
     * stored time, the method will consider the object expired.
     *
     * @return True if the TDateTime instance has expired; otherwise, false.
     */
    bool HasExpired() const;

    /**
     * Blocks the execution of the current thread until the expiration time
     * represented by the TDateTime instance is reached.
     *
     * This method compares the stored expiration time with the current system
     * time and calculates the remaining time until expiration. If the expiration
     * time has not yet elapsed, it pauses the thread for the remaining duration.
     *
     * For systems supporting RDOS, the method utilizes `RdosWaitRealUntil` to
     * wait for the specified time. On other systems, it uses a combination of
     * system calls to compute the time difference and pauses execution
     * accordingly.
     *
     * @note If the expiration time has already passed when this method is called,
     *       the method returns immediately without blocking the thread.
     */
    void WaitUntilExpired() const;

    /**
     * Adds the specified number of "tics" to the current date and time.
     * The "tics" are added to the internal representation, and the updated
     * date and time are recalculated accordingly.
     *
     * @param tics The number of tics to add. A tic typically represents a
     *             small unit of time, depending on the system implementation.
     */
    void AddTics(long tics);

    /**
     * Adds a specified number of microseconds to the current date and time.
     * The method adjusts the internal representation of the date and time
     * accordingly and ensures that all values remain valid.
     *
     * @param us The number of microseconds to add. Can be positive to move forward
     *           in time or negative to move backward in time.
     */
    void AddMicro(long us);

    /**
     * Adds the specified number of milliseconds to the current date and time.
     *
     * @param ms The number of milliseconds to add. Can be positive to move
     *           forward in time or negative to move backward.
     */
    void AddMilli(long ms);

    /**
     * Adds a specified number of seconds to the current date and time.
     *
     * This method updates the internal representation of the TDateTime object by adding
     * the given number of seconds to its time value. After the addition, it synchronizes
     * the raw time representation with the record representation.
     *
     * @param sec The number of seconds to add. Can be positive or negative to adjust
     *            the time forward or backward, respectively.
     */
    void AddSec(long sec);

    /**
     * Adds the specified number of minutes to the current TDateTime instance.
     *
     * @param min The number of minutes to add. This value can be positive
     *            to move forward in time or negative to move backward.
     */
    void AddMin(long min);

    /**
     * Adds a specified number of hours to the current date and time represented
     * by the TDateTime object. The operation updates the internal timestamp
     * and recalculates the date and time components accordingly.
     *
     * @param hour The number of hours to add. Can be negative to subtract hours.
     */
    void AddHour(long hour);

    /**
     * Adjusts the current date and time by adding a specified number of days to it.
     * The adjustment respects the boundaries of months and years.
     *
     * @param day The number of days to add. Positive values move the date forward,
     *            while negative values move it backward.
     */
    void AddDay(long day);

    /**
     * Adjusts the current date by adding or subtracting a specified number of months.
     * The operation updates the internal state of the TDateTime object, maintaining
     * the correct year and month values even if the addition/subtraction causes overflow
     * or underflow in months. For example, adding 14 months to December of one year will
     * correctly result in February of the second year.
     *
     * If the month adjustment results in a change in years, both months and years are
     * updated accordingly. After the update, the internal raw representation of the
     * datetime is recalculated.
     *
     * @param month The number of months to add to the current date. Can be positive
     *              (to add months) or negative (to subtract months).
     */
    void AddMonth(long month);

    /**
     * Adds the specified number of years to the current TDateTime instance.
     *
     * This method updates the year field of the TDateTime object by the given
     * value and adjusts the internal representation to reflect the change.
     *
     * @param year The number of years to add. Can be positive to move forward
     *             in time or negative to move backward in time.
     */
    void AddYear(long year);

    /**
     * Advances the current date to the next day while resetting the time
     * components (hour, minute, second, millisecond, microsecond) to zero.
     *
     * This method modifies the internal date and time representation of the
     * TDateTime instance. It ensures that the time is set to the beginning
     * of the next day, accurately updating the internal members and invoking
     * necessary conversions to synchronize the representation.
     */
    void NextDay();

    /**
     * Retrieves the day of the week for the current date stored in the TDateTime object.
     *
     * The method returns the day of the week as an integer, where the value is
     * typically represented according to the convention:
     * 0 = Sunday, 1 = Monday, ..., 6 = Saturday.
     *
     * @return An integer representing the day of the week for the current date.
     */
    int GetDayOfWeek() const;

    /**
     * Retrieves the year component of the date represented by the TDateTime instance.
     *
     * @return The year as an integer.
     */
    int GetYear() const;

    /**
     * Retrieves the month component of the date stored in the TDateTime instance.
     *
     * @return The month as an integer, where 1 represents January and 12 represents December.
     */
    int GetMonth() const;

    /**
     * Retrieves the day of the month represented by the current TDateTime object.
     *
     * @return The day of the month (1-31) stored within the TDateTime instance.
     */
    int GetDay() const;

    /**
     * Retrieves the hour component of the datetime.
     *
     * @return The hour component of the datetime, as an integer ranging from 0 to 23.
     */
    int GetHour() const;

    /**
     * Retrieves the minute component of the stored date and time.
     *
     * @return The minute value (0-59) of the current date and time.
     */
    int GetMin() const;

    /**
     * Retrieves the second component of a TDateTime object.
     * The value represents the current second within a minute.
     *
     * @return The second value, ranging from 0 to 59.
     */
    int GetSec() const;

    /**
     * Retrieves the millisecond component of the time stored in the TDateTime object.
     *
     * @return The millisecond part (0-999) of the time.
     */
    int GetMilliSec() const;

    /**
     * Retrieves the microsecond component of the stored date and time.
     *
     * @return The microsecond value (0-999999) representing the fraction of a
     *         second in the current TDateTime object.
     */
    int GetMicroSec() const;

    /**
     * Retrieves the Linux timestamp representation of the current TDateTime object.
     * The Linux timestamp is the number of seconds since the Unix epoch
     * (January 1, 1970, 00:00:00 UTC).
     *
     * @return The Linux timestamp for the current TDateTime object.
     */
    long long GetLinuxTimestamp() const;

    /**
     * Retrieves the current timestamp in milliseconds since the Unix epoch
     * (January 1, 1970, 00:00:00 UTC).
     *
     * @return The timestamp as a 64-bit integer representing the number of
     *         milliseconds since the Unix epoch.
     */
    long long GetLinuxMilliTimestamp() const;

    /**
     * Sets the internal date and time representation. This method adjusts the
     * platform-specific date and time source depending on the environment.
     *
     * - On RDOS platforms, it configures the time using the internal Msb and Lsb values.
     * - On non-RDOS platforms, it adjusts the time based on the `timespec` `FTs`, calculating
     *   any necessary time differences.
     *
     * This method is typically used to synchronize or set the date and time values
     * internally for the `TDateTime` instance, utilizing the platform-specific implementation.
     */
    void Set();

    /**
     * Populates the individual date and time fields of the TDateTime object based
     * on its internal raw time representation.
     *
     * This method decodes the raw time information stored within the object
     * and updates the year, month, day, hour, minute, second, millisecond,
     * and microsecond fields with the corresponding values.
     *
     * On RDOS platforms, it uses Rdos-specific decoding of the MSB and LSB values.
     * On non-RDOS platforms, it uses the BinaryToTime method and additional
     * calculations to derive time components.
     */
protected:
    void RawToRecord();

    /**
     * Updates the raw time representation of the TDateTime object based on its
     * current date and time components.
     *
     * This method calculates and sets either a platform-specific representation
     * of time (for RDOS systems) or a standard time format (on other platforms).
     * For RDOS systems, it utilizes `RdosCodeMsbTics` and `RdosCodeLsbTics` to
     * encode the time. On non-RDOS systems, it calculates seconds since the Unix
     * epoch and nanoseconds based on the object's components.
     *
     * This method is called internally after modifications to date or time
     * components to ensure that the raw time representation remains consistent.
     *
     * Note: This function should not be directly invoked by users; it is meant
     * to be used internally by the class or its methods after time-related
     * changes are made.
     */
    void RecordToRaw();

private:
#ifdef __RDOS__
    /**
     * The most significant 32 bits of a raw timestamp used for internal
     * representation of date and time. This variable is part of the
     * raw storage mechanism for timestamp data and is typically managed
     * in conjunction with the least significant 32 bits (FLsb) to form
     * a complete 64-bit representation of time.
     */
    unsigned long FMsb;
    /**
     * Stores the least significant bits (LSB) of the raw datetime value for
     * encoding and manipulating date and time data in a low-level binary format.
     *
     * This value is typically used in conjunction with a most significant bits
     * (MSB) counterpart to represent a complete raw datetime value in systems
     * where datetime computations or storage rely on a binary representation.
     */
    unsigned long FLsb;
#else
    /**
     * Represents a structure containing time specifications with seconds and
     * nanoseconds precision since the Unix epoch.
     *
     * The `FTs` variable usually holds detailed time information relevant
     * for high-resolution time operations. It includes:
     * - `tv_sec`: Seconds since the epoch (January 1, 1970).
     * - `tv_nsec`: Additional nanoseconds component to extend precision beyond seconds.
     *
     * This structure is leveraged for operations requiring precise timekeeping
     * or elapsed time measurements.
     */
    struct timespec FTs;
#endif

    /**
     * Represents the year component of a date in the TDateTime class.
     *
     * This variable stores the four-digit year (e.g., 2023) associated with a
     * specific instance of TDateTime. The value is internally set or updated
     * through class constructors or assignment operations, and it is used
     * in conjunction with other date and time components to define a complete
     * date-time representation.
     */
    int FYear;
    /**
     * Represents the month component of a date within the TDateTime class.
     *
     * This variable stores the numerical value of the month (1 for January, 2 for February,
     * ..., 12 for December) as part of the internal date representation.
     *
     * It is used in various operations and methods within the TDateTime class to
     * handle and manipulate date information.
     */
    int FMonth;
    /**
     * Represents the day of the month in a date.
     *
     * This variable holds a value corresponding to the day component
     * of a specific date. Its value should fall within the range of 1 to
     * 31, depending on the month and year, and is used in the internal
     * representation and manipulation of dates within the TDateTime class.
     */
    int FDay;
    /**
     * Holds the hour component of a date and time, represented as an integer
     * in the range from 0 to 23.
     *
     * This variable is used internally within the `TDateTime` class to
     * store the hour portion of a specific datetime value. It is initialized
     * during the construction of a `TDateTime` object or updated when related
     * operations are performed on the instance.
     */
    int FHour;
    /**
     * Stores the minute component of a date and time.
     *
     * This variable represents the minute (0–59) within an hour in the context
     * of the TDateTime class. It is used for time representation and manipulation
     * throughout the class's operations, such as construction, assignment, and
     * date/time calculations.
     */
    int FMin;
    /**
     * Represents the second component of a time value in the context of a
     * TDateTime object. It typically ranges from 0 to 59 and is used to store
     * the number of seconds in a given time representation.
     */
    int FSec;
    /**
     * Stores the millisecond component of a timestamp in the TDateTime class.
     *
     * This variable represents the fractional seconds component of a datetime
     * instance, expressed in milliseconds (0-999). It is used in conjunction with
     * the other components (year, month, day, hour, minute, second, and microseconds)
     * to construct and manipulate a complete timestamp.
     */
    int FMilli;
    /**
     * Stores the microsecond component of a datetime value. This represents the
     * fractional part of a second at the microsecond precision level.
     *
     * FMicro is typically used for operations requiring precise time
     * calculations, such as high-resolution timestamps or computing
     * durations with microsecond accuracy. It complements the millisecond
     * component stored in FMilli and other time components like FSec, FMin, etc.
     *
     * The range of FMicro is 0 to 999,999, corresponding to the valid range of
     * microseconds within one second.
     */
    int FMicro;
};

#endif

