#include "StringStreamTest.hpp"

int main() {
    int ret = Qentem::Test::RunStringStreamTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
