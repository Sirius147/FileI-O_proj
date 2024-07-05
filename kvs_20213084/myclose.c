#include "kvs.h"

int myclose(kvs_t* kvs)
{
	node_t* curr = (kvs->db)->next;
	node_t* new;
	for(int i=0; i<(kvs->items);i++){
		free(curr->value);
		new = curr->next;
		free(curr);
		curr = new;
	}
	free(kvs->db);
	kvs->db = NULL;
	kvs->items = 0;
	free(kvs);
	/* do program */
	return 0;
}
