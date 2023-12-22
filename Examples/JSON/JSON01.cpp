#include "JSON.hpp"

#include <iostream>

using Value = Qentem::Value<char>;
using Qentem::JSON;

int main() {
    Value abc = JSON::Parse(R"(["A","B","C"])");

    Value v_arr; // To be used as an array.

    v_arr += 0;
    v_arr[1] = 10;
    v_arr += nullptr;
    v_arr += true;
    v_arr += false;

    v_arr[0] = "text"; // Overwrite

    v_arr += abc; // Add sub-array
    v_arr += JSON::Parse(R"([100,200,300])");

    std::cout << v_arr.Stringify() << '\n';
    /* Output:
        [
            "text",
            10,
            null,
            true,
            false,
            [
                "A",
                "B",
                "C"
            ],
            100,
            200,
            300
        ]
    */

    ///////////////////////////////////////////

    Value v_obj; // To be used as an object.

    v_obj["key0"] = 0;
    v_obj["key1"] = 10;
    v_obj["key2"] = nullptr;
    v_obj["key3"] = true;
    v_obj["key4"] = false;
    v_obj["key5"] = nullptr;

    v_obj["key6"] = abc; // Add sub-array

    // Replace any value with the same key, or add it if it does not exist.
    v_obj += JSON::Parse(R"({"key0": "text", "key4": true, "key5": 500, "key7": [1,2,3,4], "key8": null})");

    std::cout << v_obj.Stringify() << '\n';
    /* Output:
       {
            "key0": "text",
            "key1": 10,
            "key2": null,
            "key3": true,
            "key4": true,
            "key5": 500,
            "key6": [
                "A",
                "B",
                "C"
            ],
            "key7": [
                1,
                2,
                3,
                4
            ],
            "key8": null
        }
    */

    ///////////////////////////////////////////

    // Sorting

    v_arr.Reset();

    v_arr += 4;
    v_arr += 1;
    v_arr += 3;
    v_arr += 5;
    v_arr += 2;
    v_arr += 7;
    v_arr += 6;

    v_arr.Sort(); // Ascending

    std::cout << v_arr.Stringify() << '\n';
    // Output: [1,2,3,4,5,6,7]

    v_arr.Sort(false); // Descending

    std::cout << v_arr.Stringify() << '\n';
    // Output: [7,6,5,4,3,2,1]

    ///////////////////////////////////////////

    // Grouping

    v_arr = JSON::Parse(
        R"([
            {"year":2019,"month":4},
            {"year":2020,"month":5},
            {"year":2017,"month":1},
            {"year":2020,"month":6},
            {"year":2018,"month":2},
            {"year":2020,"month":7},
            {"year":2018,"month":3}])");

    Value v_arr2;
    if (v_arr.GroupBy(v_arr2, "year")) {
        v_arr2.Sort();
        std::cout << v_arr2.Stringify() << '\n';
    }

    /* Output:
    {
        "2017": [
            {
                "month": 1
            }
        ],
        "2018": [
            {
                "month": 2
            },
            {
                "month": 3
            }
        ],
        "2019": [
            {
                "month": 4
            }
        ],
        "2020": [
            {
                "month": 5
            },
            {
                "month": 6
            },
            {
                "month": 7
            }
        ]
    }
    */
}
