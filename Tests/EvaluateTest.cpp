#include "EvaluateTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunEvaluateTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
