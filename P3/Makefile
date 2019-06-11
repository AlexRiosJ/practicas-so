all: leibnizSerial leibnizPosix leibnizFork leibnizClone

leibnizSerial: leibnizSerial.c
	gcc -o leibnizSerial leibnizSerial.c

leibnizPosix: leibnizPosix.c
	gcc -o leibnizPosix leibnizPosix.c -lpthread

leibnizFork: leibnizFork.c
	gcc -o leibnizFork leibnizFork.c

leibnizClone: leibnizClone.c
	gcc -o leibnizClone leibnizClone.c

clean:
	rm leibnizSerial leibnizPosix leibnizFork leibnizClone
