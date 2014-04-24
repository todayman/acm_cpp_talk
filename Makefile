CXX=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -g
#CXX=g++
#CXXFLAGS=-std=c++11 -g

#LD=clang++
LD=$(CXX)
LDFLAGS=-stdlib=libc++ -lc++abi

TARGETS=printf io raii

all: $(TARGETS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp
%: %.o
	$(LD) $(LDFLAGS) $*.o -o $*
#printf: printf.o
#	$(LD) $(LDFLAGS) printf.o -o printf

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
