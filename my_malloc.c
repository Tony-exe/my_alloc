#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef struct block{
	char free;
	size_t size;
	struct block *next;
}node;

node* first;

node* searchFreeNode(size_t size){
	node* cur = first;
	while(cur != NULL && (!cur->free || cur->size<size + sizeof(node))){
		cur = cur->next;
	}
	return cur;
}

void* allocMemory(node* free_node, size_t size){
	size_t remaining = free_node->size-size;
	if(remaining>size + sizeof(node)){
		node* new_node = (node*) ((void*)free_node + sizeof(node) + size);
		new_node->free = 1;
		new_node->size = remaining - sizeof(node);
		new_node->next = free_node->next;
		free_node->next = new_node;
		free_node->size = size;
		free_node->free = 0; 
	}
	return (void*)free_node + sizeof(node);
}	

void* myAlloc(size_t size){
	node* free_node = searchFreeNode(size);
	return allocMemory(free_node, size);
}

void freeMemory(void* ptr){
	if(ptr == NULL) return;
	node* to_free = (node*) (ptr - sizeof(node));
	to_free->free = 1;
}

void printMemory(){
	node* cur = first;
	int i = 0;
	printf("-----------------------------------------------\n");
	while(cur != NULL){
		printf("Area: %d\t Free: %d\t Size: %lu\n", i, cur->free, cur->size);
		i++;
		cur = cur->next;
	}
	printf("-----------------------------------------------\n\n");
}

int main(int argc, char *argv[])
{
	if(argc == 2){
		size_t tot_mem = 1024 * atoi(argv[1]);
		void* mem = mmap(NULL, tot_mem, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		first = (node*) mem;

		first->free = 1;
		first->size = tot_mem - sizeof(node);
		first->next = NULL;

		printMemory();
		void* ptr1 = myAlloc(10);
		void* ptr2 = myAlloc(20);
		void* ptr3 = myAlloc(30);

		printMemory();

		freeMemory(ptr3);

		printMemory();

	}else{
		fprintf(stderr, "Max one argument\n");
	}
	return 0;
}