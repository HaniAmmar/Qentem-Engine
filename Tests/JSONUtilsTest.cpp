#include "JSONUtilsTest.hpp"

int main() {
    Qentem::TestHelper::PrintInfo();
    const int ret = Qentem::Test::RunJSONUtilsTests();
    Qentem::MemoryRecord::PrintMemoryStatus();

    return ret;
}
