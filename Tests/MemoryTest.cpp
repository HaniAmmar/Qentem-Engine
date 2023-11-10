#include "MemoryTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunMemoryTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
