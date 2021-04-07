#include <stdexcept>

#include "bitio.hh"

BitIO::BitIO(std::ostream* os, std::istream* is) {

    // Throw errors if inputs are invalid
    if(os == nullptr && is == nullptr) {
        throw std::invalid_argument("os and is are both nullptr");
    }
    else if(os != nullptr && is == nullptr) {
        throw std::invalid_argument("neither os nor is are nullptr")
    }

    // Otherwise, assign os and is values to class variables
    os_ = os;
    is_ = is;
}