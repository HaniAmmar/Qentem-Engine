#include "ArrayTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunArrayTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
