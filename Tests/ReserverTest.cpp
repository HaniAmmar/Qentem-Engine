#include "ReserverTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunReserverTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
