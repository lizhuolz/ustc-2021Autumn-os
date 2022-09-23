#include "tick.h"
#include "wallClock.h"
extern void oneTickUpdateWallClock(int HH, int MM, int SS);

void tick(void){
	//你需要填写这里
	system_ticks++;
	if (system_ticks % 100 == 0) {
		getWallClock(&HH, &MM, &SS);
		oneTickUpdateWallClock(HH, MM, SS);
	}

	return;
}

