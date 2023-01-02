#include "UnicodeTest.hpp"

int main() {
    int ret = Qentem::Test::RunUnicodeTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
