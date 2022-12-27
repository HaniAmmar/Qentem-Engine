#include "HArrayTest.hpp"

int main() {
    int ret = Qentem::Test::RunHArrayTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
