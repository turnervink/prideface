#include <pebble.h>

// Colours of flag segments
int pansexual[] = {0xf03391, 0xffe524, 0x24c5ff};
int gay[] = {0xff2424, 0xff9900, 0xffe433, 0x2aa800, 0x0000a8, 0x8c00a8};
int asexual[] = {0x000000, 0xa2a2a2, 0xffffff, 0x762979};
int bisexual[] = {0xFF00AA, 0xFF00AA, 0xAA55FF, 0x0055AA, 0x0055AA};
int trans[] = {0x00AAFF, 0xFFAAFF, 0xffffff, 0xFFAAFF, 0x00AAFF};
int polysexual[] = {0xf03391, 0x00AA00, 0x55AAFF};
int nonbinary[] = {0xFFFF00, 0xffffff, 0xAA00FF, 0x000000};
int blank[] = {0xffffff};

int *flags[] = {pansexual, gay, asexual, bisexual, trans, polysexual, nonbinary, blank}; // Array of all flag colour arrays
int flag_segments[] = {3, 6, 4, 5, 5, 3, 4, 1}; // Array of # of segments for each flag
