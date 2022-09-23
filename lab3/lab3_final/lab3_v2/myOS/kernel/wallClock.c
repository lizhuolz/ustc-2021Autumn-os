#include "wallClock.h"

void (*wallClock_hook)(int, int, int) = 0;
void oneTickUpdateWallClock(int HH, int MM, int SS){
	if(wallClock_hook) wallClock_hook(HH,MM,SS);
}

void setWallClockHook(void (*func)(int, int, int)) {
	wallClock_hook = func;
}

int hh = 0, mm = 0, ss = 0;
void setWallClock(int HH,int MM,int SS){//hook
	//你需要填写这里

	SS = (SS + 1) % 60;
	if (SS == 0)
		MM = (MM + 1) % 60;
	if (MM == 0 && SS == 0)
		HH = (HH + 1) % 24;
	ss = SS; mm = MM; hh = HH;

	unsigned short int* p;
	p = (unsigned short int*)(0xB8000 + 24 * 160 + 2 * 72);
	*p++ = 0x2f00 + 0x30 + (unsigned short int)(HH / 10);
	*p++ = 0x2f00 + 0x30 + (unsigned short int)(HH % 10);
	*p++ = 0x2f00 + 0x3a;
	*p++ = 0x2f00 + 0x30 + (unsigned short int)(MM / 10);
	*p++ = 0x2f00 + 0x30 + (unsigned short int)(MM % 10);
	*p++ = 0x2f00 + 0x3a;
	*p++ = 0x2f00 + 0x30 + (unsigned short int)(SS / 10);
	*p++ = 0x2f00 + 0x30 + (unsigned short int)(SS % 10);
	return;
}

void getWallClock(int *HH,int *MM,int *SS){
	//你需要填写这里
	*HH = hh; *MM = mm; *SS = ss;
	return;
	
}
