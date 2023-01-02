#include "TemplateUTest.hpp"

int main() {
    int ret = Qentem::Test::RunTemplateUTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
