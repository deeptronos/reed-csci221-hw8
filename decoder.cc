#include <fstream>
#include "huffman.hh"
#include "bitio.hh"

///////////////////////////////////////////////////////////////////////////////
// The main function performs the decoding.
int main(int argc, char* argv[]) {
    // Ensure that filename was given
    if(argc != 2) { throw std::logic_error("Invalid number of arguments. Correct usage: ./<exe> <filename>"); }

    std::ifstream encoded_file;
    std::string encoded_filename = argv[1];

    // Open file and throw an error if the file doesn't exist.
    encoded_file.open(argv[1]);
    if (!encoded_file) { throw std::logic_error("File not found."); }

    // Declare / initialize necessary objects, files, and streams
    Huffman decoder;
    std::string plaintext_filename = encoded_filename.append(".plaintext");
    std::ofstream plaintext_file(plaintext_filename, std::ofstream::out | std::ofstream::trunc);
    BitIO inp(nullptr, &encoded_file); // To get input from encoded_file

    // Call Huffman::decode() until a symbol is found. If said symbol is a valid character,
    // output it to plaintext_file and continue. Otherwise, if said symbol is Huffman::HEOF,
    // then stop (as we've reached the end of the file).
    int symbol = -1;
    while (true) {
        while (symbol < 0) { symbol = decoder.decode(inp.input_bit()); }
        if (symbol == Huffman::HEOF) { break; }
        else {
            plaintext_file.put(symbol);
            symbol = -1;
        }
    }

    encoded_file.close();
    plaintext_file.close();
}