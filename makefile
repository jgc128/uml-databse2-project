CC=g++

CFLAGS=-std=c++11 -Wall -O3

PNAME=program

all:
	$(CC) $(CFLAGS) db2_project/*.cpp -o $(PNAME)	

clean:
	rm $(PNAME)
 
