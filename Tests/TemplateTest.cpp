#include "TemplateTest.hpp"

int main() {
    int ret = Qentem::Test::RunTemplateTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
