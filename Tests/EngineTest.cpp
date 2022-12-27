#include "EngineTest.hpp"

int main() {
    int ret = Qentem::Test::RunEngineTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
