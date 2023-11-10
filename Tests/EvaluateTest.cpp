#include "EvaluateTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunEvaluateTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
