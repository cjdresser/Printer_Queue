/* Christian Dresser */
/* 28/11/2021 */
#include "header.h"

void freeListMemory(LIST* list){//free memory for each node in list and list itself
     LIST_NODE* curNode = list->tail;
     LIST_NODE* tmp;
     while(curNode != NULL){
	tmp = curNode;
	curNode = curNode->next;
	free(tmp);
     }
}
