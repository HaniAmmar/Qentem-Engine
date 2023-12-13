#include "TemplateLTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunTemplateLTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
