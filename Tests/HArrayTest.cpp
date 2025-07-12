#include "HArrayTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunHArrayTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
