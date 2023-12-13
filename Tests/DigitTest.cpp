#include "DigitTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunDigitTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
