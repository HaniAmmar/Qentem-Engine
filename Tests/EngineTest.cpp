#include "EngineTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunEngineTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
