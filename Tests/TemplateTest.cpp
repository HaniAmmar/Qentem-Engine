#include "TemplateTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunTemplateTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
