#include <fstream>
#include "huffman.hh"
#include "htree.hh"
#include "bitio.hh"

///////////////////////////////////////////////////////////////////////////////
// Function that outputs a sequence of bits to ostream as defined in BitIO obj
void output_bits_t(std::vector<bool> bits, BitIO& out) {
    for(bool bit : bits) {
        out.output_bit(bit);
    }
}

int main(int argc, char* argv[]) {
    // Ensure that filename was given and that file exists (and open said file)
    if(argc != 2) {
        throw std::logic_error("Invalid number of arguments.");
    }
    std::ifstream source_file;
    std::string source_filename = argv[1];
    // A bit redundant...
    try {
        source_file.open(argv[1]);
        if (!source_file){
            throw std::logic_error("File not found.");
        }
    }
    catch (...) {
        throw std::logic_error("File not found.");
    }

    /////////////////// Encoding /////////////////////////////
    Huffman encoder;
    std::string encoded_filename = source_filename.append(".comp");
    std::ofstream encoded_file (encoded_filename);
    BitIO outp(&encoded_file, nullptr);

    while(source_file.eof() == false) {
        auto c = source_file.get(); // Get character
        auto bits = encoder.encode(c); // Get bits_t sequence of encoded char c
        output_bits_t(bits, outp); // Output bits_t sequence to encoded_file
    }
    auto bits = encoder.encode(Huffman::HEOF);
    output_bits_t(bits, outp);
    outp.~BitIO();
    
    source_file.close();
    encoded_file.close();
    
}