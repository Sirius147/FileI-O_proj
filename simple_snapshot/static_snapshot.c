#include "kvs.h"
#include <time.h>
int main()
{
	kvs_t* kvs = myopen();
	time_t start, end;
    double seconds;
	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

    FILE* op = fopen("kvs.txt","a+");
	if ( op == NULL){
		printf("failed to make \'kvs.txt\'. at main\n");
		return -1;
	}

    fseek(op,0,SEEK_END);   // set pos to end, it will 0 or not
    int currPos = ftell(op);

    char currChar;
    int offset = 1;
    long newLinePos = 0;
        // if recovery done -> currPos > 0
		// get newLinePos -> fp = newLinePos (recovery 이후 line부터 읽기)
    while(currPos > 0){
        fseek(op, -offset, SEEK_END);
        fscanf(op,"%c",&currChar);

        if (currChar == ',') {
            fscanf(op,"%ld\n",&newLinePos); // 현재 op가 newLinePos 정수의 시작부분을 가리키는 중, 해당 값 저장
            break;  
        }

        offset++;
    }
    fclose(op);

	FILE* fp = fopen("cluster004.trc","r");
	if ( fp == NULL) {
		printf(" failed to open \'cluster004.trc\'.\n ");
		return -1;
	}
	printf("file pointer loaded successfully\n");
	
    if(newLinePos!=0){
		printf("after recovery, new line ptr is:%ld\n",newLinePos);
        fseek(fp,newLinePos,SEEK_SET);  // set fp if recovery done
    }
	// format "%*[^,],%49[^,],%d" , "%3[^,],%49[^,],%d"
    
    char* key = malloc(sizeof(char)*100);
	char* value = malloc(sizeof(char)*4300);

	while(fscanf(fp,"%*[^,],%[^,],%s\n",key,value) == 2){
            // set and do snapshot
        newLinePos = ftell(fp);
        // set(kvs,key,value);
        if(set(kvs,key,value)!=0){
				printf("set failed.\n");
				return -1;
		}
		// Record the start time
    	time(&start);
        do_snapshot(kvs,key,value,newLinePos);
        time(&end);
		seconds += difftime(end, start);
    }
	printf("total snapshot time: %f seconds\n", seconds);
	free(key);
	free(value);

	fclose(fp);
	myclose(kvs);
	 
	
	return 0;
}
