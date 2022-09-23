#ifndef __WALLCLOCK_H__
#define __WALLCLOCK_H__

void getWallClock(int *HH,int *MM,int *SS);
void setWallClockHook(void (*func)(int, int, int));
void setWallClock(int HH,int MM,int SS);

#endif
