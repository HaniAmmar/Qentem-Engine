#include "TemplateTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunTemplateTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
