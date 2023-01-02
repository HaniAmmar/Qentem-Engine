#include "MemoryTest.hpp"

int main() {
    int ret = Qentem::Test::RunMemoryTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
