#include "ValueTest.hpp"

int main() {
    int ret = Qentem::Test::RunValueTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
