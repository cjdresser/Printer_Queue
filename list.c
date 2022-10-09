/* Christian Dresser */
/* 28/11/2021 */
#include "header.h"

LIST_NODE* newNodePtr(void* dataPtr){
    LIST_NODE* newNodePtr;//create new node pointer
    newNodePtr = (LIST_NODE*)malloc(sizeof(LIST_NODE));//allocate memory for the pointer

    newNodePtr->next = NULL;//set next to null because node is not in list yet
    newNodePtr->dataPtr = dataPtr;//set node data pointer to dataPtr

    return newNodePtr;//return pointer to the new node
}

LIST* newListPtr(){
    LIST* listPtr;//create new list pointer
    listPtr = (LIST*)malloc(sizeof(LIST));//allocate memory for the list

    listPtr->head = NULL;//set head to null initially
    listPtr->tail = NULL;//set tail to null initially
    listPtr->count = 0;//set count to 0

    return listPtr;//return pointer to the list
}

int addNodeOnPriority(LIST* listPtr, void* dataPtr){
    //four cases:
    //  list is empty
    //  new node should be in the head position (lower priority than every other node)
    //  new node should go in tail position (higher priority than every other node)
    //  new node should go somewhere in the middle
    LIST_NODE* newNodeP;//create pointer to new node
    newNodeP = newNodePtr(dataPtr);//create new node with data pointer

    if(listPtr->head == NULL){//if the list is empty
        listPtr->head = newNodeP;//assign head and tail to the new node
        listPtr->tail = newNodeP;
        (listPtr->count)++;//increment the list count
        return 1;//return 1 (i.e. node was successfully added)
    }
    else{
        LIST_NODE* curNode;//create node pointer to traverse list
        int newPriority = ((PRINT_JOB*)newNodeP->dataPtr)->priority;//create variable  for the new node's priority
        curNode = listPtr->tail;//start at the tail
        //first check if the new node should just go at the end of the queue
        if((newPriority >= (((PRINT_JOB*)curNode->dataPtr)->priority))){
            newNodeP->next = listPtr->tail;//if new priority >= tail node priority, at new node to tail
            listPtr->tail = newNodeP;//set tail to new node
            return 1;
        }
        else if((newPriority < (((PRINT_JOB*)listPtr->head->dataPtr)->priority))){//check if new node should go at front
            listPtr->head->next = newNodeP;//make head's next the new node
            listPtr->head = newNodeP;//set head to new node
            return 1;
        }
        else {
            for (curNode; curNode->next != NULL; curNode = curNode->next) {
                //if new node's priority is < current node's priority, but greater than or equal to the next node's priority,
                //then insert the new node in between them
                if ((newPriority < (((PRINT_JOB *) curNode->dataPtr)->priority)) &&
                    (newPriority >= (((PRINT_JOB *) curNode->next->dataPtr)->priority))) {
                    newNodeP->next = curNode->next;
                    curNode->next = newNodeP;
                }

            }
        }
        (listPtr->count)++;//increment count
        return 1;
    }
}


