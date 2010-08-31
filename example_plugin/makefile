GCC = g++
OUTFILE = "../bin/plugins/example.so"

COMPILE_FLAGS = -c -O2 -fno-strict-aliasing -w -DLINUX

all:
	$(GCC) $(COMPILE_FLAGS) ./*.cpp
	$(GCC) -O2 -shared -o $(OUTFILE) *.o
	-rm -f *~ *.o

