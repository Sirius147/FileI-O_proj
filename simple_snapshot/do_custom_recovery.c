#include "kvs.h"
#include <time.h>
void do_custom_recovery(kvs_t* kvs){

    time_t start, end;
    double seconds;

    // Record the start time
    time(&start);
    // read kvs.txt if exist
    int fd,retval;
    // if file does not exist -> first execution
    if((fd = open("kvs_custom.txt",O_RDONLY)) < 0){
        perror("failed to read \'kvs.txt\'. Maybe file does not exist yet.\n");
        return;

    }
    else{
        char* key = malloc(sizeof(char)*100);
	    char* value = malloc(sizeof(char)*4300);
        char buf[4500];
        ssize_t n_reads;
        int first_comma = 1;
        int first_comma_idx;

        while((n_reads = read(fd,buf,sizeof(buf))) > 0)
        {
            for(int i = 0; i < n_reads; i++){
                if(buf[i] == ',' && (first_comma == 1)){
                    strncpy(key,buf,i);
                    key[i] = '\0';
                    first_comma = 0;
                    first_comma_idx = i;
                }
                else if(buf[i] == ',' && (first_comma!=1)){
                    strncpy(value, buf + (first_comma_idx+1), i-(first_comma_idx+1));
                    value[i-(first_comma_idx+1)] = '\0';
                    first_comma = 1;
                }
                else if(buf[i] == '\n'){
                    
                    // move cursor to next line
                    lseek(fd, -n_reads + i + 1, SEEK_CUR);
                    break;
                }
                
            }
            // set(kvs,key,value);
            if(set(kvs,key,value)!=0){
				printf("set failed.\n");
				return;
			}
            printf("================================\n");
            printf(".... do_recovery is excuting now ....\n");
            printf("================================\n");
            key[0] = '\0';
            value[0] = '\0';
        }

    }
    printf("================================\n");
    printf(".... do_recovery is excuted successfully ....\n");
    printf("================================\n");

    // Record the end time
    time(&end);
    seconds = difftime(end, start);
    printf("recovery time: %f seconds\n", seconds);
    if((retval = close(fd)) < 0){
        perror("error from closing kvs_custom.txt\n");
        exit(1);
    }
    return;
}