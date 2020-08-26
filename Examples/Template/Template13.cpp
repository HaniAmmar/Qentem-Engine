#include "Template.hpp"

#include <iostream>

using Qentem::String;
using Qentem::Template;
using Qentem::Value;

namespace JSON = Qentem::JSON;

int main() {
    Value value = JSON::Parse(R"css(
{
    "dir": "ltr",
    "title": "Wishful News",
    "darkmode": 1,
    "body": {
        "bc": "rgb(17, 66, 61)",
        "color": "black",
        "darkmode_color": "white",
        "H1": "Leatest News"
    },
    "news": {
        "Tech": [
            {
                "text": "The new modern operating system CoolOp is \"Cool\".",
                "date": "1",
                "options": ""
            },
            {
                "text": "The Global Internet is now free.",
                "date": "2",
                "options": "style=\"color: violet;\""
            },
            {
                "text": "No more hackers after implementing NO-WAY-CRPT encryption.",
                "date": "3",
                "options": ""
            }
        ],
        "Health": [
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
                "options": "style=\"font-style: italic;\""
            }
        ],
        "Energy": [
            {
                "text": "No more Uranium.",
                "date": "3011-1-19",
                "options": "style=\"color: red;\""
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
}
    )css");

    String content(R"HTML(
<!DOCTYPE html>
<html dir="{var:dir}">

<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>{var:title}</title>
    <style>
        body {
            background-color: {var:body[bc]};
            color: {if case="{var:darkmode}" true="{var:body[darkmode_color]}" false="{var:body[color]}"}
        }
    </style>
</head>

<body>
    <h1>{var:body[H1]}</h1>

    <loop set="news" key="NewsID">
    <h2>NewsID</h2>
    <div>
        <ul>
            <loop set="news[NewsID]" value="news_val">
            <li><span news_val[options]>news_val[text]</span> -
            <span>
                <if case="news_val[date] == 1">
                New
                <elseif case="news_val[date] == 2" />
                Yesterday
                <elseif case="news_val[date] == 3" />
                Two days ago
                <else />
                news_val[date]
                </if>
            </span>
            </li>
            </loop>
        </ul>
    </div>
    </loop>
</body>

</html>
)HTML");

    std::cout << Template<>::Render(content, &value).Char() << '\n';
}
