#include "UnicodeTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunUnicodeTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
