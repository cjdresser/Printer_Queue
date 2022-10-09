/* Christian Dresser */
/* 28/11/2021 */
#include "header.h"
static PRINT_JOB curJob;//declared print job as static so don't have to pass it in every time

void printDocument(int* DocInPrinter){//print document
    processDocument();//first process it
#ifdef DEBUG_PRINT_PROCESS//print debug message if defined
    printf("\n");
#endif
    curJob.pages = (curJob.pages) - PAGESPERMINUTE;//decrement pages by PAGESPERMINUTE
    if(curJob.pages == 0){
        //once no more pages to print, print completed message and set value of pointer
        // DocInPrinter to 0 as there is no longer a document in the printer
        printf("Print Job Completed - Document Number: %2d - Cycle Count: %2d\n", curJob.doc_num, curJob.cycles);
        *DocInPrinter = 0;
    }
}

void processDocument(){
#ifdef DEBUG_PRINT_PROCESS//print processing message if debug option is defined
    printf("PRINTING - DOCUMENT: %d PAGE: %d priority: %d\n", curJob.doc_num, curJob.pages, curJob.priority);
#endif
}

int newJob(){//determine if a new job should be created or not based on 10% chance
    int chance = (rand() % 100) + 1;
    if(chance <= 10){
        return 1;
    }
    else{
        return 0;
    }
}


void incrementCycles(LIST* list){//increment cycles of each node
    LIST_NODE* tmpNode = NULL;
    LIST_NODE* loopNode = list->tail;
    printReverseList(loopNode, 1);//first print debug messages and increment cycles
    while(loopNode != NULL){//traverse list to see if any have reached max cycles
        if((((PRINT_JOB*)loopNode->dataPtr)->cycles) == (MAXCYCLES)) {
            tmpNode = loopNode;//if one has reached max cycles, store it in tmpNode so it can be repositioned
            //Note: only one job can reach max cycles per cycle because only adding one job at a time
            loopNode = loopNode->next;
        }
        else{
            loopNode = loopNode->next;//keep traversing
        }
    }
    if(tmpNode != NULL){//if a node that reached max cycles was found, reposition it
        repositionDoc(list, ((PRINT_JOB*)tmpNode->dataPtr)->doc_num);
    }
#ifdef DEBUG_SHOW_CYCLES
    printf("\n");
#endif
}



void printerLoop(){//main loop of program
    LIST* list = newListPtr();//create list
    PRINT_JOB* addJob;//pointer that will be assigned the addresses of any new jobs created
    int DocInPrinter = 0;//1 if a document is in the printer, 0 if not
    int count = 0;
    for(int i = 0;i < ITERATIONS;i++){//main loop
        if(newJob()){//if newJob returns 1 (i.e. 10% of the time)
            addJob = newJobPtr((count)+1);//create a new job with doc number = to number of items in list + 1
            finalAddNode(list, addJob);//add the job to the print queue
            count++;//increment counter variable

#ifdef DEBUG_ADDING//print adding message if debug option defined
            printf("Adding to Queue - Doc: %3d NoPages: %2d Priority: %2d\n", addJob->doc_num, addJob->pages, addJob->priority);
#endif
        }

        if(((list->head) != NULL) && DocInPrinter == 0){
            //if the list isn't empty and no job is currently in printer,
            curJob = *((PRINT_JOB*)(dequeue(list))->dataPtr);//remove front of queue and send it to printer
            DocInPrinter = 1;//set DocInPrinter to 1 because there is now a document in printer
        }
        if(DocInPrinter == 1) {//if there is a document in printer
            //print the document currently in the printer and change DocInPrinter back to 0 when done with current document
            printDocument(&DocInPrinter);
        }
#ifdef DEBUG_LIST//print list if debug option defined
        if(list->count == 0){
            printf("EMPTY QUEUE - no print jobs currently in queue\n");
        }
        else {
            printf("Current Printer Queue Size: %2d\n", list->count);
            printReverseList(list->tail, 2);
            printf("END OF LIST\n\n");
        }
#endif
        incrementCycles(list);//increment cycle count for each node in list


    }//end of main loop

    printf("\nEnd of Program - * CHRISTIAN DRESSER *\n");//end statements
    printf("Number of printer jobs left in queue: %d\n", list->count);

    freeListMemory(list);//free memory for the list and its nodes

}
