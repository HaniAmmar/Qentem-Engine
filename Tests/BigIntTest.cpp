#include "BigIntTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunBigIntTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
