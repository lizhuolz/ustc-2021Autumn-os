#include "../../include/mem.h"

unsigned long malloc(unsigned long size){
	//本函数需要实现！！！

    //调用实现的dPartition或者是ePartition的alloc
    
    return dPartitionAlloc(pMemStart, size);
}

unsigned long free(unsigned long start){
	//本函数需要实现！！！

    //调用实现的dPartition或者是ePartition的free
    
    return dPartitionFree(pMemStart, start);
}


unsigned long kmalloc(unsigned long size)
{
    return dPartitionAlloc(pMemStart, size);
}

unsigned long kfree(unsigned long start)
{
    return dPartitionFree(pMemStart, start);
}