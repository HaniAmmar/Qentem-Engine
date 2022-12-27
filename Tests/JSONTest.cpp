#include "JSONTest.hpp"

int main() {
    int ret = Qentem::Test::RunJSONTests();
    Qentem::Test::TestHelper::PrintMemoryStatus();

    return ret;
}
