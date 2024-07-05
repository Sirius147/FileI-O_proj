#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

struct node {
	char key[100];
	char* value;
	struct node* next;
};	
typedef struct node node_t;


struct kvs{
	struct node* db; // database
	int items; // number of data 
};
typedef struct kvs kvs_t; 


kvs_t* myopen();
int myclose(kvs_t* kvs); // free all memory space 
int set(kvs_t* kvs, const char* key, const char* value); // return -1 if failed.
char* get(kvs_t* kvs, const char* key); // return NULL if not found. 
void do_snapshot(kvs_t* kvs, const char* key, const char* value, long newLinePos);
void do_recovery(kvs_t* kvs);
kvs_t* custom_open();
void do_custom_recovery(kvs_t* kvs);
void do_custom_snapshot(kvs_t* kvs, const char* key, const char* value, long newLinePos);