/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "one Man Army",
    /* First member's full name */
    "Charlie Wang",
    /* First member's email address */
    "edyzhagao@163.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define DWORD 16      //minimum block size is a double word
#define CHUNK (1<<12)

//basic pointer operations
//always assuming p points to the second word in the block(that is, payload if allocated, *prev if freed), call it block pointer.
#define MAX(a,b) a>b?a:b
#define MIN(a,b) a<b?a:b
#define PACK(size,alloc) (size|alloc)
#define GET(p) (*(unsigned*)p)
#define PUT(p,word) ((*(unsigned*)p)=((unsigned)word))

#define GETSIZE(p) (GET(p)&(~0x7))
#define GETALLOC(p) (GET(p)&(0x7))

//the following p must be a block pointer! the result is a pointer!
#define HDP(p) ((char*)p-4)
#define FTP(p) ((char*)p-8+GETSIZE(HDP(p)))

#define NXBLKP(p) ((char*)p+GETSIZE(HDP(p)))
#define PRBLKP(p) ((char*)p-((*((unsigned *)p-2))&(~7)))

//explicit free list operations! p must be a block pointer, result is also a block pointer
#define LISTPREV(p) ((unsigned*)*((unsigned *)p+1))
#define LISTNEXT(p) ((unsigned*)(GET(p)))
/*
 * mm_init - initialize the malloc package.
 */
void *list;    //entry for the free list
unsigned listnum=21; //total 21 entries is needed for storing the list entries for 20MB

unsigned ulog2(unsigned x);
void *findEntry(unsigned size);
void listAdd(void *p);
void listDel(void *p);
void printList();
void *extendHeap(unsigned size);
void place(void *p,unsigned size);
void *coalesce(void *p);
void *findFit(unsigned size);
unsigned isEnd(void* p);

int mm_init(void)
{
    mem_init();
    //initializing the explicit free list
    list=mem_sbrk(listnum*4);
    for(int i=0;i<listnum;i++){
        unsigned *temp=(unsigned *)list+i;
        PUT(temp,NULL);
    }
    //aligning to 8
    void* temp=mem_sbrk(0);
    mem_sbrk(8-((unsigned)temp%8));
    mem_sbrk(4);
    //put the prologue block
    temp=mem_sbrk(8);
    PUT(temp,PACK(8,3));
    //put the epilogue block header
    temp=mem_sbrk(4);
    PUT(temp,PACK(0,3));
    return 0;
}


/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 *
 *      using unsigned instead of size_t(unsigned long) since it is in 32bit mode just for less work and thinking
 */
void *mm_malloc(size_t size){
    if(size==0){
        return NULL;
    }
    size+=4;
    if(size<16){                //size computation
        size=16;
    }
    else{
        size=(size/16+1)*16;
    }
    void *p=NULL;
    if((p=findFit(size))==NULL){       //if no entry fits then extend the heap
        p=extendHeap(MAX(CHUNK,size));
    }
    place(p,size);
    return p;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr){
    unsigned header=*((unsigned *)ptr-1);
    //free the block
    header&=(~1);
    PUT(HDP(ptr),header);
    PUT(FTP(ptr),header);
    listAdd(ptr);
    //change the prev alloc bit in the next block
    (*HDP(NXBLKP(ptr)))&=(~2);
    if(((GETALLOC(NXBLKP(ptr))&1)==0)){
        (*FTP(NXBLKP(ptr)))&=(~2);
    }
    coalesce(ptr);
}



