#include "config.h"

#define BATTERY "BAT0"
#define PSU_CLASS "/sys/class/power_supply"

void st_battery_init( void );
size_t st_battery( char buffer[PER_SEGMENT_BUFFER] );
