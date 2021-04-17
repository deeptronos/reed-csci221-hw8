#include <fstream>
#include "../inc/huffman.hh"
#include "../inc/bitio.hh"

///////////////////////////////////////////////////////////////////////////////
// Function that outputs a sequence of bits to ostream as defined in BitIO obj
void output_bits_t(std::vector<bool> bits, BitIO& out) {
    for(bool bit : bits) {
        out.output_bit(bit);
    }
}

///////////////////////////////////////////////////////////////////////////////
// The main function performs the encoding.
int main(int argc, char* argv[]) {
    // Ensure that filename was given
    if(argc != 2) { throw std::logic_error("Invalid number of arguments. Correct usage: ./<exe> <filename>"); }

    std::ifstream source_file;
    std::string source_filename = argv[1];

    // Open file and throw an error if the file doesn't exist.
    source_file.open(argv[1]);
    if (!source_file) { throw std::logic_error("File not found."); }

    // Declare / initialize necessary objects, files, and streams
    Huffman encoder;
    std::string encoded_filename = source_filename.append(".comp");
    std::ofstream encoded_file(encoded_filename, std::ofstream::out | std::ofstream::trunc);
    BitIO outp(&encoded_file, nullptr);

    // Output bits_t sequence given by each encoded char of source_file to encoded_file
    while(source_file.good()) {
        char c = source_file.get();
        if(c == -1) { break; } // If get() = -1, it is EOF. Without this condition, random characters are appended to plaintext after decoding.
        auto bits = encoder.encode(c);
        output_bits_t(bits, outp);
    }
    // Lastly, encode HEOF to designate the end of the file.
    auto bits = encoder.encode(Huffman::HEOF);
    output_bits_t(bits, outp);
    outp.~BitIO(); // to flush out any possibly remaining bits in BitIO buffer_
    
    source_file.close();
    encoded_file.close();
}