#include "JSONUtilsTest.hpp"

int main() {
    int ret = Qentem::Test::RunJSONUtilsTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
