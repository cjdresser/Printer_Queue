//
// Created by cjdre on 2021-11-28.
//

#ifndef CDRESSE_PRINTER_QUEUE_DEFINITIONS_H
#define CDRESSE_PRINTER_QUEUE_DEFINITIONS_H

#define PAGESPERMINUTE 1
#define MAXCYCLES 200
#define MAXPAGES 30
#define ITERATIONS 1000
#ifdef DEBUG_ALL//option to define all debug options at once
#define DEBUG_PRINT_PROCESS
#define DEBUG_ADDING
#define DEBUG_SHOW_CYCLES 1
#define DEBUG_SHOW_EXCEEDED
#define DEBUG_LIST 2
#endif

//job definitions
typedef struct print_job{
    int doc_num;
    int priority;
    int pages;
    int cycles;
} PRINT_JOB;

int getPriority();

int getNumPages();

PRINT_JOB* newJobPtr(int);
//end job definitions

//list definitions
typedef struct node{
    void* dataPtr; // pointer to document structure
    struct node* next; // must be a singly linked list... so literally not a queue
} LIST_NODE;

typedef struct list{
    LIST_NODE* head;//items are removed at the head position
    LIST_NODE* tail;//items are added to tail
    int count;//number of items in list
} LIST;

LIST_NODE* newNodePtr(void*);

LIST* newListPtr();

LIST_NODE* dequeue(LIST*);

int addNodeOnPriority(LIST*, void*);

int finalAddNode(LIST*, void*);

int isEmpty(LIST*);

int evaluatePosition(LIST_NODE*, LIST_NODE*);

int repositionDoc(LIST*, int);

void printReverseList(LIST_NODE*, int);
//end list definitions


//printer definitions
void printDocument(int*);

void processDocument();

void printerLoop();

void incrementCycles(LIST*);

void freeListMemory(LIST*);

#endif //CDRESSE_PRINTER_QUEUE_DEFINITIONS_H
