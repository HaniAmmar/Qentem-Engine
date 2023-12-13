#include "StringUtilsTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunStringUtilsTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
