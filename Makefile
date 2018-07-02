mmu:
	gcc -g3 -o0 memory-manager/main.c memory-manager/mmu.c memory-manager/memory-map.c -o ./bin/mmu

alloc:
	gcc -g3 -o0 allocator/main.c allocator/allocator.c -o ./bin/alloc
