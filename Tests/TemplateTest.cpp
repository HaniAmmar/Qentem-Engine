#include "TemplateTest.hpp"

int main() {
    int ret = Qentem::Test::RunTemplateTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
