#include "StringUtilsTest.hpp"

int main() {
    int ret = Qentem::Test::RunStringUtilsTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
