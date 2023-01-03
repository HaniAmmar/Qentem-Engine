#include "MemoryTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunMemoryTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
