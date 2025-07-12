#include "StringUtilsTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunStringUtilsTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
