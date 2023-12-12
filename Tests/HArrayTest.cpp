#include "HArrayTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunHArrayTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
