#include "StringStreamTest.hpp"

int main() {
    int ret = Qentem::Test::RunStringStreamTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
