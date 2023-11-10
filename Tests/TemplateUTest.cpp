#include "TemplateUTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunTemplateUTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
