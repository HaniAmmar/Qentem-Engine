#include "TemplateLTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunTemplateLTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
