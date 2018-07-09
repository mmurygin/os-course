mmu:
	gcc -g3 -o0 memory-manager/main.c memory-manager/mmu.c memory-manager/memory-map.c -o ./bin/mmu

alloc:
	gcc -g3 -o0 allocator/main.c allocator/allocator.c -o ./bin/alloc

slab-allocator:
	gcc -g3 -o0 slab-allocator/main.c slab-allocator/allocator.c -o ./bin/sl-alloc

planner:
	gcc simple-planner/main.c -o ./bin/planner

