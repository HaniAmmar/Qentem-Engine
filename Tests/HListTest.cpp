#include "HListTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunHListTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
