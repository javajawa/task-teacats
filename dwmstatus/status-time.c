#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "status-time.h"

size_t st_datetime( char buffer[PER_SEGMENT_BUFFER] )
{
	char const * const fmt = "%a %d %b %H:%M:%S";

	time_t tim = time( NULL );
	struct tm timtm;

	if ( localtime_r( &tim, &timtm ) == NULL )
	{
		return strlcpy( buffer, "TIME ERR!", PER_SEGMENT_BUFFER );
	}

	int result = strftime( buffer, PER_SEGMENT_BUFFER-1, fmt, &timtm );

	if ( result == 0 )
	{
		return strlcpy( buffer, "strftime err!", PER_SEGMENT_BUFFER );
	}

	return result;
}
