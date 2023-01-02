#include "JSONTest.hpp"

int main() {
    int ret = Qentem::Test::RunJSONTests();
    Qentem::TestHelper::PrintMemoryStatus();

    return ret;
}
