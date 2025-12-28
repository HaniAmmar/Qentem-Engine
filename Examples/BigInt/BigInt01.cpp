#include "Qentem/BigInt.hpp"

#include <iostream>

/*
mkdir Build
c++ -g ./Examples/BigInt/BigInt01.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

///////////////////////////////////////
using Qentem::BigInt;
///////////////////////////////////////
template <typename BigInt_T>
static void StreamDigits(char *storage, unsigned int &index, BigInt_T &b_int) {
    index = 0;

    while (b_int != 0) {
        storage[index] = (static_cast<char>(b_int.Divide(10)) + '0');
        ++index;
    }
}

template <typename BigInt_T>
static void PrintDigits(BigInt_T b_int) {
    constexpr unsigned int max = (((b_int.TotalBitWidth() * 30103U) / 100000U) + 1U);
    char                   storage[max]{0};
    unsigned int           index = 0;

    StreamDigits(&(storage[0]), index, b_int);

    while (index != 0) {
        --index;
        std::cout << storage[index];
    }

    std::cout << "\n\n";
}
///////////////////////////////////////
int main() {
    // 1024 is the number of bits, and it can be much bigger.
    BigInt<unsigned long long, 1024U> b_int;
    // or
    // BigInt<unsigned long long, 2048U> b_int;

    b_int = 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;
    b_int <<= 64U;
    b_int |= 0xFFFFFFFFFFFFFFFF;

    PrintDigits(b_int);
    // Output:
    // 179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215

    ////////// unsigned short 128-bit /////////////

    BigInt<unsigned short, 128U> b_int2 = 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;
    b_int2 <<= 16U;
    b_int2 |= 0xFFFF;

    PrintDigits(b_int2);
    // Output: 340282366920938463463374607431768211455

    ////////// unsigned int 128-bit /////////////

    BigInt<unsigned int, 128U> b_int3 = 0xFFFFFFFF;
    b_int3 <<= 32U;
    b_int3 |= 0xFFFFFFFF;

    PrintDigits(b_int3);
    // Output: 18446744073709551615

    b_int3 *= 4294967295U;
    b_int3 *= 4294967295U;

    PrintDigits(b_int3);
    // Output: 340282366762482138434845932253270245375

    std::cout << "Remaining: " << b_int3.Divide(1000000000U) << '\n';
    // Output: Remaining: 270245375

    PrintDigits(b_int3);
    // Output: 340282366762482138434845932253

    // Also
    // b_int3 += number;
    // b_int3 -= number;
    // b_int3 &= number;
    // b_int3 /= number;
}
