#include "ValueTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunValueTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
