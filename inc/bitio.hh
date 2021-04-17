/*****************************************************************************
 * A simple class to perform stream I/O on individual bits.                  *
 * Before reading any bits, ensure your input stream still has valid inputs. *
 ****************************************************************************/

#include <iostream>
#include <bitset>

class BitIO {
  private:
    std::ostream* os_;
    std::istream* is_;
    std::bitset<8> buffer_;
    int pos_; // The position of the current bit in the buffer

  public:
    // Construct with one of either an input stream or output (the other null)
    BitIO(std::ostream* os, std::istream* is);

    // Flushes out any remaining output bits, if any:
    ~BitIO();

    BitIO(const BitIO&) = default;
    BitIO(BitIO&&) = default;
    BitIO& operator=(const BitIO&) = default;
    BitIO& operator=(BitIO&&) = default;

    // Output a single bit (buffered)
    void output_bit(bool bit);

    // Read a single bit (or trailing zero)
    bool input_bit();
};