#include "QPoolTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunQPoolTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
