/* Christian Dresser */
/* 28/11/2021 */
#include "header.h"

int getPriority() {
    int priorityChance = (rand() % (100)) + 1;
    if (priorityChance <= 10) {//10% chance of being high priority
        return 1;
    } else if ((priorityChance > 10) && (priorityChance <= 80)) {//70% chance of being average priority
        return 2;
    } else {//20% chance of being average priority
        return 3;
    }
}

int getNumPages(){
    int numPages = (rand() % (MAXPAGES)) + 1;//generate random number between 1 and MAXPAGES
    return numPages;
}


PRINT_JOB* newJobPtr(int document_number){
    PRINT_JOB* jobPtr;
    jobPtr = (PRINT_JOB *) malloc(sizeof(PRINT_JOB));
    jobPtr->cycles = 0;//cycles set to 0 because the job is new, therefore has not been in queue for any cycles
    jobPtr->doc_num = document_number;//set document #
    jobPtr->pages = getNumPages();//set number of pages
    jobPtr->priority = getPriority();//set priority
    return jobPtr;
}

