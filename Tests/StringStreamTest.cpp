#include "StringStreamTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunStringStreamTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