/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size){
    //special cases
    if(ptr==NULL){
        return mm_malloc(size);
    }
    if(size==0){
        mm_free(ptr);
        return ptr;
    }

    //normal cases
    size=(size/16+1)*16;  //round up to the nearest multiples of 16
    unsigned oldsize=GETSIZE(HDP(ptr));
    if(size<oldsize){     //if the new block is smalletpor than the orginal one, truncate it and make a new free block after it
        unsigned header=PACK(size,GETALLOC(HDP(ptr)));
        unsigned nextHeader=PACK((oldsize-size),2);
        //realloc the block
        PUT(HDP(ptr),header);
        //create the free block left and add it to list
        PUT(HDP(NXBLKP(ptr)),nextHeader);
        PUT(FTP(NXBLKP(ptr)),nextHeader);
        //set alloc of the next next block
        *HDP(NXBLKP(NXBLKP(ptr)))&=(~2);
        if((GETALLOC(HDP(NXBLKP(NXBLKP(ptr))))&1)==0){    //if that block is free, set its footer too
            *FTP(NXBLKP(NXBLKP(ptr)))&=(~2);
        }
        listAdd(NXBLKP(ptr));
        coalesce(NXBLKP(ptr));
        return ptr;
    }
    else if(size==oldsize){       //if same size, no operation is needed
        return ptr;
    }
    else{
        if(isEnd(ptr)){         //if it is the last element on the top of the stack, extend the heap and append a new block to the end of that element
            int sizediff=size-oldsize;
            assert(sizediff%16==0);
            mem_sbrk(sizediff);
            unsigned header=PACK(size,GETALLOC(HDP(ptr)));
            //set up the feee block
            PUT(HDP(ptr),header);
            //set up the epilogue block
            PUT(HDP(NXBLKP(ptr)),3);
            //remove it from the list
            return ptr;
        }
        else{
            assert(size>=16);
            unsigned *ans=mem_sbrk(size);
            unsigned header=((*(ans-1))&2)|(size)|1;
            //set up the feee block
            PUT(HDP(ans),header);
            //set up the epilogue block
            PUT(HDP(NXBLKP(ans)),1);
            memcpy(ans,ptr,GETSIZE(HDP(ptr))-4);
            mm_free(ptr);
            return ans;
        }
    }
}
void *extendHeap(unsigned size){   //extend the heap by size, add to the list, and return th=-e block pointer
    assert(size>=16);
    unsigned *ptr=mem_sbrk(size);
    if(ptr==(void *)-1){
        printf("extendHeap error!\n");
        exit(0);
    }
    unsigned header=((*(ptr-1))&2)|(size);
    //set up the feee block
    PUT(HDP(ptr),header);
    PUT(FTP(ptr),header);
    listAdd(ptr);
    //set up the epilogue block
    PUT(HDP(NXBLKP(ptr)),1);
    return coalesce(ptr);
}
void place(void *p,unsigned size){ //place a block with "size" size at block pointer p
    assert(size>=16);
    unsigned osize=GETSIZE(HDP(p));
    assert(osize>=size);
    listDel(p);
    unsigned header=size|(GETALLOC(HDP(p))|1);  //pack the header
    PUT(HDP(p),header);
    PUT(FTP(p),header);
    if(size!=osize){    //if there is space left
        header=(osize-size)|2;
        void *nxp=NXBLKP(p);
        PUT(HDP(nxp),header);
        PUT(FTP(nxp),header);
        listAdd(nxp);
    }
    else{
        GET(HDP(NXBLKP(p)))|=2;
    }
}
void *coalesce(void *p){
    unsigned prevAlloc=GETALLOC(HDP(p))&3,nextAlloc=GETALLOC(HDP(NXBLKP(p)))&3;     //prev--the one for p, next-- the next block
    assert((prevAlloc&1)!=1);
    if((prevAlloc&2)&&(nextAlloc&1)){       //4 cases
        return p;
    }
    else if((!(prevAlloc&2))&&(nextAlloc&1)){     //previous block free
        void *pre=PRBLKP(p);
        listDel(p);                                 //! WARNING! whenever a free block is created it should be add to list immediately!
        listDel(pre);                               //! all free blocks are assumed to be in the list when coalescing happens!
        unsigned header=GETSIZE(HDP(p))+GETSIZE(HDP(pre));
        header=PACK(header,GETALLOC(HDP(pre)));
        PUT(HDP(pre),header);
        PUT(FTP(pre),header);
        listAdd(pre);
        return pre;
    }
    else if((prevAlloc&2)&&!(nextAlloc&1)){
        void *next=NXBLKP(p);
        listDel(p);
        listDel(next);
        unsigned header=GETSIZE(HDP(p))+GETSIZE(HDP(next));
        header=PACK(header,prevAlloc);
        PUT(HDP(p),header);
        PUT(FTP(p),header);
        listAdd(p);
        return p;
    }
    else{
        void *pre=PRBLKP(p),*next=NXBLKP(p);
        listDel(pre);
        listDel(p);
        listDel(next);
        unsigned header=GETSIZE(HDP(p))+GETSIZE(HDP(pre))+GETSIZE(HDP(next));
        header=PACK(header,GETALLOC(HDP(pre)));
        PUT(HDP(pre),header);
        PUT(FTP(pre),header);
        listAdd(pre);
        return pre;
    }
}
void *findFit(unsigned size){   //find a block that fits in the explicit list
    unsigned *entry=findEntry(size);
    for(;entry<((unsigned *)list+listnum);entry++){    //first-fit algorithm
        unsigned *p=(unsigned *)*entry;
        while(p!=NULL){
            if(GETSIZE(HDP(p))>=size){
                return p;
            }
            p=(unsigned *)*p;
        }
    }
    return NULL;
}
void *findEntry(unsigned size){ //find the corresponding explicit free list entry with a given size
    unsigned *listEntry=list;
    listEntry+=ulog2(size)-4;
    return listEntry;
}

