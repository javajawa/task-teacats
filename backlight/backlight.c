#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>


#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)
#define errfs(code,format,...) fprintf( stderr, "[" __FILE__ ":" STR(__LINE__) "] " format "\n", __VA_ARGS__ ); if ( code ) return code;


char const * const PATH = "/sys/devices/pci0000:00/0000:00:02.0/backlight/acpi_video0/brightness";


int set_value( char const * const value ) {
	int fd;
	int status;

	uint8_t brightness;

	status = sscanf( value, "%hhu", &brightness );
	if ( status == -1 ) {
		errfs( 1, "Error parsing %s", value );
	}

	fd = open( PATH, O_WRONLY | O_TRUNC );
	if ( fd == -1 ) {
		errfs( 1, "Can not write to %s", PATH );
	}

	status = write( fd, value, strlen( value ) );
	if ( status == -1 ) {
		errfs( 0, "Error writing to %s", value );
	} else {
		printf( "Brightness set to %s\n", value );
	}

	status = close( fd );
	fd = -1;
	if ( status ) {
		errfs( 0, "Error closing %s", PATH );
	}

	return 0;
}

int show_current( void ) {
	int fd;
	int status;
	FILE* file;

	uint8_t brightness;

	fd = open( PATH, O_RDONLY );
	if ( fd == -1 ) {
		errfs( 1, "Can not read %s", PATH );
	}

	file = fdopen( fd, "r" );
	if ( file == NULL ) {
		close( fd );
		errfs( 1, "Can not read %s", PATH );
	}

	status = fscanf( file, "%hhu", &brightness );
	if ( status == EOF ) {
		fclose( file );
		errfs( 0, "Error processing %s", PATH );
	}

	printf( "Current Brightness: %hhu\n", brightness );

	status = fclose( file );
	fd = -1;
	if ( status ) {
		errfs( 0, "Error closing %s", PATH );
	}

	return 0;
}

int main( int const argc, char const * const * const argv )
{
	if (argc == 1) {
		return show_current();
	}
	return set_value( argv[1] );
}
