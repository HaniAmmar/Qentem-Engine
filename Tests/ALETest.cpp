#include "ALETest.hpp"

int main() {
    int ret = Qentem::Test::RunALETests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
