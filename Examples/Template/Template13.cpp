#include "JSON.hpp"
#include "Template.hpp"

#include <iostream>

using Qentem::JSON;
using Qentem::StringStream;
using Qentem::Template;
using Qentem::Value;

int main() {
    auto value = JSON::Parse(R"css(
{
    "dir": "ltr",
    "title": "Wishful News",
    "dark_mode": 1,
    "body": {
        "bc": "rgb(17, 66, 61)",
        "color": "black",
        "dark_mode_color": "white",
        "H1": "Latest News"
    },
    "news": [
        {
            "name": "Tech",
            "list": [
                {
                    "text": "The new modern operating system CoolOp is \"Cool\".",
                    "date": "1",
                    "options": ""
                },
                {
                    "text": "The Global Internet is now free.",
                    "date": "2",
                    "options": "v_span"
                },
                {
                    "text": "No more hackers after implementing NO-WAY-CRPT encryption.",
                    "date": "3",
                    "options": ""
                }
            ]
        },
        {
            "name": "Health",
            "list": [
                {
                    "text": "Free medicine are being given away.",
                    "date": "3011-1-28",
                    "options": ""
                },
                {
                    "text": "People are healthier now after using MakeMYMeds devices.",
                    "date": "3011-1-20",
                    "options": ""
                },
                {
                    "text": "TeraMedicine forced to shutdown.",
                    "date": "3011-1-16",
                    "options": "i_span"
                }
            ]
        },
        {
            "name": "Energy",
            "list": [
                {
                    "text": "No more Uranium.",
                    "date": "3011-1-19",
                    "options": "red_span"
                },
                {
                    "text": "The Global Internet is now free.",
                    "date": "3011-1-19",
                    "options": ""
                },
                {
                    "text": "Gasoline cars are now banned.",
                    "date": "3011-1-18",
                    "options": ""
                }
            ]
        }
    ]
}
    )css");

    const char *content = R"HTML(
<html dir="{var:dir}">

<head>
    <style>
        body {
            background-color: {raw:body[bc]};
            color: {if case="{var:dark_mode}" true="{var:body[dark_mode_color]}" false="{var:body[color]}"}
        }

        .i_span {
            font-style: italic;
        }

        .red_span {
            color: red;
        }

        .v_span {
            color: violet;
        }
    </style>
</head>

<body>
    <h1>{var:body[H1]}</h1>

    <loop set="news" value="value_s">
    <h2>{var:value_s[name]}</h2>
    <div>
        <ul>
            <loop set="value_s[list]" value="value">
            <li><span class="{var:value[options]}">{var:value[text]}</span> -
                <span>
                    <if case="{var:value[date]} == 1">
                    New
                    <elseif case="{var:value[date]} == 2" />
                    Yesterday
                    <elseif case="{var:value[date]} == 3" />
                    Two days ago
                    <else />
                    {var:value[date]}
                    </if>
                </span>
            </li>
            </loop>
        </ul>
    </div>
    </loop>
</body>

</html>
)HTML";

    std::cout << Template::Render<StringStream<char>>(content, value) << '\n';
}
