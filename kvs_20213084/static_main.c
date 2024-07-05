#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MAXLINE 1024
int main(int argc, char** argv)
{
	int clientfd, port;
	struct sockaddr_in serveraddr;

	char* host;

	host = argv[1];
	port = atoi(argv[2]);

	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket\n");
		exit(1);
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short)port);
	serveraddr.sin_addr.s_addr = inet_addr(host);

	if(connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr))< 0){
		perror("connect\n");
		exit(1);
	}

	int fd,retval,fd2,retval2;
	if((fd = open("./net_data/cluster004_web.trc",O_RDONLY)) < 0){
        perror("failed to open \'cluster004.trc\'.\n");
        return -1;

    }
	if((fd2 = open("answer.dat",O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0){
        perror("failed to make \'answer.dat\'.\n");
        return -1;

    }
	printf("open answer.dat successfully.\n");
	char str_buf[4500];
	ssize_t n_reads,n_writes;
	int w_bytes, r_bytes;
	char* request = malloc(sizeof(char)*4500);
	char* response = malloc(sizeof(char)*4500);
	char newline = '\n';
	
	while((n_reads = read(fd,str_buf,sizeof(str_buf))) > 0){
		for(int i = 0; i< n_reads; i++){
			if(str_buf[i] == '\n'){
				strncpy(request,str_buf,i+1);
				request[i+1] = '\0';
				w_bytes = write(clientfd,request,strlen(request));
				if(w_bytes < strlen(request)){
					printf("failed to send whole msg\n");
				}
				r_bytes = read(clientfd,response,sizeof(char)*4500);
				if(r_bytes < 0){
					printf("error: read from connfd at client.\n");
					exit(1);
				}
				printf("read data from server ..\n");
				if((n_writes = write(fd2,response,strlen(response))) < 0){
					perror("error: write to answer.dat\n");
					exit(1);
				};
				write(fd2,&newline,1);
				lseek(fd, -n_reads + i + 1, SEEK_CUR);
				break;
			}
		}
		
		memset(response, '\0', sizeof(char)*4500);
		memset(request, '\0', sizeof(char)*4500);
	}
	close(clientfd);
	printf("connection closed ..\n");
	
	free(request);
	free(response);

	if((retval = close(fd)) < 0){
        perror("error from closing cluster004_web.trc\n");
        exit(1);
    }
	if((retval2 = close(fd2)) < 0){
        perror("error from closing answer.dat\n");
        exit(1);
    }
	
	return 0;
}
