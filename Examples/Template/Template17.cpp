#include "JSON.hpp"
#include "Template.hpp"
#include "StringView.hpp"

#include <iostream>

using Qentem::StringStream;
using Qentem::StringView;

/*
mkdir Build
c++ -O3 ./Examples/Template/Template17.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

// -- Cached Render Function (Pre-parses template once) --
template <typename Char_T, typename Value_T, typename StringStream_T>
inline static void CachedRender(const StringView<Char_T> &content, const Value_T &value, StringStream_T &stream,
                                const StringView<Char_T> &template_name) {
    // This is not a thread-safe function, and it's here to show how to cache processed tags.
    // One lazy way to make it thread-safe is to Parse() all templates before
    // starting a multi-threading process.

    using Qentem::Array;
    using Qentem::HArray;
    using Qentem::String;

    using TemplateCore = Qentem::TemplateCore<Char_T, Value_T, StringStream_T>;
    using TagBit       = Qentem::Tags::TagBit;

    TemplateCore                                 temp{content.First(), content.Length()};
    static HArray<String<Char_T>, Array<TagBit>> tags_cache;

    Array<TagBit> &tags = tags_cache.Get(template_name.First(), template_name.Length());

    if (tags.IsEmpty()) {
        TemplateCore::Parse(content.First(), content.Length(), tags);
    }

    temp.Render(tags, value, stream);
}

////////////////////////////////////////////////////////////////////

int main() {
    Qentem::Value<char> value = Qentem::JSON::Parse(R"(
[
    {
        "major": "Computer Science",
        "students": [
            { "Name": "Student1", "GPA": 3.2 },
            { "Name": "Student2", "GPA": 3.8 },
            { "Name": "Student3", "GPA": 2.8 }
        ]
    },
    {
        "major": "Math",
        "students": [
            { "Name": "Student4", "GPA": 3.0 },
            { "Name": "Student5", "GPA": 2.5 },
            { "Name": "Student6", "GPA": 2.4 }
        ]
    }
]
    )");

    ////////////////////////////////////////////////////////////////////

    const StringView<char> content = R"(
<html>

<body>
    <div>
        <h2>Students' list:</h2>
        <loop value="department_val">
            <h3>Major: {var:department_val[major]}</h3>
            <ul>
            <loop set="department_val[students]" value="student_val">
                <li>
                    <span>Name: {var:student_val[Name]}</span>
                    <span>
                        GPA: {var:student_val[GPA]}
                        <if case="{var:student_val[GPA]} < 2.5"> (Inform adviser!)
                        <else if case="{var:student_val[GPA]} >= 3.5"> (President's List!)
                        <else if case="{var:student_val[GPA]} >= 3.0"> (Dean's List!)
                        </if>
                    </span>
                </li>
            </loop>
            </ul>
        </loop>
    </div>
</body>

</html>
)";

    ////////////////////////////////////////////////////////////////////

    StringStream<char>     stream;
    const StringView<char> template_name{"page1"};

    for (unsigned int i = 0; i < 10000U; i++) {
        stream.Clear();
        CachedRender(content, value, stream, template_name);
    }

    std::cout << stream << '\n';
}
