#include "kvs.h"
#define MAXLINE 4500
#define LISTENQ 1024

int main(int argc, char** argv){
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[MAXLINE];

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket in server\n");
        exit(1);
    }

    port = atoi(argv[1]);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);

    if(bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1){
        perror("failed to bind.\n");
        return -1;
    }
    if(listen(listenfd, LISTENQ) <0){
        return -1;
    }
    printf("waiting for request...\n");
    clientlen = sizeof(clientaddr);

    if((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen)) <0){
        perror("accept\n");
        exit(-1);
    }
    char* caddrp = inet_ntoa(clientaddr.sin_addr);
    printf("connected to %s\n",caddrp);

   
    kvs_t* kvs = myopen();

	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

    int n_reads, n_writes;
    char* type = malloc(sizeof(char)*4);
    char* key = malloc(sizeof(char)*100);
	char* value = malloc(sizeof(char)*4500);
	int first_comma = 1;
    int first_comma_idx,second_comma_idx;
    while(1){
        n_reads = read(connfd, buf, MAXLINE);
        printf("server got the new text.%s\n",buf);
        for(int i = 0; i < n_reads; i++){
            if(buf[i] == ',' && first_comma == 1){
                first_comma = 0;
                first_comma_idx = i;
                strncpy(type, buf, i);
                type[i] = '\0';
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
                    break;
            }
        }
        if(strcmp(type, "set") == 0){
            int size = strlen(value);
            // Convert integer to a string
            char buffer[20];
            sprintf(buffer, "%d", size);
            if(set(kvs,key,value)!=0){
                printf("set failed.\n");
                return -1;
            }
            printf("buffer:%s\n",buffer);
            write(connfd, buffer, strlen(buffer));            
            printf("send value size: %d to client.. \n", size);
        }
        else{
            if(get(kvs,key) == NULL){
                char nll[20] = "-1";
                write(connfd,nll,strlen(nll));
            }
            else{
                char* res = malloc(sizeof(char)*4500);
                res = get(kvs,key);
                write(connfd, res, strlen(res));
                printf("send value to client.. \n");
                free(res);
            }
        }
        type[0] = '\0';
        key[0] = '\0';
        value[0] = '\0';
    }
    free(type);
    free(key);
    free(value);
    myclose(kvs);
    return 0;
}