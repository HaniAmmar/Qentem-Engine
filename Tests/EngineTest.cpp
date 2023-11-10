#include "EngineTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunEngineTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
