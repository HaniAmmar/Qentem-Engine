#include "StringStreamTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunStringStreamTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
