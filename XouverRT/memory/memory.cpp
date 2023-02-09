#include "memory.h"

#include <XNI.h>

#include <stdlib.h>
#include <runtime/runtime.h>

memorymanager::memorymanager(void* rt) {
	this->rt = rt;

	blocks = (memblock*)malloc(0);
	if (blocks == nullptr) ((runtime*)rt)->setError(XRT_Error::MEMORY_INITIALIZATION_FAILED);
	else bC = 0;
}

void* memorymanager::allocate(size_t size) {
	for (int i = 0; i < bC; i++) {
		if (blocks[i].ptr == nullptr) {
			blocks[bC].ptr = malloc(size);

			return blocks[i].ptr;
		}
	}

	memblock* nblocks = reallocBlocks(bC+1);

	if (nblocks != nullptr) {
		nblocks[bC-1] = {};
		nblocks[bC-1].ptr = malloc(size);

		if (nblocks[bC-1].ptr != nullptr) {
			free(blocks);
			blocks = nblocks;
			
			return blocks[bC-1].ptr;
		}
		else {
			free(nblocks);
		}
	}

	return nullptr;
}

memblock* memorymanager::reallocBlocks(size_t nsize) {
	memblock* nblocks = (memblock*)malloc(sizeof(memblock) * nsize);

	if (nblocks == nullptr) return nullptr;

	for (int i = 0; i < bC; i++) {
		nblocks[i] = blocks[i];
	}
	bC++;

	return nblocks;
}

void memorymanager::deallocate(void* ptr) {
	for (int i = 0; i < bC; i++) {
		if (blocks[i].ptr == ptr) {
			free(blocks[i].ptr);
			ptr = nullptr;
		}
	}
}

memorymanager::~memorymanager() {
	for (int i = 0; i < bC; i++) {
		free(blocks[i].ptr);
	}
}