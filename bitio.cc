#include <stdexcept>
#include <iostream>
#include "bitio.hh"

BitIO::BitIO(std::ostream* os, std::istream* is) {

    // Throw errors if inputs are invalid
    if(os == nullptr && is == nullptr) {
        throw std::invalid_argument("Ostream and istream are both nullptr.");
    }
    else if(os != nullptr && is != nullptr) {
        throw std::invalid_argument("Neither ostream nor istream are nullptr.");
    }

    // Otherwise, assign os and is values to class variables
    os_ = os;
    is_ = is;
    bit_pos = 0;
    buffer_exists = false;
}

// Function that returns a char from a bitset
char bitset_to_char(std::bitset<8> byte) {
    return static_cast<unsigned char>(byte.to_ulong());
}

BitIO::~BitIO() {
    if(os_ != nullptr && buffer_exists) {
        unsigned char byte = bitset_to_char(buffer_);
        *os_ << byte;
    }
}

void BitIO::output_bit(bool bit) {

    // Check if BitIO is correctly constructed with valid arguments.
    if(is_ != nullptr) {
        throw std::logic_error("BitIO object currently constructed for input.");
    }
    else if(os_ == nullptr) {
        throw std::invalid_argument("Ostream is nullptr.");
    }

    buffer_.set(bit_pos, bit);

    // If we there is less than 7 bits or fewer in the buffer
    if(bit_pos < 7) {
        buffer_exists = true;
        bit_pos = bit_pos+1;
    }
    // If current bit is the final bit, we can then output to os_
    else if(bit_pos == 7) {
        unsigned char byte = bitset_to_char(buffer_);
        *os_ << byte;
        buffer_.reset();
        bit_pos = 0;
        buffer_exists = false;
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
    if(bit_pos == 0) {

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
        buffer_exists = true;
    }

    // If a byte has already been loaded into buffer
    if(bit_pos < 7) {
        bool temp = buffer_.test(bit_pos);
        bit_pos++;
        return temp;
    }
    // If we have reached the final bit in the buffer
    else if(bit_pos == 7) {
        bool temp = buffer_.test(bit_pos);
        buffer_.reset();
        bit_pos = 0;
        buffer_exists = false;
        return temp;
    }
    return false;
}