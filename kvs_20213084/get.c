#include "kvs.h"

char* get(kvs_t* kvs, const char* key)
{
	/* do program here */

	char* value = (char*)malloc(sizeof(char)*4500);

	if(!value){
		printf("Failed to malloc\n");
		return NULL;
	}

	node_t* curr = (kvs->db)->next;
	for(int i = 0; i< (kvs->items); i++){
		if( strcmp(curr->key,key) == 0){
			strcpy(value,curr->value);
			return value;
		}
		else{
			curr = curr->next;
		}
	}

	// strcpy(value, "deadbeaf");
	free(value);
	return NULL;

}
