#include <pebble.h>

// Colours of flag segments
int pansexual[] = {0xf03391, 0xffe524, 0x24c5ff};
int gay[] = {0xff2424, 0xff9900, 0xffe433, 0x2aa800, 0x0000a8, 0x8c00a8};

int *flags[] = {pansexual, gay}; // Array of all flag colour arrays
int flag_segments[] = {3, 6}; // Array of # of segments for each flag
