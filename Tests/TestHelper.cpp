#include "TestHelper.hpp"

int main() {
    int ret = Qentem::Test::RunTestHelperTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
