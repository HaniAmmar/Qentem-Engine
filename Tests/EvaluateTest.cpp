#include "EvaluateTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunEvaluateTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
