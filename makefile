CC=g++

CFLAGS=-std=c++11 -Wall

PNAME=program

all:
	$(CC) $(CFLAGS) db2_project/*.cpp -o $(PNAME)	

clean:
	rm $(PNAME)
 
