#include <sys/sysinfo.h>

#include "status-load.h"

size_t st_loadavg( char buffer[PER_SEGMENT_BUFFER] )
{
	struct sysinfo info;

	if ( sysinfo( &info ) )
	{
		return strlcpy( buffer, "ERR!", PER_SEGMENT_BUFFER );
	}

	uintmax_t used_megs = (info.totalram - info.freeram) >> 20;
	uintmax_t total_megs = info.totalram >> 20;

	// hardware manufacturers appearantly count 1,000 megabytes as a gigabyte.
	// why? who the fuck knows.
	// we fix this by making our final division by 1,000 not 1,024.
	double used_gigs = ( (double)used_megs ) / 1024.0;
	double total_gigs = ( (double)total_megs ) / 1024.0;

	return snprintf(
		buffer, 24,
		"%.2f %.2f | %.1f/%.1fGB",
		info.loads[0] / (float)(1 << SI_LOAD_SHIFT),
		info.loads[2] / (float)(1 << SI_LOAD_SHIFT),
		used_gigs,
		total_gigs
	);
}


