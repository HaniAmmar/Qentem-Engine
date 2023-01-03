#include "TemplateUTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunTemplateUTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
