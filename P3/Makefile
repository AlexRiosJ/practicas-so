all: leibnizSerial leibnizPosix leibnizFork leibnizClone

leibnizSerial: leibnizSerial.c
	gcc -o leibnizSerial leibnizSerial.c -lm

leibnizPosix: leibnizPosix.c
	gcc -o leibnizPosix leibnizPosix.c -lm -lpthread

leibnizFork: leibnizFork.c
	gcc -o leibnizFork leibnizFork.c -lm

leibnizClone: leibnizClone.c
	gcc -o leibnizClone leibnizClone.c -lm

clean:
	rm leibnizSerial leibnizPosix leibnizFork leibnizClone
