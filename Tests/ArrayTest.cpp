#include "ArrayTest.hpp"

int main() {
    int ret = Qentem::Test::RunArrayTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
