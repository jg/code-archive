all : asd

asd : asd.cc test.cc
	g++ -O2 -D _SUNOS asd.cc test.cc /home/common/dyd/aisdi/hash/timer.cc -o asd 
	
del :
	rm asd
debug :
	g++ -g -D _SUNOS asd.cc test.cc /home/common/dyd/aisdi/hash/timer.cc -o asd_debug 
	gdb asd_debug

view:
	lynx /home/common/dyd/aisdi/hash/info/index.html
