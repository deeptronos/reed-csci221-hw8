#include <stdexcept>
#include "bitio.hh"

BitIO::BitIO(std::ostream* os, std::istream* is) {

    // Throw errors if inputs are invalid
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

// Function that takes an 8-bit bitset and returns a char
char bitset_to_char(std::bitset<8> byte) {
    return static_cast<unsigned char>(byte.to_ulong());
}

// Destructor that outputs leftover bits in buffer with enough trailing zeros to constitute a byte
BitIO::~BitIO() {
    // If BitIO is constructed for output and there are bits in the buffer....
    if(os_ != nullptr && pos_ != 0 && buffer_.to_ulong() != 0) {
        unsigned char byte = bitset_to_char(buffer_);
        *os_ << byte;
    }
}

// Outputs one bit into buffer and one byte into ostream when buffer has 8 bits
void BitIO::output_bit(bool bit) {

    // Check if BitIO is correctly constructed with valid arguments.
    if(is_ != nullptr) {
        throw std::logic_error("BitIO object currently constructed for input.");
    }
    else if(os_ == nullptr) {
        throw std::invalid_argument("Ostream is nullptr.");
    }

    buffer_.set(pos_, bit);

    // If there is less than 7 bits in the buffer
    if(pos_ < 7) {
        pos_++;
    }
    // If current bit is the final bit, we can then output to os_
    else if(pos_ == 7) {
        unsigned char byte = bitset_to_char(buffer_);
        *os_ << byte;
        buffer_.reset();
        pos_ = 0;
    }
}

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

        // Check if is_ is at EOF, in which case there is no longer any bytes left to read
        if(is_ -> eof() == true) {
            return false;
        }
        // If fail flag is set but eof flag is not
        else if(is_ -> fail() == true) {
            throw std::logic_error("is_ encountered fail.");
        }
        char byte = is_ -> get();
        buffer_ = byte;
    }

    // If a byte has already been loaded into buffer
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
    return false;
}