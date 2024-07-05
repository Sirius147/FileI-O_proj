#include "kvs.h"
kvs_t* custom_open()
{
	kvs_t* kvs = (kvs_t*) malloc (sizeof(kvs_t));
    // init and assign kvs
    if(kvs)
		kvs->items = 0;

    kvs->db = (node_t*)malloc(sizeof(node_t));
	(kvs->db)->next = (kvs->db);

    do_custom_recovery(kvs);
    
	printf("number of items now: %d\n", kvs->items);

	return kvs;
}
