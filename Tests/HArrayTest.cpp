#include "HArrayTest.hpp"

int main() {
    int ret = Qentem::Test::RunHArrayTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
