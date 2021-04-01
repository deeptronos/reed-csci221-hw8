CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++2a -O0 -g 
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)
all:  test_hforest test_htree 

test_htree: test_htree.o htree.o
	$(CXX) $(LDFLAGS) -o $@ $^
test_hforest:  htree.o test_hforest.o hforest.o
	$(CXX) $(LDFLAGS) -o $@ $^
%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<
clean:
	rm -rf *.o test_htree test_hforest
test: all
	./test_htree
	./test_hforest
