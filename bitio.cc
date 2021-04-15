#include <stdexcept>
#include "bitio.hh"

BitIO::BitIO(std::ostream* os, std::istream* is) {

    // Throw errors if arguments are invalid
    if(os == nullptr && is == nullptr) {
        throw std::invalid_argument("Ostream and istream are both nullptr.");
    }
    else if(os != nullptr && is != nullptr) {
        throw std::invalid_argument("Neither ostream nor istream are nullptr.");
    }

    // Otherwise, assign os and is values to class variables os_ and is_
    os_ = os;
    is_ = is;
    pos_ = 0; // Start pos_ at 0
}

//////////////////////////////////////////////////////////////////////////////
// Function that takes an 8-bit std::bitset and returns an unsigned char.

char bitset_to_char(std::bitset<8> byte) {
    return static_cast<unsigned char>(byte.to_ulong());
}

//////////////////////////////////////////////////////////////////////////////
// Destructor that outputs leftover bits in buffer with enough trailing zeros 
// to constitute one byte of data.

BitIO::~BitIO() {
    // If BitIO obj is constructed for output and buffer is not empty...
    if(os_ != nullptr && pos_ != 0 && buffer_.to_ulong() != 0) {
        unsigned char byte = bitset_to_char(buffer_);
        *os_ << byte;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Outputs one bit into the buffer every call. Once the buffer contains 8
// bits of data, the buffer is output to os_. If the total number of bits 
// outputted is not a multiple of 8 (i.e., 8, 16, 24...), then the remaining 
// bits in the buffer will be output to os_ upon destruction of BitIO object,
// padded with trailing zeros to constitute one byte of data.

void BitIO::output_bit(bool bit) {

    // Check if BitIO is correctly constructed with valid arguments.
    if(is_ != nullptr) {
        throw std::logic_error("BitIO object currently constructed for input.");
    }
    else if(os_ == nullptr) {
        throw std::invalid_argument("Ostream is nullptr.");
    }

    buffer_.set(pos_, bit); // Sets bit at current pos_

    // If there is less than 7 bits in the buffer
    if(pos_ < 7) {
        pos_++;
    }
    // If current bit is the final bit, we output to os_ (since we have 1 byte of data now)
    else if(pos_ == 7) {
        unsigned char byte = bitset_to_char(buffer_);
        *os_ << byte;
        buffer_.reset(); // Sets buffer to 00000000
        pos_ = 0;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Returns one bit (true or false) every call. After all significant bits 
// (not trailing zeros) have been returned via input_bit(), all subsequent
// calls will return false. For use in encoding/decoding, make use of
// Huffman::HEOF symbol at the end of the encoded text to designate when 
// the user should stop calling input_bit().


bool BitIO::input_bit() {
    // Check if BitIO is correctly constructed with valid arguments.
    if(os_ != nullptr) {
        throw std::logic_error("BitIO object currently constructed for output.");
    }
    else if(is_ == nullptr) {
        throw std::invalid_argument("Istream is nullptr");
    }

    // If there is no byte loaded into buffer
    if(pos_ == 0) {
        // Check if is_ is at EOF. If so, then since buffer is empty, there are no bytes left to read.
        if(is_ -> eof() == true) {
            return false;
        }
        // If fail flag is set but eof flag is not, then something bad has probably happened...
        else if(is_ -> fail() == true) {
            throw std::logic_error("is_ encountered fail.");
        }
        char byte = is_ -> get(); // Get char from is_ and store in byte
        buffer_ = byte; // Set val of buffer to char (numerical)
    }

    // Once a byte has been loaded into buffer and we haven't yet finished reading the individual bits
    if(pos_ < 7) {
        bool temp = buffer_.test(pos_);
        pos_++;
        return temp;
    }
    // If we have reached the final bit in the buffer
    else if(pos_ == 7) {
        bool temp = buffer_.test(pos_);
        buffer_.reset();
        pos_ = 0;
        return temp;
    }
	// Compiler gets angry if there isn't a default return value. 
	// Program should never reach this line, though, as all cases are covered above.
    return false; 
}