int finalAddNode(LIST* listPtr, void* dataPtr){
    //- four outcomes:
    //  - 1. list is empty, so new node is head and tail
    //  - 2. new node goes in the tail position
    //      - a. new node has greater priority than current tail OR
    //      - b. new node has equal priority but more pages than current tail
    //  - 3. new node goes in the head position
    //      - a. new node has lower priority than current head OR
    //      - b. new node has equal priority but less pages than current head
    //  - 4. new node goes somewhere in the middle (loop)
    //      - a. two outcomes for each check:
    //          - i.  node is inserted
    //          - ii. move onto next node

    LIST_NODE* newNodeP;
    newNodeP = newNodePtr(dataPtr);
    int newPriority = ((PRINT_JOB*)newNodeP->dataPtr)->priority;
    int newPages = ((PRINT_JOB*)newNodeP->dataPtr)->pages;
    LIST_NODE* curNode = listPtr->tail;

    //first outcome
    if(isEmpty(listPtr)){
        listPtr->head = newNodeP;
        listPtr->tail = newNodeP;
        (listPtr->count)++;
        return 1;
    }

    //second outcome
    else if((newPriority > (((PRINT_JOB*)curNode->dataPtr)->priority)) ||
            ((newPriority == (((PRINT_JOB*)curNode->dataPtr)->priority)) &&
             (newPages > (((PRINT_JOB*)curNode->dataPtr)->pages)))){
        newNodeP->next = listPtr->tail;
        listPtr->tail = newNodeP;
        (listPtr->count)++;
        return 1;
    }

    //third outcome
    else if((newPriority < (((PRINT_JOB*)listPtr->head->dataPtr)->priority)) ||
            ((newPriority == (((PRINT_JOB*)listPtr->head->dataPtr)->priority)) &&
             (newPages < (((PRINT_JOB*)listPtr->head->dataPtr)->pages)))){
        listPtr->head->next = newNodeP;
        listPtr->head = newNodeP;
        (listPtr->count)++;
        return 1;
    }
    //fourth outcome
    else{
        while(curNode->next != NULL){
            switch(evaluatePosition(newNodeP, curNode)){
                case 1://outcome 4.->a.->i.
                    newNodeP->next = curNode->next;
                    curNode->next = newNodeP;
                    (listPtr->count)++;
                    return 1;
                case 2://outcome 4.->a.->ii.
                    curNode = curNode->next;
                    break;
                default:
                    printf("Something went wrong");
                    return 0;
            }
        }
    }
}

int isEmpty(LIST* list){//check if list is empty
    if(list->head == NULL){
        return 1;//if list empty
    }
    else{
        return 0;//if list not empty
    }
}

int evaluatePosition(LIST_NODE* newNode, LIST_NODE* curNode){
    //return 1 if new node should be added after curNode, 2 if curNode should be curNode->next
    int newPriority = ((PRINT_JOB*)newNode->dataPtr)->priority;
    int curPriority = ((PRINT_JOB*)curNode->dataPtr)->priority;
    int nextPriority = ((PRINT_JOB*)curNode->next->dataPtr)->priority;
    int newPages = ((PRINT_JOB*)newNode->dataPtr)->pages;
    int nextPages = ((PRINT_JOB*)curNode->next->dataPtr)->pages;

    if(newPriority < curPriority){
        //inserting a 2 between a 1 and 3
        if(newPriority > nextPriority){
            return 1;
        }
        //newPriority less than current and next priority e.g. inserting priority 1 where cur priority and next priority are 2
        else if(newPriority < nextPriority){
            return 2;
        }
        else{//if newPriority == nextPriority
            if(newPages >= nextPages){//if new priority < current priority but >= than next priority, insert new node after current
                return 1;
            }
            else{//otherwise, move on
                return 2;
            }
        }
    }
    else{//if newPriority == curPriority
        if(newPriority > nextPriority){//node hasn't been inserted yet so if new > next priority, insert after current
            return 1;
        }
        else if(newPriority == nextPriority){//if new == current priority, check page counts
            if(newPages >= nextPages){//if new node has more or equal pages to next node, insert new node before next node
                return 1;
            }
            else{//otherwise, new node has less pages than next node so move on
                return 2;
            }
        }
        else{//newPriority < nextPriority->shouldn't happen
            return 2;
        }
    }
    return 0;

}

