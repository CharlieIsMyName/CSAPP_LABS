#include "cachelab.h"
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct{
	int valid;
	long counter;		//for LRU implementation
	long tag;
} line;
typedef struct{
	long counter;		//for LRU implementation, the recently assigned line.couter in this set
	line *lines;
} set;

long hit=0,miss=0,evict=0,setNum,setMask,tagMask,s=-1,b=-1,e=-1;

void *Malloc(unsigned long bytes);	//wrapper function for libc malloc()
void parseFlag(char *path,int argc,char *argv[]);	//parse the options flags when running
set *cacheSetup();	//initialized the cache structure
void memOperation(set* head,unsigned long addr,char type);	//mimics "type" operation  of "address" on cache simlator "head"
char *readline(int fd);		//read only one line from the trace file and return it
void trace(char *path, set* head);	//load the trace file and trace it with the simulator

int main(int argc, char *argv[])
{
	char *path=Malloc(128);

    parseFlag(path,argc,argv);
    //calculating important constant
    setNum=1L<<s;
    setMask=(setNum-1)<<b;
    tagMask=~(setMask+((1L<<b)-1));
    printf("s: %ld,b: %ld,e: %ld,setNum: %ld, setMask: 0x%lx, tagMask: 0x%lx",s,b,e,setNum,setMask,tagMask);
    set *head=cacheSetup();
    trace(path,head);
    printSummary(hit,miss,evict);
    return 0;
}
void *Malloc(unsigned long bytes){
	void *p=malloc(bytes);
	if(p==NULL){
		printf("Malloc failed!");
		exit(-1);
	}
	return p;
}
void parseFlag(char *path,int argc,char *argv[]){
	char temp;
	while((temp=getopt(argc,argv,"s:b:E:t:"))!=-1){
		switch(temp){
			case 's':
				s=atoi(optarg);
				break;
			case 'b':
				b=atoi(optarg);
				break;
			case 'E':
				e=atoi(optarg);
				break;
			case 't':
				if(strlen(optarg)<=128){			//avoiding buffer overflow
					strcpy(path,optarg);
				}
				else{
					printf("Path is too long!");
					exit(-1);
				}
		}
	}
	if(s<0 || b<0 || e<0 || *path==0 ||s>63 || b>63){
		printf("Invalid or insuffcient input. s:%ld, b:%ld, E:%ld, path:%s\n",s,b,e,path);
		exit(-1);
	}
}
set *cacheSetup(){
	set *head=Malloc(setNum*sizeof(set));
	for(long i=0;i<setNum;i++){
		head[i].lines=Malloc(e*sizeof(line));
		head[i].counter=0;
		for(long j=0;j<e;j++){
			head[i].lines[j].valid=0;			//set all caches to cold cache
		}
	}
	return head;
}

void memOperation(set* head,unsigned long addr,char type){
	unsigned long addrtag=addr&tagMask;
	unsigned long addrsetno=(addr&setMask)>>b;		//the set number of this address
	set* s=&head[addrsetno];							//the current set we are working on
	line *lineArray=s->lines;
	line *LruLine=&lineArray[0];
	long leastCounter=LruLine->counter;
	for(int i=0;i<e;i++){
		if(lineArray[i].valid==0){				//if a cold cache line, take it!
			lineArray[i].valid=1;
			lineArray[i].tag=addrtag;
			lineArray[i].counter=s->counter++;
			if(type=='S' || type=='L'){
				miss++;
			}
			else{
				miss++;
				hit++;
			}
			return;
		}
		else if((lineArray[i].valid) && (lineArray[i].tag==addrtag)){	//a cache hit!
			lineArray[i].counter=s->counter++;
			if(type=='S' || type=='L'){
				hit++;
			}
			else{
				hit+=2;
			}
			return;
		}
		else{					//a conflict miss
			if(lineArray[i].counter<leastCounter){		//trying to get the line with the least counter(that is, LRU)
				LruLine=&lineArray[i];
				leastCounter=LruLine->counter;
			}
		}
	}
	LruLine->tag=addrtag;		//conflict miss eviction
	LruLine->counter=s->counter++;
	if(type=='S' || type=='L'){
		miss++;
		evict++;
	}
	else{
		miss++;
		hit++;
		evict++;
	}
}
char *readline(int fd){
	char *buffer=malloc(128);
	char *bufp=buffer;
	char c=0;
	for(int i=0;i<128;++i){
		int stat=read(fd,&c,1);
		if(c=='\n'){
			*bufp=0;
			return buffer;
		}
		else if(stat==0){
			return NULL;
		}
		else if(stat==-1){
			printf("read error occured!\n");
			exit(-1);
			return NULL;
		}
		*bufp=c;
		bufp++;
	}
	printf("why is this line so long?! something must be wrong!\n");
	exit(-1);
	return 0;
}
void trace(char *path, set* head){
	int fd=open(path,O_RDONLY,0);
	if(fd==-1){
		printf("read error!\n");
		exit(-1);
	}
	char *buf;
	char type;
	unsigned long addr;
	while((buf=readline(fd))){
		if(*buf==' '){
			buf++;
			type=*buf;
			buf+=2;
			addr=strtoul(buf,(char**)NULL,16);		//let it stop automatically
			memOperation(head,addr,type);
		}
	}
}
