#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <string.h>

#define SIZE 1000

char Heap[SIZE];

typedef struct Metadata { 
    int size; 
    char mark; 
    struct Metadata *next; 
} Metadata;

Metadata *head_ptr;

void Initialize() {
    head_ptr = (Metadata *)(void *)Heap;
    head_ptr->size = (SIZE - sizeof(Metadata)); 
    head_ptr->mark = 'f'; 
    head_ptr->next = NULL;
}

int Allocate(int request_size) {
    Metadata *ptr = head_ptr;
    Metadata *prev = NULL;
    int index = 0;

    while (ptr != NULL && (ptr->mark == 'a' || ptr->size < request_size)) {
        prev = ptr;
        ptr = ptr->next;
        index++;
    }

    if (ptr == NULL) {
        return -1; // No block available
    } 

    if (ptr->size == request_size) {
        ptr->mark = 'a';
    } else {
        Metadata *new_block = (Metadata *)((char *)ptr + sizeof(Metadata) + request_size);
        new_block->size = ptr->size - (request_size + sizeof(Metadata));
        new_block->mark = 'f';
        new_block->next = ptr->next;

        ptr->size = request_size;
        ptr->mark = 'a';
        ptr->next = new_block;
    }
    return index; // Index of allocated block
}

void Merge() {
    Metadata *ptr = head_ptr;
    Metadata *next_ptr;

    while (ptr != NULL && ptr->next != NULL) {
        next_ptr = ptr->next;

        if (ptr->mark == 'f' && next_ptr->mark == 'f') {
            ptr->size += next_ptr->size + sizeof(Metadata);
            ptr->next = next_ptr->next;
            continue; // Stay on the same block
        }
        ptr = ptr->next;
    }
}

bool Free(int index) {
    Metadata *ptr = head_ptr;
    bool ret_val = false;

    while (index > 0 && ptr != NULL) {
        ptr = ptr->next;
        index--;
    }

    if (index == 0 && ptr != NULL && ptr->mark == 'a') {
        ptr->mark = 'f';
        ret_val = true;
        Merge(); // Merge only if deallocation happened
    }

    return ret_val;
}

void Display_Heap() {
    Metadata *ptr = head_ptr;
    printf("Heap Details:\n");
    printf(" Block Size \t Block status \t Block Address \t Block Index\n");

    int index = 0;
    while (ptr != NULL) {
        printf(" %d \t %c \t %p \t %d\n", ptr->size, ptr->mark, (void *)ptr, index);
        ptr = ptr->next;
        index++;
    }
}

int main() {
    Initialize();
    int flag = 0;

    while (flag == 0) {
        printf("Choose the function you want to perform:\n");
        printf("1. Allocate memory\n2. De-allocate memory\n3. Display heap elements\n4. Quit\n");
        printf("Enter: ");

        int n;
        scanf("%d", &n);

        if (n == 1) {
            int size;
            printf("Enter the size of block you want to allocate: ");
            scanf("%d", &size);
            int ret_val = Allocate(size);
            if (ret_val == -1) {
                printf("Failed to assign memory\n");
            } else {
                printf("Required memory is successfully assigned at index: %d\n", ret_val);
            }
        } else if (n == 2) {
            printf("Enter the index you want to free: ");
            int index;
            scanf("%d", &index);
            bool ret_val = Free(index);
            printf(ret_val ? "The de-allocation is done successfully\n" : "De-allocation has failed\n");
        } else if (n == 3) {
            Display_Heap();
            printf("\n");
        } else if (n == 4) {
            flag = 1;
        }
    }
    return 0;
}
