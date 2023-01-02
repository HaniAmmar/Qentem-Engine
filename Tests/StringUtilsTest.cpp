#include "StringUtilsTest.hpp"

int main() {
    int ret = Qentem::Test::RunStringUtilsTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
