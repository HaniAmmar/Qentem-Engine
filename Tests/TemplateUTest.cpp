#include "TemplateUTest.hpp"

int main() {
    int ret = Qentem::Test::RunTemplateUTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
