#include "../../include/myPrintk.h"
#include "../../include/mem.h"
//unsigned long pMemStart;//可用的内存的起始地址
//unsigned long pMemSize;//可用的大小

void memTest(unsigned long start, unsigned long grainSize){
	/*本函数需要实现！！！*/
	/*功能：检测算法
		这一个函数对应实验讲解ppt中的第一大功能-内存检测。
		本函数的功能是检测从某一个地址开始的内存是否可用，具体算法就可以用ppt上说的写了读看是否一致。
	注意点两个：
	1、开始的地址要大于1M，需要做一个if判断。
	2、grainSize不能太小，也要做一个if判断
	*/
	if (start <= (1 << 20))
		start = (1 << 20);
	if (grainSize < (1 << 10))
		grainSize = (1 << 10);
	pMemStart = start;
	pMemSize = 0;
	unsigned long position = start;
	unsigned short* pus = (unsigned short*)start;
	//check memory
	while (1){
		// write 0xAA55 0x55AA and writeback
		//head 
		unsigned short save = *pus;
		*pus = 0xAA55;
		if (*pus != 0xAA55)
			break;
		*pus = 0x55AA;
		if (*pus != 0x55AA)
			break;
		*pus = save;
		//tail
		pus = (unsigned short*)((unsigned long)pus + grainSize - sizeof(unsigned short));
		save = *pus;
		*pus = 0xAA55;
		if (*pus != 0xAA55)
			break;
		*pus = 0x55AA;
		if (*pus != 0x55AA)
			break;
		*pus = save;
		//check next one
		pus = (unsigned short*)((unsigned long)pus + sizeof(unsigned short));
		position = (unsigned long)pus;
	}
	pMemSize = position - pMemStart;
	//print to screen
	myPrintk(0x7, "MemStart: %x  \n", pMemStart);
	myPrintk(0x7, "MemSize:  %x  \n", pMemSize);
	
}

extern unsigned long _end;
void pMemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}

	pMemHandler = dPartitionInit(pMemStart,pMemSize);	
}
