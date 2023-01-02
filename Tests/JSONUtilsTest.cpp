#include "JSONUtilsTest.hpp"

int main() {
    int ret = Qentem::Test::RunJSONUtilsTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
