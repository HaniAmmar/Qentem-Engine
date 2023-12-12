#include "UnicodeTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunUnicodeTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
