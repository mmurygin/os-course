mmu: bin
	gcc -g3 -o0 memory-manager/main.c memory-manager/mmu.c memory-manager/memory-map.c -o ./bin/mmu

alloc: bin
	gcc -g3 -o0 allocator/main.c allocator/allocator.c -o ./bin/alloc

slab-allocator: bin
	gcc -g3 -o0 -lm slab-allocator/main.c slab-allocator/allocator.c slab-allocator/slab.c -o ./bin/sl-alloc

prioritizer: bin
	gcc -g3 -o0 prioritizer/main.c -o ./bin/prioritizer

rr-scheduler: bin
	g++ -std=c++11 round-robin-scheduler/main.cpp round-robin-scheduler/scheduler.cpp -o bin/scheduler

elfreader: bin
	g++ -std=c++11 elf-reader/main.cpp elf-reader/reader.cpp -o bin/elf-reader

bin:
	mkdir bin
