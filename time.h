// Raspicop@Jean Collas 2015-10-02 : time-related functions

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <time.h> // clock_gettime & time structs

using namespace std;

long getMax(long a, long b) {return a>b?a:b;}
long getMin(long a, long b) {return a>b?b:a;}


namespace Time
{

	long getMissingMicrosec(timespec start, timespec * endTime, long totalMicroSec)
	{
		long elapsedNsec, elapsedSec;
		clock_gettime(CLOCK_MONOTONIC, endTime);
		elapsedNsec = endTime->tv_nsec - start.tv_nsec;
		elapsedSec = endTime->tv_sec - start.tv_sec;
		if(elapsedSec>0 && elapsedNsec<0)
		{
			elapsedSec-=1;
			// supposed only 1 sec elapsed
			elapsedNsec+=1000000000;
		}

		// Supposed that loop is less than 1sec
		if(elapsedSec > 0) return 0;
	//	if(elapsedSec > LOOP_MICROSEC/1000/1000) return 0;

		if(elapsedNsec/1000>totalMicroSec) return 0;
//		cout << totalMicroSec-(elapsedNsec/1000)<< endl;
		return getMin(totalMicroSec, getMax(0, totalMicroSec-(elapsedNsec/1000) ));
	}

	long getMissingMicrosec(timespec start, long totalMicroSec)
	{
		long elapsedNsec, elapsedSec;
		timespec endTime;
		clock_gettime(CLOCK_MONOTONIC, &endTime);
		elapsedNsec = endTime.tv_nsec - start.tv_nsec;
		elapsedSec = endTime.tv_sec - start.tv_sec;
		if(elapsedSec>0 && elapsedNsec<0)
		{
			elapsedSec-=1;
			// supposed only 1 sec elapsed
			elapsedNsec+=1000000000;
		}

		// Supposed that loop is less than 1sec
		if(elapsedSec > 0) return 0;
	//	if(elapsedSec > LOOP_MICROSEC/1000/1000) return 0;

		if(elapsedNsec/1000>totalMicroSec) return 0;
		return getMin(totalMicroSec, getMax(0, totalMicroSec-(elapsedNsec/1000) ));
	}


/*
    // result=abs(x-y), returns 0=positive, 1=negative
    int
    timeval_subtract (timespec *result, timespec *x, timespec *y)
    {
        timespec * max;
        timespec * min;

        bool xIsMax=(x->tv_sec > y->tv_sec) || ((x->tv_sec == y->tv_sec) && (x->tv_usec >= y->tv_usec));
        max=xIsMax?x:y;
        min=xIsMax?y:x;

        bool umaxIsMax=(max->tv_usec >= min->tv_usec);

        result-> tv_usec=max->tv_usec - min->tv_usec+(umaxIsMax?0:1000000000);

        result-> tv_sec=max->tv_sec - min->tv_sec-(umaxIsMax?0:1);

        return 1-xIsMax;

    }
*/
}

#endif
