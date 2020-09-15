# Linked-List-Implementation

This is a linked list implementation done without dynamically allocating memory (ie using malloc). 
Instead, a set array of structs are available for storage of linked list heads and nodes according to the LIST_MAX_NUM_HEADS and LIST_MAX_NUM_NODES.
Upon filling up allocated space, previously stored lists or nodes will need to be deleted to free space.
