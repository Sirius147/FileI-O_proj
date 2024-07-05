#include "kvs.h"

int set(kvs_t* kvs, const char* key, const char* value)
{
	printf("set: %s, %s\n", key, value);
	
	// assign node
	node_t* curr = (node_t*)malloc(sizeof(node_t));
	// set key, value
	strcpy(curr->key, key);
	curr->value = (char *)malloc(sizeof(char)*4300);
	
	if (curr-> value == NULL) {
		return -1;
	}
	else{
		strcpy(curr->value, value);
	}
	// set items
	kvs->items += 1;
	node_t* prev = (kvs->db)->next;
	(kvs->db)->next = curr;
	((kvs->db)->next)->next = prev;
	/* do program here */
	return 0;
}
