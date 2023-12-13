#include "ValueTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunValueTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
