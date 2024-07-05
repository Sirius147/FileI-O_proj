#include "kvs.h"

void do_custom_snapshot(kvs_t* kvs, const char* key, const char* value, long newLinePos){

    int fd, retval;
    char longStr[sizeof(long) * 8 + 1];
    char comma = ',';
    char enter = '\n';
    // long 변수를 문자열로 변환하여 버퍼에 저장
    sprintf(longStr, "%ld", newLinePos);
    // key 와 value, newLinePos의 length check?
    ssize_t n_writes;
    if((fd = open("kvs_custom.txt",O_WRONLY | O_APPEND)) < 0){
        perror("failed to open \'kvs_custom.txt\'. at do_snapshot\n");
        return;

    }
    if((n_writes = write(fd,key, strlen(key))) < 0){
        perror("write at snapshot.\n");
        exit(1);
    }
    if((n_writes = write(fd,&comma,1)) < 0){
        perror("write at snapshot.\n");
        exit(1);
    }
    if((n_writes = write(fd,value,strlen(value))) < 0){
        perror("write at snapshot.\n");
        exit(1);
    }
    if((n_writes = write(fd,&comma,1)) < 0){
        perror("write at snapshot.\n");
        exit(1);
    }
    if((n_writes = write(fd,longStr,strlen(longStr))) < 0){
        perror("write at snapshot.\n");
        exit(1);
    }
    if((n_writes = write(fd,&enter,1)) < 0){
        perror("write at snapshot.\n");
        exit(1);
    }
    if((retval = close(fd)) < 0){
        perror("error from closing kvs_custom.txt at do_snapshot\n");
        exit(1);
    }
    return;

    
}