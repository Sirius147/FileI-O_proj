#include "kvs.h"

void do_snapshot(kvs_t* kvs, const char* key, const char* value, long newLinePos){

    FILE* fp = fopen("kvs.txt","a");
	if ( fp == NULL){
		printf("failed to open \'kvs.txt\'. at do_snapshot");
		return;
	}
    
    fprintf(fp,"%s,%s,%ld\n",key,value,newLinePos);
    fclose(fp);
    return;

    
}