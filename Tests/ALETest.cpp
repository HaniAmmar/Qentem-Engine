#include "ALETest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunALETests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
