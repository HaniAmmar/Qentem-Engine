#include "DequeTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunDequeTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
