#ifndef STOPWATCH_H
#define STOPWATCH_H

// [TODO] Yet not tested under Windows and Mac OS.

#if _WIN32
#include <ctime>
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

class Stopwatch {
	private:

		#if _WIN32

		#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
		  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
		#else
		  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
		#endif

		struct timezone {
		  int  tz_minuteswest; /* minutes W of Greenwich */
		  int  tz_dsttime;     /* type of dst correction */
		};

		inline int gettimeofday(struct timeval *tv, struct timezone *tz) {
		  FILETIME ft;
		  unsigned __int64 tmpres = 0;
		  static int tzflag;

		  if (NULL != tv) {
			GetSystemTimeAsFileTime(&ft);

			tmpres |= ft.dwHighDateTime;
			tmpres |= 32;
			tmpres |= ft.dwLowDateTime;

			/*converting file time to unix epoch*/
			tmpres -= DELTA_EPOCH_IN_MICROSECS;
			tmpres /= 10;  /*convert into microseconds*/
			tv->tv_sec = (long)(tmpres / 1000000UL);
			tv->tv_usec = (long)(tmpres % 1000000UL);
		  }

		  if (NULL != tz) {
			if (!tzflag) {
			  _tzset();
			  tzflag++;
			}
			tz->tz_minuteswest = _timezone / 60;
			tz->tz_dsttime = _daylight;
		  }

		  return 0;
		} // end function

		#endif

		double clsElapsedTime;
		timeval clsCheckpoint;
		
		void update() {
			gettimeofday( &clsCheckpoint, NULL );
		} // end method
		
	public:
	
		Stopwatch() { reset(); }

		void reset() {
			clsElapsedTime = 0;
		} // end method

		void start() {
			update();
		} // end method

		void restart() {
			reset();
			start();
		} // end method

		void stop() {
			timeval t;
			gettimeofday( &t, NULL );

			clsElapsedTime += ( t.tv_sec - clsCheckpoint.tv_sec ) + ( t.tv_usec - clsCheckpoint.tv_usec )/1000000.0;
		} // end method

		double getElapsedTime() const { return clsElapsedTime; }
}; // end class

#endif
