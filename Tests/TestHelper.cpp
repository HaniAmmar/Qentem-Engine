#include "TestHelper.hpp"

int main() {
    int ret = Qentem::Test::RunTestHelperTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
