#include "BigInt.hpp"

#include <iostream>
///////////////////////////////////////
using Qentem::BigInt;
///////////////////////////////////////
template <typename BigInt_T>
static void StreamDigits(char *storage, unsigned int &index, BigInt_T &bint) {
    index = 0;

    while (bint != 0) {
        storage[index] = (static_cast<char>(bint.Divide(10)) + '0');
        ++index;
    }
}

template <typename BigInt_T>
static void PrintDigits(BigInt_T bint) {
    constexpr unsigned int max = (((bint.TotalBits() * 30103U) / 100000U) + 1U);
    char                   storage[max]{0};
    unsigned int           index = 0;

    StreamDigits(&(storage[0]), index, bint);

    while (index != 0) {
        --index;
        std::cout << storage[index];
    }

    std::cout << "\n\n";
}
///////////////////////////////////////
int main() {
    // 1024 is the number of bits, and it can be much bigger.
    BigInt<unsigned long long, 1024U> bint;
    // or
    // BigInt<unsigned long long, 2048U> bint;

    bint = 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;
    bint <<= 64U;
    bint |= 0xFFFFFFFFFFFFFFFF;

    PrintDigits(bint);
    // Output:
    // 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215

    ////////// unsigned short 128-bit /////////////

    BigInt<unsigned short, 128U> bint2 = 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;
    bint2 <<= 16U;
    bint2 |= 0xFFFF;

    PrintDigits(bint2);
    // Output: 340282366920938463463374607431768211455

    ////////// unsigned int 128-bit /////////////

    BigInt<unsigned int, 128U> bint3 = 0xFFFFFFFF;
    bint3 <<= 32U;
    bint3 |= 0xFFFFFFFF;

    PrintDigits(bint3);
    // Output: 18446744073709551615

    bint3 *= 4294967295U;
    bint3 *= 4294967295U;

    PrintDigits(bint3);
    // Output: 340282366762482138434845932253270245375

    std::cout << "Remaining: " << bint3.Divide(1000000000U) << '\n';
    // Output: Remaining: 270245375

    PrintDigits(bint3);
    // Output: 340282366762482138434845932253

    // Also
    // bint3 += number;
    // bint3 -= number;
    // bint3 &= number;
    // bint3 /= number;
}
