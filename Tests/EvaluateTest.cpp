#include "EvaluateTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunEvaluateTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
