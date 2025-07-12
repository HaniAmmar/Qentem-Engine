#include "DigitTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunDigitTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
