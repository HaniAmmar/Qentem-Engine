#include "DigitTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    int ret = Qentem::Test::RunDigitTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
