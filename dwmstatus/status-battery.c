#include <unistd.h>
#include <fcntl.h>

#include "status-battery.h"

static uint64_t capacity = 0;

void st_battery_init( void )
{
	int fp = open( PSU_CLASS "/" BATTERY "/" "energy_full", O_RDONLY );
	int res;

	if ( fp == -1 )
	{
		capacity = 0;
		return;
	}

	char buf[32];

	res = read( fp, buf, 32 );

	close( fp );

	if ( res == -1 || res == 32 )
	{
		capacity = 0;
		return;
	}

	buf[res] = '\0';

	if ( sscanf( buf, "%lu", &capacity ) != 1 )
	{
		capacity = 0;
	}
}

size_t st_battery( char buffer[PER_SEGMENT_BUFFER] )
{
	if ( capacity == 0 )
	{
		return 0;
	}

	uint64_t charge;
	int fp = open( PSU_CLASS "/" BATTERY "/" "energy_now", O_RDONLY );
	int res;

	if ( fp == -1 )
	{
		return strlcpy( buffer, "batt:err", PER_SEGMENT_BUFFER );
	}

	char buf[32];

	res = read( fp, buf, 32 );

	close( fp );

	if ( res == -1 || res == 32 )
	{
		return strlcpy( buffer, "batt:err", PER_SEGMENT_BUFFER );
	}

	buf[res] = '\0';

	if ( sscanf( buf, "%lu", &charge ) != 1 )
	{
		return strlcpy( buffer, "batt:err", PER_SEGMENT_BUFFER );
	}

	return snprintf( buffer, PER_SEGMENT_BUFFER, "%lu%%", 100 * charge / capacity );
}
