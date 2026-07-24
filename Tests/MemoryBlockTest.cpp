#include "MemoryBlockTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunMemoryBlockTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
