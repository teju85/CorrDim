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


#include "Timer.h"


Timer::Timer() {
    m_stopped = false;
    m_started = false;
    m_elapsed = -2;
}

#ifdef __USE_TIMEVAL__
void Timer::setTime(timeval& t) {
    gettimeofday(&t, 0);
}
#else
void Timer::setTime(clock_t& t) {
    t = clock();
}
#endif

void Timer::start() {
    m_stopped = false;
    m_started = true;
    m_elapsed = -2;
    setTime(m_start);
}

void Timer::stop() {
    if(!m_started) {
        fprintf(stderr, "You cannot call 'Timer::stop' without calling 'Timer::start' first!\n");
        return;
    }
    m_stopped = true;
    m_started = false;
    setTime(m_stop);
}

REAL Timer::report() {
    if(!m_stopped) {
        fprintf(stderr, "You cannot call 'Timer::report' without calling 'Timer::stop' first!\n");
        return -1;
    }
    if(m_elapsed != -2) {
        return m_elapsed;
    }
#ifdef __USE_TIMEVAL__
    m_elapsed = (m_stop.tv_sec - m_start.tv_sec) + ((m_stop.tv_usec - m_start.tv_usec) / 1000000.0);
#else
    m_elapsed = (m_stop - m_start) / ((double) CLOCKS_PER_SEC);
#endif
    return m_elapsed;
}

void Timer::stopAndPrintTime(const char* fmt) {
    stop();
    fprintf(stdout, fmt, report());
}
