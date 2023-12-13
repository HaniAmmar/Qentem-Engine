#include "HArrayTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunHArrayTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
