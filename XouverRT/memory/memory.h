#ifndef MEMORY_H
#define MEMORY_H

void initMem();
void* allocate(size_t size);
void* reallocate(void* ptr, size_t);
void deallocate(void* ptr);

void clearmemory();

#endif