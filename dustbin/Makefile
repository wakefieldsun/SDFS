#CC := g++
#CFLAGS := -g
#TARGET := test
#SRCS := $(wildcard *.cpp)
#OBJS := $(patsubst %cpp,%o,$(SRCS))
#
#all:$(TARGET)
#
#%.o:%.cpp
#	$(CC) $(CFLAGS) -C $<
#$(TARGET):$(OBJS)
#	$(CC) $(CFLAGS) -o $@ $^
#clean:
#	rm -rf $(TARGET) *.o
CC := g++
TARGET := test
CFLAGS := -g

$(TARGET):main.o Customer.o Observer.o Subject.o waiter.o
	$(CC) -o $(TARGET) main.o Customer.o Observer.o Subject.o waiter.o

main.o:main.cpp waiter.h Customer.h
	$(CC) -c main.cpp
Customer.o:Customer.cpp Customer.h Subject.h
	$(CC) -c Customer.cpp
waiter:waiter.cpp waiter.h Object.h
	$(CC) -c waiter.cpp
Subject:Subject.cpp Obverser.h
	$(CC) -c Subject.cpp
Observer:Observer.cpp
	$(CC) -c Observer.cpp
clear:
	rm -f *.o $(TARGET)
