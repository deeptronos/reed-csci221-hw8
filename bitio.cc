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
}

void BitIO::output_bit(bool bit) {

    // Check if BitIO is correctly constructed with valid arguments.
    if(is_ != nullptr) {
        throw std::logic_error("BitIO object currently constructed for input.");
    }
    else if(os_ == nullptr) {
        throw std::invalid_argument("Ostream is nullptr.");
    }
    
    // Insert bit into os_
    *os_ << bit;
}

bool BitIO::input_bit() {

    // Check if BitIO is correctly constructed with valid arguments.
    if(os_ != nullptr) {
        throw std::logic_error("BitIO object currently constructed for output.");
    }
    else if(is_ == nullptr) {
        throw std::invalid_argument("Istrem is nullptr");
    }

    // Check if is_ has encountered an error or is at the EOF
    if(is_ -> eof() == true) {
        return false;
    }
    else if(is_ -> fail() == true) {
        throw std::logic_error("is_ encountered fail.")
    }

    // Otherwise, retrieve one bit and return true or false accordingly
    char bit = is_ -> get();
    if(bit == '1') {
        return true;
    }
    else {
        return false;
    }
}