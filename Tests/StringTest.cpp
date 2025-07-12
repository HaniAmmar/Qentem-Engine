#include "StringTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunStringTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
