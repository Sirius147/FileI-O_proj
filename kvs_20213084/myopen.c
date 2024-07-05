#include "kvs.h"


kvs_t* myopen()
{
	kvs_t* kvs = (kvs_t*) malloc (sizeof(kvs_t));

	if(kvs)
		kvs->items = 0;
	
	kvs->db = (node_t*)malloc(sizeof(node_t));
	(kvs->db)->next = (kvs->db);

	printf("num of items at kvs:%d\n", kvs->items);

	return kvs;
}
