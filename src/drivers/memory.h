#ifndef MEMORY_H
#define MEMORY_H

void print_largest_block(struct MemMapInfo* mmi);
EFI_MEMORY_DESCRIPTOR* find_largest_block(struct MemMapInfo* mmi);
u64 init_heap(struct MemMapInfo* mmi);
#endif