#include "kvs.h"
#include <dlfcn.h>
int main(){
    
    void* handle;
    kvs_t* (*open)();
    int (*close)(kvs_t*);
    int (*set)(kvs_t*,const char*,const char*);
    char* (*get)(kvs_t*,const char*);
    char* error;
	///home/sirius/sp23f/kvs_lab
    //handle = dlopen("./libkvs.so",RTLD_LAZY);
	handle = dlopen("libkvs.so",RTLD_LAZY);
    if(!handle){
        fprintf(stderr,"%s\n",dlerror());
        exit(1);
    }
    open = (kvs_t* (*)())dlsym(handle,"open");
	if((error = dlerror()) != NULL){
        fprintf(stderr,"%s\n",error);
        exit(1);
    }
    close = (int (*)(kvs_t*))dlsym(handle, "close");
    if((error = dlerror()) != NULL){
        fprintf(stderr,"%s\n",error);
        exit(1);
    }
	set = (int (*)(kvs_t*,const char*,const char*))dlsym(handle,"set");
    if((error = dlerror()) != NULL){
        fprintf(stderr,"%s\n",error);
        exit(1);
    }
	get = (char* (*)(kvs_t*,const char*))dlsym(handle,"get");

    if((error = dlerror()) != NULL){
        fprintf(stderr,"%s\n",error);
        exit(1);
    }

    kvs_t* kvs = open();

	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

	// workload execution
	kvs->db = (node_t*)malloc(sizeof(node_t));
	(kvs->db)->next = (kvs->db);
	

	// 1) 	file read 
	FILE* fp = fopen("query.dat","r");
	if ( fp == NULL) {
		printf(" failed to open \'query.dat\'. ");
		return -1;
	}
	printf("file pointer loaded successfully\n");
	FILE* op = fopen("answer.dat","w");
	if ( op == NULL){
		printf("failed to make \'answer.dat\'.");
		return -1;
	}
	// 2) 	if set, insert (key, value) into kvs.
	// 		if get, search the key in kvs and return the value.
	//		Return -1 if the key is not found  

	// new idea -->> read one line at once
	// format "%*[^,],%49[^,],%d" , "%3[^,],%49[^,],%d"

	
	char* type = malloc(sizeof(char)*4);
	char* key = malloc(sizeof(char)*100);
	char* value = malloc(sizeof(char)*100);
	while(fscanf(fp,"%3[^,],%[^,],%s\n",type,key,value) == 3){
		if (strcmp(type,"set")==0){
			if(set(kvs,key,value)!=0){
				printf("set failed.\n");
				return -1;
			}
			
		}
		else {
			if(get(kvs,key)==NULL){
				fprintf(op,"%s\n", "-1");
			}
			else{
				char* res = malloc(sizeof(char)*100);
				res = get(kvs,key);
				fprintf(op,"%s\n",res);
				free(res);
			}
		}
	}
	free(type);
	free(key);
	free(value);

	fclose(fp);
	fclose(op);
	close(kvs);
	 

    if (dlclose(handle) < 0){
        fprintf(stderr,"%s\n",dlerror());
        exit(1);
    }
	
	return 0;
}    
