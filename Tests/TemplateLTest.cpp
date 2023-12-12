#include "TemplateLTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunTemplateLTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
