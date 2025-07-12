#include "TemplateTest.hpp"

int main() {
    Qentem::QTest::PrintInfo();
    const int ret = Qentem::Test::RunTemplateTests();
    Qentem::QTest::PrintMemoryStatus();

    return ret;
}
