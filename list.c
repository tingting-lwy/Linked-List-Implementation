#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <assert.h>


static List listHeads[LIST_MAX_NUM_HEADS];
static Node listNodes[LIST_MAX_NUM_NODES];

static int Headindex = 0; // index of next free head listHeads
static int Nodeindex = 0; // index of next free node in listNodes

static int freedHeadindex = LIST_MAX_NUM_HEADS+1; // initialize to an int that is not possible
static int freedNodeindex = LIST_MAX_NUM_NODES+1; // initialize to an int that is not possible
static int Headcounter = 0; // how many times a head has been freed;
static int Nodecounter =0; // how many times a node has been freed;



// Function to create new nodes -> call within List functions. Returns a pointer to that node. 
// Return NULL on failure
static Node * Node_create(void* itemptr)
{
	if(Nodeindex == LIST_MAX_NUM_NODES && Nodecounter == LIST_MAX_NUM_NODES) // fails, no free nodes
	{
		return NULL;
	}
	
	else if (Nodeindex == LIST_MAX_NUM_NODES && Nodecounter != LIST_MAX_NUM_NODES){ // a freed node available
		// Swapping freed node with first node in listNodes
		listNodes[freedNodeindex] =listNodes[0];
		listNodes[freedNodeindex].index = freedNodeindex;
		
		listNodes[0].item = itemptr; 
		listNodes[0].index = 0;
		listNodes[0].next = NULL;
		listNodes[0].prev = NULL;
		Nodecounter++;


		return &listNodes[0];
	}

	else{ // listNodes not empty -> available nodes
		listNodes[Nodeindex].item = itemptr; 
		listNodes[Nodeindex].index = Nodeindex;
		listNodes[Nodeindex].next = NULL;
		listNodes[Nodeindex].prev = NULL;
		Nodeindex++;
		Nodecounter++;
	}

	return &listNodes[Nodeindex-1];
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{	
	if(Headindex == LIST_MAX_NUM_HEADS && Headcounter == LIST_MAX_NUM_HEADS) // fails, no free heads
	{
		return NULL;
	}
	
	else if (Headindex == LIST_MAX_NUM_HEADS && Headcounter != LIST_MAX_NUM_HEADS){ // a freed head available

		//swap index 0 and freedHeadindex in listHeads
		listHeads[freedHeadindex] =listHeads[0];
		listHeads[freedHeadindex].index = freedHeadindex;

		listHeads[0].index = 0;
		listHeads[0].head = NULL;
		listHeads[0].tail = NULL;
		listHeads[0].current = NULL;
		listHeads[0].currLocation = 0;
		listHeads[0].numNodes = 0;
		Headcounter++;

		return &listHeads[0];
	}

	else{ // listHeads not empty = available nodes to use
		listHeads[Headindex].index = Headindex;
		listHeads[Headindex].head = NULL;
		listHeads[Headindex].tail = NULL;
		listHeads[Headindex].current = NULL;
		listHeads[Headindex].currLocation = 0;
		listHeads[0].numNodes = 0;
		Headindex++;
		Headcounter++;
	}

	return &listHeads[Headindex-1];
}

// // Returns the number of items in pList.
int List_count(List* pList)
{
	assert(pList != NULL);
	return pList->numNodes;
}

// // Returns a pointer to the first item in pList and makes the first item the current item.
// // Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList)
{
	assert(pList != NULL);

	if(pList->numNodes == 0){ // if empty list
		pList->current = NULL;
		pList->currLocation = 0;
		
		return NULL;
	}
	pList->current = pList->head; // makes current point to head
	pList->currLocation = 0; 
	
	return pList->current->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{
	assert(pList != NULL);

	if(pList->numNodes == 0){ // empty list
		pList->current = NULL;
		pList->currLocation = 0;
		
		return NULL;
	}

	pList->current= pList->tail; // makes current pointer to tail
	pList->currLocation = 0;
	
	return pList->tail->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
	assert(pList != NULL);

	if(pList->numNodes == 0 || pList->currLocation == 1){ // if list is empty or already after tail
		return NULL;
	}
	
	else if(pList->current == pList->tail){ //if current is at end of list
		pList->current = pList->tail->next;
		pList->currLocation = 1; // now current pointer points beyond tail
		
		return NULL;
	}

	else if(pList->currLocation == -1){ // if current is before head of list
		pList->current = pList->head;
		pList->currLocation = 0; // now current points within list
		
		return pList->current->item;
	}

	else{ // current within list
		pList->current = pList->current->next;

		return pList->current->item;
	}
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList)
{
	assert(pList != NULL);

	if(pList->numNodes ==0 || pList->currLocation == -1){ // if list is empty or already before list
		return NULL;
	}
	
	else if(pList->current == pList->head){ // if current is at start of list
		pList->current = pList->head->prev;
		pList->currLocation = -1; // now current pointer points before head
	
		return NULL;
	}
	else if(pList->currLocation == 1){ // if current is beyond tail
		pList->current = pList->tail;
		pList->currLocation = 0; // now current pointer is within list
		
		return pList->current->item;
	}

	else{ // current within list
		// Node* tmp = pList->current;
		// printf("t\n");
		pList->current = pList->current->prev;
		// printf("t\n");

		return pList->current->item;
	}
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList)
{
	assert(pList != NULL);

	if(pList->numNodes ==0 || pList->currLocation == 1 || pList->currLocation == -1){ //empty 
	//list or current pointer before or beyond list
		return NULL;
	}

	return pList->current->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem)
{	
	assert(pList != NULL);

	Node* pNode = Node_create(pItem); // create new node
	assert(pNode != NULL);
	
	if(pList->numNodes == 0){ // if list is empty
		pList->head = pNode;
		pList->current = pNode;
		pList->tail = pNode;
		pList->numNodes++;
		
		if(pList->current == pList->head && pList->head == pNode){ //check
			return 0;
		}

		return -1;
	}

	else if(pList->currLocation == -1){ // if current pointer is before head
		// add newnode at start
		pList->head->prev = pNode;
		pNode->next = pList->head;
		pList->current = pNode;
		pList->currLocation = 0;

		pList->head = pNode;
		pList->numNodes++;

		if(pList->head == pNode && pList->current == pNode){ //check
			return 0;
		}
		return -1;
	}

	else if(pList->currLocation == 1 || pList->current == pList->tail){ // if current pointer is after tail
	// or at the tail
		// add newnode at end
		pList->tail->next = pNode;
		pNode->prev = pList->tail;
		pList->current = pNode;
		pList->currLocation = 0;

		pList->tail = pNode;
		pList->numNodes++;

		if(pList->tail == pNode && pList->current == pNode){ //check
			return 0;
		}

		return -1;
	}
	
	else{ // current pointer within list or at head
		pNode->next = pList->current->next;
		pNode->prev = pList->current;
		pList->current->next = pNode;
		Node* tmp = pNode->next;
		tmp->prev = pNode;

		pList->current = pNode;
		pList->numNodes++;

		if(pList->current == pNode){ //check
			return 0;
		}

		return -1;
	}
	
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)
{
	assert(pList != NULL);

	Node* pNode = Node_create(pItem); // create new node
	assert(pNode !=NULL);
	
	if(pList->numNodes== 0){ // if list is empty
		pList->head = pNode;
		pList->current = pNode;
		pList->tail = pNode;
		pList->numNodes++;
		
		if(pList->current == pList->head && pList->head == pNode){ //check
			return 0;
		}

		return -1;
	}

	else if(pList->currLocation == -1 || pList->current == pList->head){ // if current pointer is before head 
	// or at the head
		// add newnode at start
		pList->head->prev = pNode;
		pNode->next = pList->head;
		pList->current = pNode;
		pList->currLocation = 0;

		pList->head = pNode;
		pList->numNodes++;

		if(pList->head == pNode && pList->current == pNode){ //check
			return 0;
		}
		return -1;
	}

	else if(pList->currLocation == 1){ // if current pointer is after tail
		// add newnode at end
		pList->tail->next = pNode;
		pNode->prev = pList->tail;
		pList->current = pNode;
		pList->currLocation = 0;

		pList->tail = pNode;
		pList->numNodes++;

		if(pList->tail == pNode && pList->current == pNode){ //check
			return 0;
		}

		return -1;
	}

	else{ // current pointer within list
		pNode->next = pList->current;
		pNode->prev = pList->current->prev;
		pList->current->prev = pNode;
		Node* tmp = pNode->prev;
		tmp->next = pNode;
		pList->current = pNode;
		pList->numNodes++;

		if(pList->current == pNode){ //check
			return 0;
		}

		return -1;
	}
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
	assert(pList != NULL);

	Node* pNode = Node_create(pItem); // create new node
	assert(pNode != NULL); 

	if(pList->numNodes == 0){ // if list is empty
		pList->head = pNode;
		pList->current = pNode;
		pList->tail = pNode;
		pList->numNodes++;

		if(pList->current == pList->tail && pList->tail == pNode){ //check
			return 0;
		}
		return -1;
	}

	else{
		// append node to tail
		pList->tail->next = pNode;
		pNode->prev = pList->tail;
		pList->current = pNode;

		pList->tail = pNode;
		pList->numNodes++;
		

		pList->currLocation = 0;

		if(pList->current == pNode && pList->tail == pNode){ // check if successfully appended.
			return 0;
		}
		else{
			return -1;
		}
	}
}

// // Adds item to the front of pList, and makes the new item the current one. 
// // Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
	assert(pList != NULL);

	Node* pNode = Node_create(pItem); // create new node
	assert(pNode != NULL); 

	if(pList->numNodes == 0){ // if list is empty
		pList->head = pNode;
		pList->current = pNode;
		pList->tail = pNode;
		pList->numNodes++;
		
		if(pList->current == pList->tail && pList->head == pNode){ //check
			return 0;
		}
		return -1;
	}

	else{
		// prepend node to head
		pList->head->prev = pNode;
		pNode->next = pList->head;
		pList->current = pNode;

		pList->head = pNode;
		pList->numNodes++;

		pList->currLocation = 0;

		if(pList->current == pNode && pList->head == pNode){ // check if successfully prepended.
			return 0;
		}
		else{
			return -1;
		}
	}
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
	if(pList->numNodes ==0 || pList->currLocation == -1 || pList->currLocation == 1){
		// empty list or the current pointer is before or beyond list
	return NULL;
	}
	
	else if(pList->numNodes == 1){ // only one node, becomes empty list
		assert(pList->current == pList->tail);
		void* storage = pList->tail->item;

		// freeing node
		pList->current->prev = NULL;
		pList->current->next = NULL;
		pList->numNodes--;

		freedNodeindex = pList->current->index;
		Nodecounter--;
		pList->current = NULL;

		return storage;
	}

	else if(pList->current == pList->head && pList->numNodes != 1){ // if current pointer is at head, return the head 
		pList->head = pList->current->next;

		pList->head->prev = NULL;

		void* storage = pList->current->item; // store item pointer
		pList->currLocation = 0;
		
		// freeing node
		pList->current->prev = NULL;
		pList->current->next = NULL;
		pList->numNodes--;
		freedNodeindex = pList->current->index;
		Nodecounter--;

		pList->current = pList->head;

		return storage;

	}
	else if(pList->current == pList->tail){ // if current pointer is at tail, return tail
		pList->tail = pList->current->prev;
		pList->tail->next = NULL;

		void* storage = pList->current->item; // store item pointer
		pList->currLocation = 1;

		// freeing node
		pList->current->next = NULL;
		pList->current->prev = NULL;
		pList->numNodes--;

		freedNodeindex = pList->current->index;
		Nodecounter--;

		return storage;
	}

	else{ // current within list (not head or tail)
		// freeing the node and remembering it's place in pool;
		Node* currNode = pList->current;
		void* storage = pList->current->item; // store item pointer

		Node* tmp = pList->current->prev;
		pList->current = pList->current->next;
		pList->current->prev = tmp;
		tmp->next = pList->current;

		//freeing node
		currNode->next = NULL;
		currNode->next = NULL;
		pList->numNodes--;
	
		freedNodeindex = currNode->index;
		Nodecounter--;

		return storage;
	}
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
	assert(pList1 != NULL);
	assert(pList2 != NULL);
	assert(pList2 != pList1);

	if(pList2->numNodes ==0){ // 2 empty lists OR list2 empty (both same)-> pList1 stays same, plist2 freed
		freedHeadindex = pList2->index;
		Headcounter--;
		pList2->head = NULL;
		pList2->tail = NULL;
		pList2->current = NULL;
	}

	else if(pList1->numNodes ==0 && pList2->numNodes > 0){ // list 1 is empty, list 2 is not-> list1 head 
		//becomes list2 and list2 is freed
		pList1->head = pList2->head;
		pList1->tail = pList2->tail;
		pList1->current = NULL;
		pList1->currLocation = -1;
		pList1->numNodes = pList2->numNodes;

		// free list 2
		freedHeadindex = pList2->index;
		Headcounter--;
		pList2->head = NULL;
		pList2->tail = NULL;
		pList2->current = NULL;
		pList2->numNodes = 0; // list 2 freed
	}

	else{ // both lists nonempty -> plist2 is freed
		List* tmp = pList2;
		pList1->tail->next = tmp->head;
		tmp->head->prev = pList1->tail;
		pList1->tail = tmp->tail;
		pList1->numNodes += pList2->numNodes;
		

		if(pList1->currLocation == 1){ // list 1 current pointer is beyond list tail
			pList1->current = tmp->head;
			pList1->currLocation = 0;
		}

		// free list 2
		freedHeadindex = pList2->index;
		Headcounter--;
		pList2->head = NULL;
		pList2->tail = NULL;
		pList2->current = NULL;
		pList2->numNodes = 0; // list 2 is empty
	}
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
// UPDATED: Changed function pointer type, May 19
void List_free(List* pList, FREE_FN pItemFreeFn)
{
	assert(pList != NULL);

	if(pList->numNodes == 0){ // empty list

		freedHeadindex = pList->index;
		Headcounter--;
		pList->head = NULL;
		pList->tail = NULL;
		pList->current = NULL;
	}
	else{
		List_first(pList); // current pointer at head

		while(pList->current != NULL){
			void* deleteNode = List_remove(pList);
			(*pItemFreeFn)(deleteNode);
		}

		freedHeadindex = pList->index;
		Headcounter--;
		pList->head = NULL;
		pList->tail = NULL;
	}
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
	assert(pList != NULL);

	if(pList->numNodes ==0){ // empty list
		return NULL;
	}

	if(pList->numNodes == 1){ // one node, becomes empty list
		assert(pList->current == pList->tail);
		void* storage = pList->tail->item;
		pList-> current = NULL;
		pList->head = NULL;
		pList->tail = NULL;
		pList->currLocation =0;
		pList->numNodes--;
		
		return storage;
	}
	void* trimmed = pList->tail->item;

	pList->tail = pList->tail->prev;
	pList->current = pList->tail;
	pList->tail->next = NULL;
	pList->currLocation=0;

	// freeing node
	pList->current->next = NULL;
	pList->current->prev = NULL;
	pList->numNodes--;

	freedNodeindex = pList->current->index;
	Nodecounter--;

	return trimmed;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// UPDATED: Added clarification of behaviour May 19
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
	assert(pList != NULL);

	if(pList->numNodes == 0 || pList->currLocation == 1){ //empty list or current beyond tail
		return NULL;
	}

	else if(pList->currLocation == -1){ //current is pointing before start
		pList->current = pList->head;

		while(pList->current !=NULL){ // iterate through list
			void* searchNode = pList->current->item;
			if ((*pComparator)(searchNode, pComparisonArg) == 1){ // found the item we want
				return searchNode;
			}
			pList->current = pList->current->next;
		}
		return NULL;
	}

	else{

		while(pList->current !=NULL){
			void* searchNode = pList->current->item;
			
			if ((*pComparator)(searchNode, pComparisonArg) == 1){ // found the item we want
				return searchNode;
			}

			pList->current = pList->current->next;
		}

	return NULL;
	}
}