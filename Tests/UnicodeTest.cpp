#include "UnicodeTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunUnicodeTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
