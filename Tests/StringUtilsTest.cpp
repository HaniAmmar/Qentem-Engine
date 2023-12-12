#include "StringUtilsTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunStringUtilsTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
