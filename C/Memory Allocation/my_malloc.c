#define _XOPEN_SOURCE 500 // needed for sbrk() on cslab

#include <unistd.h>

typedef struct freenode
{
    size_t size;
    struct freenode *next;
} freenode;

#define HEAP_CHUNK_SIZE 4096

//void *heap_begin = NULL;
//size_t available_bytes = 0;

// head node for the freelist
freenode *freelist = NULL;

/* allocate size bytes from the heap */
void *malloc(size_t size)
{
    // can't have less than 1 byte requested
    if (size < 1)
    {
        return NULL;
    }

    // add 8 bytes for bookkeeping
    size += 8;

    // 32 bytes is the minimum allocation
    if (size < 32)
    {
        size = 32;
    }

    // round up to the nearest 16-byte multiple
    else if (size%16 != 0)
    {
        size = ((size/16)+1)*16;
    }

    // if we have no memory, grab one chunk to start
    if (freelist == NULL)
    {
          void *heap_begin;
          heap_begin = sbrk(HEAP_CHUNK_SIZE);
          if (heap_begin == (void *)-1)
          {
              return NULL;
          }
 
          // skip the first 8 bytes so that we will return 16-byte aligned
          // addresses, after we put our 8 bytes of bookkeeping in front
          heap_begin += 8;
          freelist = (freenode *)heap_begin;
          freelist->size = HEAP_CHUNK_SIZE - 8;
          freelist->next = NULL;
    }

    // look for a freenode that's large enough for this request
    // have to track the previous node also for list manipulation later
    freenode *currentnode = freelist;
    freenode *previousnode = freelist;
    void *returned_ptr;
    while (currentnode != NULL)
    {
        if (currentnode->size >= size)
        {
            break;
        }
        previousnode = currentnode; 
        currentnode = currentnode->next;
    }
    
    // if there is no freenode that's large enough, allocate more memory
    if (currentnode == NULL)
    {
    // if the request is for more memory that we have, get enough to fulfill it
        
        int total_new_bytes = ((size/HEAP_CHUNK_SIZE)+1)*HEAP_CHUNK_SIZE;
        void *tmp = sbrk(total_new_bytes);
        if (tmp == (void *)-1)
        {
            return NULL;
        }
        tmp += 8;
        freenode *newnode = (freenode *)tmp;
        newnode->size = total_new_bytes-8;
        newnode->next = freelist;
        freelist = newnode;
        currentnode = freelist;
        previousnode = freelist;
    }     // return the front of this memory chunk to the user
    

    // here, should have a freenode with enough space for the request
    // - if there would be less than 32 bytes left, then return the entire chunk
    // - if there are remaining bytes, then break up the chunk into two pieces
    //     return the front of this memory chunk to the user
    //     and put the rest into the freelist 
    if ((currentnode->size -size) < 32)
    {
        returned_ptr = (void *)currentnode;
        if (currentnode == freelist)
        {
            freelist = currentnode->next;
        }
        else
        {       
            previousnode->next = currentnode->next;
        }

        size = currentnode->size;      
    }
    else
    {
        void * new_address = (void *)currentnode + size;
        freenode *newnode = (freenode *)new_address;
        newnode->size = currentnode->size - size;
        if (currentnode == freelist)
        {
            freelist = newnode;
            newnode->next = currentnode->next;
        }
        else
        {
            previousnode->next = newnode;
                   
            newnode->next = currentnode->next;
        }
    }

    // here, get the address for the chunk being returned to the user and return it
    currentnode->size = size;
    currentnode->next = NULL;
    returned_ptr = (void *) currentnode+ 8;
    return returned_ptr;
}

/* return a previously allocated memory chunk to the allocator */
void free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
   }

    // make a new freenode starting 8 bytes before the provided address
    freenode *new_node = (freenode *)(ptr-8);

    // the size is already in memory at the right location (ptr-8)

    // add this memory chunk back to the beginning of the freelist
    new_node->next = freelist;
    freelist = new_node;

    return;
}
