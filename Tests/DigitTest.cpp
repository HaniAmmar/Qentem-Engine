#include "DigitTest.hpp"

int main() {
    int ret = Qentem::Test::RunDigitTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
