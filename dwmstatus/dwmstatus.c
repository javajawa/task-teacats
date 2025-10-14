#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "config.h"
#include "status-battery.h"
#include "status-load.h"
#include "status-net.h"
#include "status-time.h"

#define LOOPS_PER_SECOND 2

struct status_segment {
	size_t (*updater) ( char buf[PER_SEGMENT_BUFFER] );
	uint8_t period;

	char buffer[PER_SEGMENT_BUFFER];
	size_t len;
};

volatile bool continue_looping = 1;

void interrupt_handler( int signal )
{
	if ( signal == SIGINT ) {
		continue_looping = 0;
	}
}

int main( void )
{
	Display* const display = XOpenDisplay( NULL );

	if ( !display ) {
		fprintf( stderr, "dwmstatus: cannot open display.\n" );
		return 1;
	}

	const Window window = DefaultRootWindow( display );

	st_battery_init();
	struct status_segment segments[] = {
		{ st_loadavg, LOOPS_PER_SECOND * 2, "", 0 },
		{ st_netusage, LOOPS_PER_SECOND, "", 0 },
		{ st_battery, LOOPS_PER_SECOND * 30, "", 0 },
		{ st_datetime, 0, "", 0 },
	};
	const size_t number_of_segments = sizeof(segments) / sizeof(struct status_segment);

	char output[TOTAL_BUFFER] = " ";
	size_t total_length;

	unsigned int loop_counter = 0;

	signal( SIGINT, interrupt_handler );

	while ( continue_looping )
	{
		total_length = 1;

		for ( size_t idx = 0; idx < number_of_segments; ++idx ) {
			if ( segments[idx].period == 0 || ( loop_counter % segments[idx].period ) == 0 ) {
				segments[idx].len = segments[idx].updater( segments[idx].buffer );
			}

			if ( segments[idx].len > 0 ) {
				// Add dividers when not on the first item
				if ( total_length > 1 ) {
					total_length += strlcpy( output + total_length, " | ", TOTAL_BUFFER - total_length );
				}

				total_length += strlcpy( output + total_length, segments[idx].buffer, TOTAL_BUFFER - total_length );
			}
		}

		XStoreName( display, window, output );
		XSync( display, False );

		++loop_counter;
		usleep( 1000000 / LOOPS_PER_SECOND );
	}

	XStoreName( display, window, "[dwmstatus shutdown]" );
	XSync( display, False );

	XCloseDisplay( display );

	return 0;
}
