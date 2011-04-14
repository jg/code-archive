GTEST_PATH = /home/jg/.apps/files/gtest-1.4.0
VPATH = src:tests/unit:include

OBJECTS = date.o regex.o schedule.o
TESTOBJS = date.o regex.o 

TESTS = mainTest.o dateTest.o regexTest.o

CFLAGS = -Wall -lboost_regex-mt -lboost_date_time-mt -g
# CTESTSFLAGS = -I$(GTEST_PATH)/include -Isrc -L$(GTEST_PATH)/lib -lgtest 
CTESTSFLAGS = -I$(GTEST_PATH)/include -L$(GTEST_PATH)/lib -lgtest -Wall -lboost_regex-mt -lboost_date_time-mt -g


all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o schedule
	cp schedule tests/system

$(OBJECTS): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@


check: 
	$(CC) $(CTESTSFLAGS) tests/unit/mainTest.cpp src/date.cpp src/regex.cpp -o test && LD_LIBRARY_PATH=$(GTEST_PATH)/lib ./test && rm test
	

clean:
	-rm -rf schedule $(OBJECTS)
	-rm -rf tests/system/schedule
	-rm -rf test

