#include <unistd.h>
#include <fcntl.h>

#include "status-net.h"

static unsigned long long int previous_rec, previous_sent;
static unsigned long long int current_rec, current_sent;


int parse_netdev()
{
	char buffer[4096];
	char* buf = buffer;
	char* end;

	int netstat_fd = open( "/proc/net/dev", O_RDONLY );

	if ( netstat_fd == - 1 )
		return -1;

	ssize_t amount = read( netstat_fd, buf, 4096 );
	if ( amount == -1 )
		return -1;

	buffer[amount] = '\0';
	end = buffer + amount;

	close( netstat_fd );

	// Ignore the first two lines of the file
	buf = strchr( buf, '\n' );
	if ( buf == NULL )
		return -1;
	++buf;
	buf = strchr( buf, '\n' );
	if ( buf == NULL )
		return -1;
	++buf;

	// Reset the actual counters.
	current_rec = 0;
	current_sent = 0;

	// Temp variable for per-interface values
	unsigned long long int receivedacc, sentacc;

	int rval = 1;
	char *nl_pos, *lo_pos, *data_pos;

	while ( buf < end )
	{
		// Find character after EOL
		nl_pos = strchr( buf, '\n' ) + 1;
		// Skip the `lo:` interface.
		lo_pos = strstr( buf, "lo:" );

		if ( lo_pos != NULL && lo_pos < nl_pos)
		{
			buf = nl_pos;
			continue;
		}

		// Skip the interface header
		data_pos = strstr( buf, ":" );

		// Scan the data
		// With thanks to the conky project at http://conky.sourceforge.net/
		sscanf(
			data_pos + 1,
			"%llu  %*d     %*d  %*d  %*d  %*d   %*d        %*d       %llu",
			&receivedacc,
			&sentacc
		);

		// Add it to the accumulator
		current_rec += receivedacc;
		current_sent += sentacc;

		buf = nl_pos;
		rval = 0;
	}

	return rval;
}

int calculate_speed(
	char speedstr[PER_SEGMENT_BUFFER],
	size_t offset,
	unsigned long long int newval,
	unsigned long long int oldval
)
{
	double speed;
	speed = (newval - oldval) / 1024.0;
	if (speed > 1024.0)
	{
	    speed /= 1024.0;
	    return snprintf( speedstr + offset, PER_SEGMENT_BUFFER - offset, "%.3fM", speed );
	}
	else
	{
	    return snprintf( speedstr + offset, PER_SEGMENT_BUFFER - offset, "%.2fK", speed );
	}
}

size_t st_netusage( char buffer[PER_SEGMENT_BUFFER] )
{
	int retval;

	retval = parse_netdev();
	if (retval)
	{
	    fprintf( stdout, "Error when parsing /proc/net/dev file.\n" );
	    return strlcpy( buffer, "ERR!", PER_SEGMENT_BUFFER );
	}

	if ( previous_rec == 0 )
	{
		previous_rec = current_rec;
		previous_sent = current_sent;
		return strlcpy( buffer, ".INIT.", PER_SEGMENT_BUFFER );
	}

	size_t pos = 0;
	pos += strlcpy( buffer, "Rx:", PER_SEGMENT_BUFFER );
	pos += calculate_speed( buffer, pos, current_rec, previous_rec );
	pos += strlcpy( buffer + pos, " Tx:", PER_SEGMENT_BUFFER - pos );
	pos += calculate_speed( buffer, pos, current_sent, previous_sent );

	previous_rec = current_rec;
	previous_sent = current_sent;

	return pos;
}
