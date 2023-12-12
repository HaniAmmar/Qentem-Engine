#include "ArrayTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunArrayTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
