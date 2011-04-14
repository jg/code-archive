GTEST_PATH = /home/jg/.apps/files/gtest-1.4.0
CTESTSFLAGS = -I$(GTEST_PATH)/include -L$(GTEST_PATH)/lib -lgtest -Wall -g
VPATH = src:tests

OBJECTS = main.o 
CFLAGS = -Wall -g
CC = g++

all: src/main.cpp src/list.h src/block.h
	g++ src/main.cpp -o main -g
# all: $(OBJECTS)
# 	$(CC) $(CFLAGS) $(OBJECTS) -o main
# 
# $(OBJECTS): %.o: %.cpp %.h
# 	$(CC) -c $(CFLAGS) $< -o $@

check: 
	$(CC) $(CTESTSFLAGS) tests/mainTest.cpp src/list.h src/block.h -o test && LD_LIBRARY_PATH=$(GTEST_PATH)/lib ./test && rm test

clean:
	-rm -rf main
	-rm -rf *.o

