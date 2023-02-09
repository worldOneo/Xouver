#ifndef MEMORY_H
#define MEMORY_H

typedef struct S_MEMBLOCK_T {
	void* ptr;
} memblock;

class memorymanager {
private:
	void* rt;
	memblock* blocks;
	int bC;

	memblock* reallocBlocks(size_t nsize);
public:
	memorymanager(void* rt);
	void* allocate(size_t size);
	void deallocate(void* ptr);
	~memorymanager();
};

#endif