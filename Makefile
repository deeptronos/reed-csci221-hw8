CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++2a -O0 -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

VPATH=src:inc

all:  test_bitio test_hforest test_huffman test_htree encoder decoder

test_bitio: bitio.o test_bitio.o
	$(CXX) $(LDFLAGS) -o $@ $^

test_htree: test_htree.o htree.o
	$(CXX) $(LDFLAGS) -o $@ $^

test_huffman: test_huffman.o huffman.o hforest.o htree.o
	$(CXX) $(LDFLAGS) -o $@ $^

test_hforest: test_hforest.o hforest.o
	$(CXX) $(LDFLAGS) -o $@ $^

encoder: encoder.o bitio.o huffman.o hforest.o htree.o
	$(CXX) $(LDFLAGS) -o $@ $^

decoder: decoder.o  bitio.o huffman.o hforest.o htree.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.gch *.o test_bitio encoder decoder test_hforest test_htree test_huffman

clean_o: # Use "make clean_o" to remove .gch and .o files, leaving output files.
	rm -rf *.gch *.o

clean_enc_dec: # Use to remove encoded and decoded files.
	rm -rf *.comp *.comp.plaintext

test: all
	./test_htree
	./test_hforest
	./test_huffman
	./test_bitio
