#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::SizeT;
using Qentem::StringStream;

/*
mkdir Build
c++ -g ./Examples/Template/Template17.cpp -I ./Include -o ./Build/QTest.bin
./Build/QTest.bin
*/

template <typename Char_T, typename Value_T, typename StringStream_T>
inline static void CachedRender(const Char_T *content, const SizeT length, const Value_T &value, StringStream_T &stream,
                                const Char_T *name, const SizeT name_length) {
    // This is not a thread-safe function, and it's here to show how to cache processed tags. Can be used in a
    // single-threaded process to build on. One lazy way to make it safe is to Parse() all templates before starting the
    // multi-threading process.

    using TemplateSubCV = Qentem::TemplateSub<Char_T, Value_T, StringStream_T>;
    using Tag           = Qentem::Tags::TagBit;
    using Qentem::Array;
    using Qentem::HArray;

    static HArray<Array<Tag>, Char_T> cache;
    const TemplateSubCV               temp{content, length, &stream, &value};
    Array<Tag>                       &tags = cache.Get(name, name_length);

    if (tags.IsEmpty()) {
        temp.Parse(tags);

        // Or
        // temp.LightParse(tags); // for {var:} {raw:} {math:} only
    }

    const Tag *tag = tags.First();
    const Tag *end = (tag + tags.Size()); // Or tags.End()
    temp.Render(tag, end);
}

int main() {
    Qentem::Value<char> value = JSON::Parse(R"(
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

    const char *content = R"(
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

    // Qentem::Template can use any stream if it has Write(const char_type *, length) function.
    StringStream<char> stream;
    const char        *template_name        = "page1";
    const SizeT        template_name_length = 5U;
    const SizeT        content_length       = Qentem::StringUtils::Count(content);

    for (unsigned int i = 0; i < 10000U; i++) {
        stream.Clear();
        CachedRender(content, content_length, value, stream, template_name, template_name_length);
    }

    std::cout << stream << '\n';
}
