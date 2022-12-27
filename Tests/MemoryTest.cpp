#include "MemoryTest.hpp"

int main() {
    int ret = Qentem::Test::RunMemoryTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
