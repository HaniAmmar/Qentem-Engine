#include "TemplateLTest.hpp"

int main() {
    int ret = Qentem::Test::RunTemplateLTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
