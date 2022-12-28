#include "memory.h"

#include <stdlib.h>

typedef struct block {
	void* ptr;
};

int pcount = 1;
block* nullBlock;
block** blocks;

bool isInit = false;

void initMem() {
	if (!isInit) {
		block* b = (block*)malloc(sizeof(block));
		b->ptr = malloc(sizeof(char));
		*((char*)b->ptr) = 0;

		blocks = (block**)malloc(sizeof(block*));
		blocks[0] = b;

		isInit = true;
	}
}

void* allocate(size_t size) {
	void* p = malloc(size);
	block* b = (block*)malloc(sizeof(block));

	b->ptr = p;

	blocks = (block**) realloc(b, sizeof(block*) * pcount+1);
	blocks[pcount++] = b;

	return p;
}

block* getBlock(void* ptr) {
	block* b = nullptr;

	for (int i = 0; i < pcount; i++)
		if ((blocks[i]->ptr == ptr))
			b = blocks[i];

	return b;
}

void* reallocate(void* ptr, size_t size) {
	block* b = getBlock(ptr);

	void* _ptr = realloc(ptr, size);
	b->ptr = _ptr;

	return _ptr;
}


void deallocate(void* ptr) {
	block* b = getBlock(ptr);
	if (b == nullptr) return;

	block** _blocks = (block**)malloc(sizeof(block*) * pcount--);

	for (int i = 0; i <= pcount; i++) {
		if (blocks[i] != b) {
			_blocks[i] = blocks[i];
		}
		else {
			free(blocks[i]->ptr);
			free(blocks[i]);
		}
	}

	free(blocks);
}

void clearmemory() {
	for (int i = 0; i < pcount; i++) {
		free(blocks[i]->ptr);
		free(blocks[i]);
	}

	isInit = false;
}