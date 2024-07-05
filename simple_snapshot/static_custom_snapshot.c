#include "kvs.h"
#include <time.h>
int main(){
    kvs_t* kvs = custom_open();
	time_t start, end;
    double seconds;
	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

    int fd,retval;
    // if file does not exist -> first execution
    if((fd = open("kvs_custom.txt",O_RDWR | O_CREAT | O_APPEND, 0644)) < 0){
        perror("failed to make \'kvs_custom.txt\'.\n");
        return -1;

    }

    off_t currPos = lseek(fd, 0, SEEK_END);
    printf("curr Pos: %ld\n", currPos);

    char currChar;
    off_t offset = 1;

	char tmpBuf[100];
    long newLinePos = 0;
        // if recovery done -> currPos > 0
		// get newLinePos -> fp = newLinePos (recovery 이후 line부터 읽기)
    while(currPos > 0){
        lseek(fd, -offset, SEEK_END);
        read(fd,&currChar,1);

        if (currChar == ',') {

            read(fd, tmpBuf, offset-2);
			tmpBuf[offset-1] = '\0';
			newLinePos = strtol(tmpBuf,NULL,10);
            printf("newLinePos is %ld\n",newLinePos);
            break;  
        }

        offset++;
    }
    if((retval = close(fd)) < 0){
        perror("error from closing kvs_custom.txt\n");
        exit(1);
    }

	if((fd = open("cluster004.trc",O_RDONLY)) < 0){
        perror("failed to open \'cluster004.trc\'.\n");
        return -1;

    }
	printf("file pointer loaded successfully\n");
	
    if(newLinePos!=0){
		printf("after recovery, new line ptr is:%ld\n",newLinePos);
        lseek(fd,newLinePos,SEEK_SET);  // set fp if recovery done
    }
	// format "%*[^,],%49[^,],%d" , "%3[^,],%49[^,],%d"
    printf("The newLinePos is %ld\n",newLinePos);
    int a;
    printf("출력중지선:\n");
    scanf("%d",&a);

	char buf[4500];
    ssize_t n_reads;
    char* key = malloc(sizeof(char)*100);
	char* value = malloc(sizeof(char)*4300);
	int first_comma = 1;
    int first_comma_idx,second_comma_idx;
	//int cnt = 0;
	while((n_reads = read(fd,buf,sizeof(buf))) > 0){
            // set and do snapshot
        newLinePos = lseek(fd,0,SEEK_CUR);
		for(int i = 0; i < n_reads; i++){
                if(buf[i] == ',' && (first_comma == 1)){
                    first_comma = 0;
                    first_comma_idx = i;
                }
                else if(buf[i] == ',' && (first_comma!=1)){
                    strncpy(key, buf + (first_comma_idx+1), i-(first_comma_idx+1));
                    key[i-(first_comma_idx+1)] = '\0';
                    first_comma = 1;
					second_comma_idx = i;
                }
                else if(buf[i] == '\n'){
                    strncpy(value, buf + (second_comma_idx + 1), i - (second_comma_idx + 1));
					value[i - (second_comma_idx + 1)] = '\0';
                    // move cursor to next line
                    newLinePos = lseek(fd, -n_reads + i + 1, SEEK_CUR);
                    break;
                }
        }
        //cnt++;
        // set(kvs,key,value);
        if(set(kvs,key,value)!=0){
				printf("set failed.\n");
				return -1;
		}
		// Record the start time
    	time(&start);
        do_custom_snapshot(kvs,key,value,newLinePos);
        time(&end);
		seconds += difftime(end, start);
        //if(cnt == 50){break; }
        key[0] = '\0';
        value[0] = '\0';
    }
	printf("total snapshot time: %f seconds\n", seconds);
	free(key);
	free(value);

	if((retval = close(fd)) < 0){
        perror("error from closing cluster004.trc\n");
        exit(1);
    }
	myclose(kvs);
	 
	
	return 0;
}