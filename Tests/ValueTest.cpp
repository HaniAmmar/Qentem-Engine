#include "ValueTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunValueTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
