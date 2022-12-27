#include "TemplateLTest.hpp"

int main() {
    int ret = Qentem::Test::RunTemplateLTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