void printReverseList(LIST_NODE* listTail, int mode){
    //need to print list in reverse because I'm dumb and designed it as a queue initially
    if(!listTail){//stop condition, tail = NULL means list head has been reached
        return;
    }
    printReverseList(listTail->next, mode);//recursively print each node ahead of current node before printing current node
    //Because of way main loop is set up, need to handle debug statements and incrementing cycles here
    if(mode == 1){//mode 1 is for cycle incrementing, SHOW_CYCLES and SHOW_EXCEEDED debug options
        (((PRINT_JOB*)listTail->dataPtr)->cycles)++;
#ifdef DEBUG_SHOW_CYCLES
        printf("Increment Cycle - Document: %3d Pages: %2d Priority: %2d Cycle Count: %2d\n",
               ((PRINT_JOB*)listTail->dataPtr)->doc_num,
               ((PRINT_JOB*)listTail->dataPtr)->pages,
               ((PRINT_JOB*)listTail->dataPtr)->priority,
               ((PRINT_JOB*)listTail->dataPtr)->cycles);
#endif
#ifdef DEBUG_SHOW_EXCEEDED
        if((((PRINT_JOB*)listTail->dataPtr)->cycles) == (MAXCYCLES)) {
            printf("EXCEEDED CYCLE COUNT - Document: %3d Pages: %2d Priority: %2d Cycle Count: %2d\n",
                   (((PRINT_JOB *) listTail->dataPtr)->doc_num),
                   (((PRINT_JOB *) listTail->dataPtr)->pages),
                   (((PRINT_JOB *) listTail->dataPtr)->priority),
                   (((PRINT_JOB *) listTail->dataPtr)->cycles));
        }
#endif
    }
    else{//mode 2 is for printing the queue (used for LIST debug option)
        printf("Current Printer Queue: DocNum: %3d NumofPages: %2d PriorityLevel: %2d NumOfCycles: %2d\n",
               ((PRINT_JOB *) listTail->dataPtr)->doc_num,
               ((PRINT_JOB *) listTail->dataPtr)->pages,
               ((PRINT_JOB *) listTail->dataPtr)->priority,
               ((PRINT_JOB *) listTail->dataPtr)->cycles);
    }
}

LIST_NODE* dequeue(LIST* listPtr){//dequeue a node i.e. remove node from front of list
    if(listPtr->head == NULL){//if list is empty, print error message
        fprintf(stderr, "List is empty");
        return NULL;
    }
    LIST_NODE* returnNodePtr;//node that will be popped and returned
    returnNodePtr = listPtr->head;//we want to return the first node in the list so set return node to head
    if(listPtr->head == listPtr->tail){
        listPtr->head = NULL;//now set the lists head to null
        listPtr->tail = NULL;
	free(listPtr->head);
	free(listPtr->tail);
    }
    else{
        listPtr->head = NULL;//now set the lists head to null
	free(listPtr->head);
        LIST_NODE* loopPtr;//create node pointer to traverse list
        for(loopPtr = listPtr->tail;loopPtr->next != NULL;loopPtr = loopPtr->next){
            listPtr->head = loopPtr;//set the head equal to the current pointer
        }
        listPtr->head->next = NULL;//set new head next = NULL because head->next is always NULL
    }
    (listPtr->count)--;//decrement count because removing node
    return returnNodePtr;//return pointer to dequeued node
}

int repositionDoc(LIST* list, int docNum){
    //reposition a document is cycle count reaches MAXCYCLES by finding the node based
    // on doc number, change its priority to 0, and reinsert it
    LIST_NODE *curNode;
    if(((PRINT_JOB*)list->tail->dataPtr)->doc_num == docNum){//check if the node is in the tail position and remove it if so
        curNode = list->tail;
        list->tail = curNode->next;
        curNode->next = NULL;
        ((PRINT_JOB*)curNode->dataPtr)->priority = 0;
        (list->count)--;//adding node increments count so decrement count to not count node twice
        addNodeOnPriority(list, (PRINT_JOB*)curNode->dataPtr);//add node solely based on priority
        return 1;//successfully repositioned node
    }
    else {
        LIST_NODE* tmp;//tmp to hold removed node so next node doesn't get skipped when incrementing cycle count
        for (curNode = list->tail; curNode != NULL; curNode = curNode->next) {//traverse list until found node
            if ((((PRINT_JOB *) curNode->next->dataPtr)->doc_num) == docNum) {
                tmp = curNode->next;
                curNode->next = tmp->next;
                tmp->next = NULL;//set next to NULL to fully remove from list
                ((PRINT_JOB*)tmp->dataPtr)->priority = 0;//set its priority to 0
                addNodeOnPriority(list, (PRINT_JOB*)tmp->dataPtr);//reinsert based on priority
                (list->count)--;//decrement count to avoid counting it twice
                return 1;
            }
        }
        return 0;//return 0 if something went wrong
    }
}
