all: leibnizSerial leibnizPosix leibnizFork

leibnizSerial: leibnizSerial.c
	gcc -o leibnizSerial leibnizSerial.c -lm

leibnizPosix: leibnizPosix.c
	gcc -o leibnizPosix leibnizPosix.c -lm -lpthread

leibnizFork: leibnizFork.c
	gcc -o leibnizFork leibnizFork.c -lm

clean:
	rm leibnizSerial leibnizPosix leibnizFork
