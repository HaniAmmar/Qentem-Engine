#include "EngineTest.hpp"

int main() {
    int ret = Qentem::Test::RunEngineTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
