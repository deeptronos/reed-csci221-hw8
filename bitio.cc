#include <stdexcept>
#include <iostream>
#include "bitio.hh"

/******************************************************************************************************
* Note: Although test_bitio.cc executes successfully, I've encountered issues when checking           *
* if the arguments are valid (see commented out error throws...). Essentially, even though            *
* one of the streams are nullptr upon construction, the pointer value doesn't seem to be "nullptr"... *
* Besides that, the method I used for inputting and outputting bits uses strings as a medium          *
* instead of plain bits (bools). When I directly inserted and retrieved bits to and from streams,     *
* the values I was getting were all over the place. Using strings here is a stop-gap measure,         *
* and I'll be attending drop-in tutoring later to hopefully fix it.                                   *
*******************************************************************************************************/

BitIO::BitIO(std::ostream* os, std::istream* is) {

    // Throw errors if inputs are invalid
    /*if(os == nullptr && is == nullptr) {
        throw std::invalid_argument("Ostream and istream are both nullptr.");
    }
    else if(os != nullptr && is != nullptr) {
        throw std::invalid_argument("Neither ostream nor istream are nullptr.");
    }*/

    // Otherwise, assign os and is values to class variables
    os_ = os;
    is_ = is;
}

void BitIO::output_bit(bool bit) {

    // Check if BitIO is correctly constructed with valid arguments.
    /*if(is_ != nullptr) {
        throw std::invalid_argument("BitIO object currently constructed for input.");
    }
    else if(os_ == nullptr) {
        throw std::invalid_argument("Ostream is nullptr.");
    }*/
    
    // Insert bit into os_
    if(bit == 0) {
        *os_ << '0';
    }
    else if(bit == 1) {
        *os_ << '1';
    }
}

bool BitIO::input_bit() {

    // Check if BitIO is correctly constructed with valid arguments.
    /*if(os_ != nullptr) {
        throw std::invalid_argument("BitIO object currently constructed for output.");
    }
    else if(is_ == nullptr) {
        throw std::invalid_argument("Istrem is nullptr");
    }*/

    // Check if is_ has encountered an error or is at the EOF
    if(is_ -> eof() == true) {
        return false;
    }
    else if(is_ -> fail() == true) {
        std::cerr << "fail flag is true.";
    }

    // Otherwise, retrieve one bit and return true or false accordingly
    char bit_char = is_ -> get();
    if(bit_char == '1') {
        return true;
    }
    else {
        return false;
    }
}