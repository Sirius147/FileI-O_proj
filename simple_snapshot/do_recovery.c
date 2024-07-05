#include "kvs.h"
#include <time.h>
void do_recovery(kvs_t* kvs){

    time_t start, end;
    double seconds;

    // Record the start time
    time(&start);
    // read kvs.txt if exist
    FILE* fp = fopen("kvs.txt","r");
    // if file does not exist -> first execution
	if ( fp == NULL){
		printf("failed to read \'kvs.txt\'. Maybe file does not exist yet.\n");
        return;
    }
    else{
        char* key = malloc(sizeof(char)*100);
	    char* value = malloc(sizeof(char)*4300);
        
        while(fscanf(fp,"%[^,],%[^,],%*d\n",key,value)==2)
        {
            // set(kvs,key,value);
            if(set(kvs,key,value)!=0){
				printf("set failed.\n");
				return;
			}
            printf("================================\n");
            printf(".... do_recovery is excuting now ....\n");
            printf("================================\n");
        }

    }
    printf("================================\n");
    printf(".... do_recovery is excuted successfully ....\n");
    printf("================================\n");

    // Record the end time
    time(&end);
    seconds = difftime(end, start);
    printf("recovery time: %f seconds\n", seconds);
    fclose(fp);
    return;
}