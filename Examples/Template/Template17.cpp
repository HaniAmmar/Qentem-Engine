#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::Array;
using Qentem::HArray;
using Qentem::JSON;
using Qentem::SizeT;
using Qentem::SizeT32;
using Qentem::StringStream;
using Qentem::StringUtils;
using Qentem::Template;
using Qentem::TemplateSub;
using Qentem::Value;

template <typename _Char_T, typename _Value_T, typename _StringStream_T>
inline static void CachedRender(const _Char_T *content, const SizeT length, const _Value_T &value,
                                _StringStream_T &stream, const _Char_T *name, const SizeT name_length) {
    // This is not a thread safe function, and its here to show how to cache processed tags.
    // Can be used in a single threaded process to build on. One easy way is to Parse() all templates
    // before starting multi-threading process.

    using TemplateSubCV = TemplateSub<_Char_T, _Value_T, _StringStream_T>;

    const TemplateSubCV temp{content, length, &stream, &value};

    static HArray<Array<Qentem::Tags::TagBit>, _Char_T> cache;

    Array<Qentem::Tags::TagBit> &tags = cache.Get(name, name_length);

    if (tags.IsEmpty()) {
        temp.Parse(tags);
    }

    const Qentem::Tags::TagBit *tag = tags.First();
    const Qentem::Tags::TagBit *end = (tag + tags.Size());
    temp.Render(tag, end);
}

int main() {
    auto value = JSON::Parse(R"(
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
                        <elseif case="{var:student_val[GPA]} >= 3.5" /> (President's List!)
                        <elseif case="{var:student_val[GPA]} >= 3.0" /> (Dean's List!)
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

    const SizeT        content_length = StringUtils::Count(content);
    StringStream<char> stream;

    for (SizeT32 i = 0; i < 10000U; i++) {
        stream.Clear();
        CachedRender(content, content_length, value, stream, "page1", 5);
    }

    std::cout << stream << '\n';
}