void listAdd(void *p){      //does not change alloc! LIFO list
    unsigned size=GETSIZE(HDP(p));
    assert(size>=16);
    unsigned *entry=findEntry(size);
    unsigned *firstBlock=(unsigned *)*entry;    //entry.next
    //start linking
    PUT(entry,p);
    PUT(p,firstBlock);
    if(firstBlock!=NULL){
        firstBlock++;
        PUT(firstBlock,p);
    }
    p+=4;
    PUT(p,entry);
}

void listDel(void *p){
    unsigned *ptr=p;
    unsigned *next=(unsigned *)*ptr;
    unsigned *prev=(unsigned *)*(ptr+1);
    PUT(prev,next);
    if(next!=NULL){
        next++;
        PUT(next,prev);
    }
}

unsigned ulog2(unsigned x) {		// my ilog2 from the previous datalab, O(1) efficiency
    assert(x>=16);
    x=MIN(x,20*(1<<20));
	unsigned mask1=(0x55<<8)+0x55;
	unsigned mask2=(0x33<<8)+0x33;
	unsigned mask3=(0x0f<<8)+0x0f;
	unsigned mask4=(0xff<<16)+0xff;		//mask4=0x00ff00ff
	unsigned mask5=(0xff<<8)+0xff;		//mask5=0x0000ffff
	unsigned result=x;
	mask1=(mask1<<16)+mask1;			//mask1=0x55555555 01010101
	mask2=(mask2<<16)+mask2;			//mask2=0x33333333 00110011
	mask3=(mask3<<16)+mask3;			//mask3=0x0f0f0f0f 00001111
	result|=result>>16;
	result|=result>>8;
	result|=result>>4;
	result|=result>>2;
	result|=result>>1;              //turned everthing after the first one into one
	result=((result>>1)&mask1)+(result&mask1);      //count the number of ones in the num
	result=((result>>2)&mask2)+(result&mask2);
	result=((result>>4)&mask3)+(result&mask3);
	result=((result>>8)&mask4)+(result&mask4);
	result=((result>>16)&mask5)+(result&mask5);
	return (result&0x3f)+(~0);
}

void printList(){
    printf("-------------------------\n");
    for(int i=0;i<21;i++){
        unsigned *p=(unsigned *)*((unsigned *)list+i);
        printf("i=%u:\n",i);
        while(p!=NULL){
            printf("%u\n",GETSIZE(HDP(p)));
            p=(unsigned *)*p;
        }
    }
}

unsigned isEnd(void *p){       //return 1 if this block is the end of the list (except the epilogue block), else return 0
    if(GETSIZE(HDP(NXBLKP(p)))==0){
        return 1;
    }
    return 0;
}










