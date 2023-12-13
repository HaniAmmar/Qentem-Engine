#include "TemplateUTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunTemplateUTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
