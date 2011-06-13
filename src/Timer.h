/***************************************************************************\
Tool to find the correlation dimension of a sequence.
Copyright (C) 2010 Tejaswi.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
\***************************************************************************/


#ifndef __INCLUDED_TIMER_H__
#define __INCLUDED_TIMER_H__

#define __USE_TIMEVAL__
#if defined(_WIN32) || defined(__CYGWIN__)
#undef __USE_TIMEVAL__
#endif


#ifdef __USE_TIMEVAL__
#include <sys/time.h>
#endif
#include <sys/types.h>
#include <time.h>
#include "basics.h"



/**
 * Class to measure the events.
 */
class Timer {
private:
#ifdef __USE_TIMEVAL__
    timeval m_start;     ///< clock value when Timer is started
    timeval m_stop;      ///< clock value when Timer is stopped
#else
    clock_t m_start;     ///< clock value when Timer is started
    clock_t m_stop;      ///< clock value when Timer is stopped
#endif
    bool m_stopped;      ///< If the Timer has been stopped after being started
    bool m_started;      ///< True if the Timer has been started
    REAL m_elapsed;      ///< Elapsed number of seconds between start and stop commands

private:
#ifdef __USE_TIMEVAL__
    /**
     * @brief Set current time
     * @param t the returned time from the system
     */
    void setTime(timeval& t);
#else
    /**
     * @brief Set current time
     * @param t the returned time from the system
     */
    void setTime(clock_t& t);
#endif

public:
    /**
     * @brief Constructor for this class
     */
    Timer();

    /**
     * @brief Destructor for this class
     */
    ~Timer() {}

    /**
     * @brief Start the counter
     */
    void start();

    /**
     * @brief Stop the counter
     */
    void stop();

    /**
     * @brief Report the elapsed time
     * @return Number of seconds elapsed between the start() and stop() calls
     */
    REAL report();

    /**
     * @brief Helper function to stop the timer and then print the time
     * @param fmt Message to be printed along with the elapsed time.
     *
     * 'fmt' MUST contain a '%f' character inside which will tell this
     * function to add the elapsed time into the printed message! If this is
     * not followed, then you'll get strange compile time errors!
     * Also, '%f' is the ONLY formatting character allowed in this string!
     */
    void stopAndPrintTime(const char* fmt);
};


// DO NOT WRITE ANYTHING BELOW THIS LINE!!!!
#endif // __INCLUDED_TIMER_H__
