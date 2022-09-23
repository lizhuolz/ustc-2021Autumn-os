#include "../../include/myPrintk.h"
#include "../../include/mem.h"

//dPartition 是整个动态分区内存的数据结构
typedef struct dPartition{
	unsigned long size;
	unsigned long firstFreeStart; 
} dPartition;	//共占8个字节

#define dPartition_size ((unsigned long)0x8)

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
typedef struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, blongs to user
	};	                           
} EMB;	//共占8个字节

#define EMB_size ((unsigned long)0x8)

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}

unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	//本函数需要实现！！！
	/*功能：初始化内存。
	1.在地址start处，首先是要有dPartition结构体表示整个数据结构(也即句柄)。
	2.然后，一整块的EMB被分配（以后使用内存会逐渐拆分），在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB。
	3.返回start首地址(也即句柄)。
	注意有两个地方的大小问题：
		第一个是由于内存肯定要有一个EMB和一个dPartition，totalSize肯定要比这两个加起来大。
		第二个注意EMB的size属性不是totalsize，因为dPartition和EMB自身都需要要占空间。
	
	*/
	struct dPartition* pdp = (struct dPartition*)start;
	pdp->size = totalSize - dPartition_size;
	pdp->firstFreeStart = start + dPartition_size;
	struct EMB* pemb = (struct EMB*)(pdp->firstFreeStart);
	pemb->size = EMB_size;
	pemb->nextStart = start + totalSize;
	return start;

}

void dPartitionWalkByAddr(unsigned long dp){
	//本函数需要实现！！！
	/*功能：本函数遍历输出EMB 方便调试
	1.先打印dP的信息，可调用上面的showdPartition。
	2.然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息

	*/
	showdPartition((struct dPartition*)dp);
	struct EMB* pemb = (struct EMB*)(((struct dPartition*)dp)->firstFreeStart);
	
	while (pemb != 0){
		showEMB(pemb);
		pemb = (struct EMB*)pemb->nextStart;
	}

}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
**/

unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	//本函数需要实现！！！
	/*功能：分配一个空间
	1.使用firstfit的算法分配空间，当然也可以使用其他fit，不限制。
	2.成功分配返回首地址，不成功返回0
	3.从空闲内存块组成的链表中拿出一块供我们来分配空间(如果提供给分配空间的内存块空间大于size，我们还将把剩余部分放回链表中)，并维护相应的空闲链表以及句柄
	注意的地方：
		1.EMB类型的数据的存在本身就占用了一定的空间。

	*/
	//not enough
	if (size > pMemSize)
		return 0;
	//align 4
	if (size % 4)
		size = ((1 + (size >> 2)) << 2);
	//get emb
	struct EMB* pemb = (struct EMB*)(((struct dPartition*)dp)->firstFreeStart);
	while (pemb != 0)
	{	//mem enough to allocate,
		if (pemb->size + size <= pemb->nextStart - (unsigned long)pemb){
			pemb->size += size;
			//split idle block
			//remaining enough to place another emb, insert remaining to emb list 
			if (pemb->size + EMB_size <= pemb->nextStart - (unsigned long)pemb){//warning
				struct EMB* pemb1 = (struct EMB*)((unsigned long)pemb + pemb->size + EMB_size);
				pemb1->nextStart = pemb->nextStart;
				pemb->nextStart = (unsigned long)pemb1;
				pemb1->size = EMB_size;
			}
			return (unsigned long)pemb + EMB_size;
		}
		// not fit, check next
		pemb = (struct EMB*)pemb->nextStart;
	}
	return 0;
}

/*
 *r
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	//本函数需要实现！！！
	/*功能：释放一个空间
	1.按照对应的fit的算法释放空间
	2.注意检查要释放的start~end这个范围是否在dp有效分配范围内
		返回1 没问题
		返回0 error
	3.需要考虑两个空闲且相邻的内存块的合并
	*/
	//get dp
	struct dPartition* pdp = (struct dPartition*)dp;
	//error: dp exeed start
	if (start > dp + pdp->size)
		return 0;
	struct EMB* pemb = (struct EMB*)(pdp->firstFreeStart);
	struct EMB* pemb1 = 0;//predecessor of pemb, maybe null
	//find lower_bound emp of start
	while (pemb != 0 && (unsigned long)pemb + pemb->size < start){
		pemb1 = pemb;
		pemb = (struct EMB*)(pemb->nextStart);
	}
	//find emb of start, release
	if (pemb != 0){
		pemb->size = start - (unsigned long)pemb;
		//if all released, union two idle block
		if (pemb->size == sizeof(struct EMB)){
			//warning: pemb1 maybe null !!
			if (pemb1 != 0) {
				pemb1->size += pemb->nextStart - pemb1->nextStart;
				pemb1->nextStart = pemb->nextStart;
			}
			
		}
	}
	return 1;
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
