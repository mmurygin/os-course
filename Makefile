mmu:
	gcc -g3 -o0 memory-manager/main.c memory-manager/mmu.c memory-manager/memory-map.c -o ./bin/mmu

alloc:
	gcc -g3 -o0 allocator/main.c allocator/allocator.c -o ./bin/alloc

slab-allocator:
	gcc -g3 -o0 slab-allocator/main.c slab-allocator/allocator.c -o ./bin/sl-alloc

planner:
	gcc -g3 -o0 simple-planner/main.c -o ./bin/planner

rr-scheduler:
	g++ -std=c++11 round-robin-scheduler/main.cpp round-robin-scheduler/scheduler.cpp -o bin/scheduler

