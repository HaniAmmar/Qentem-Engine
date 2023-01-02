#include "ArrayTest.hpp"

int main() {
    int ret = Qentem::Test::RunArrayTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
