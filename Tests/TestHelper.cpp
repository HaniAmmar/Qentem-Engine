#include "TestHelper.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunTestHelperTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
