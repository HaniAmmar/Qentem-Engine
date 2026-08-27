## Qentem Template Engine — Advanced Documentation

### Overview
Qentem Engine's templating system is a high-performance, lightweight solution designed to dynamically generate structured content such as HTML, JSON, or plaintext. It provides flexible data access, inline logic evaluation, math expressions, powerful looping, and template composition, all embedded directly in your templates.

This guide documents the supported tags and features, including syntax, behavior, rendering semantics, caching, template composition, and practical examples.

A central design principle of Qentem's template engine is that templates are **rendered**, rather than treated as pieces of text that must be assembled into intermediate strings. This becomes especially important when using sub-templates: a sub-template can be rendered directly into the same output stream as its parent template, allowing complex pages to be composed without first rendering every piece into a separate buffer.

---

### Tags Summary

| Tag | Purpose | Supports |
|------|---------|----------|
| `{var:...}` | Access and safely render a value (with HTML escaping). | Nested keys/indexes, strings, numbers |
| `{raw:...}` | Render raw (unescaped) values directly. | Same as `var`, but no escaping |
| `{st:...}` | Render another template selected by a template ID. | Dynamic template selection, nesting, shared render data |
| `{math:...}` | Perform arithmetic, logic, or bitwise operations. | Numbers, variables, nested expressions |
| `{svar:template, ...}` | Substitutes `{0}` to `{9}` in a string template. | Up to 10 variables, expressions, or raw values |
| `{if case="..." true="..." false="..."}` | Inline conditional rendering. | Variables, math, constants |
| `<if case="...">...</if>` | Conditional block with full logic flow. | `elseif`, `else`, nested blocks |
| `<loop set="..." value="..." group="..." sort="...">...</loop>` | Iterates over collections with sorting/grouping. | Arrays, objects |

**Legend:**
- `...` means an expression, key, or identifier.
- Supports single or double-quoted attribute values.
- `{st:...}` is different from `{var:...}` and `{raw:...}`: it selects and renders another template rather than writing a value as text.

---

### Variable Tag
```txt
{var:key}             // Access top-level key
{var:user[name]}      // Nested object
{var:items[0][price]} // Indexed array access
```

Safely outputs data by resolving the provided path. HTML-sensitive characters are escaped.

Use this for ordinary browser-visible data where the rendered value must be escaped.

---

### Raw Variable Tag
```txt
{raw:html_block}
{raw:user[signature]}
```

Outputs raw data without escaping. Only use this with trusted inputs such as validated HTML fragments.

`{raw:...}` still resolves a value from the render data. It does not select another template. For template composition, use `{st:...}`.

---

### Sub-Template Tag

```txt
{st:template_id}
```

The Sub-Template Tag selects another template using a template ID obtained from the current render data, then renders that template directly at the position of the tag.

This is **template composition**, not string substitution.

A useful mental model is:

```txt
Parent template
      |
      v
 encounter {st:template_id}
      |
      v
resolve template_id from Value
      |
      v
select matching template
      |
      v
render selected template
into the SAME output stream
      |
      v
continue rendering parent
```

The parent does not need to know the HTML content of the child in advance. It only needs a sub-template tag and the render data that identifies which template should occupy that position.

#### Basic Example

```cpp
enum struct TemplatesID : unsigned {
    GenericPage,
    HomePage,
    AboutPage
};
```

The generic template can provide a reusable page shell:

```html
<html>
<body>
    <main id="container">{st:template_id}</main>
</body>
</html>
```

The selected page is determined by the render data:

```cpp
value["template_id"] = static_cast<unsigned>(TemplatesID::HomePage);
```

or:

```cpp
value["template_id"] = static_cast<unsigned>(TemplatesID::AboutPage);
```

The same `GenericPage` template can therefore render different page templates without modifying the generic template itself.

#### Template IDs Are Data, Not Template Source

The value referenced by `{st:template_id}` is interpreted as a template ID. The tag does not contain the template source and does not require the parent template to contain the child template's HTML.

For example:

```txt
{st:template_id}
```

combined with:

```cpp
value["template_id"] = static_cast<unsigned>(TemplatesID::AboutPage);
```

causes `AboutPage` to be selected from the templates made available to the render operation.

This makes the parent template generic and reusable. A page layout can therefore contain a single insertion point while the actual page content is selected by the data supplied by the application.

#### Main Templates and Sub-Templates

A template is not permanently classified as a "main template" or a "sub-template".

The distinction is based on **how the template is used in a particular render operation**:

```txt
GenericPage
    |
    +-- HomePage
```

Here, `GenericPage` is the root template because rendering starts there, while `HomePage` is a sub-template because it is selected and rendered from `GenericPage`.

The same `HomePage` template can also be rendered directly as a root template in another render operation. A template used as a sub-template may also contain its own `{st:...}` tags.

This allows arbitrary nesting:

```txt
GenericPage
    |
    +-- SignInPage
          |
          +-- SignInForm
                |
                +-- PasskeyForm
```

Each level uses the same sub-template mechanism.

#### Sub-Templates Render Into the Same Stream

A sub-template is not first rendered into a temporary string and then inserted into the parent.

Conceptually, this:

```txt
render child
    -> temporary buffer
    -> copy child result into parent
```

is replaced by:

```txt
render parent
    -> write parent output
    -> render child directly into the same stream
    -> continue parent output
```

For example:

```html
<body>
    <header>...</header>
    <main id="container">{st:template_id}</main>
    <footer>...</footer>
</body>
```

When `template_id` selects `AboutPage`, the renderer writes the header, enters the sub-template, writes the About page content directly to the same stream, and then continues with the footer.

This makes sub-templates particularly useful for server-side HTTP rendering, where an application can compose an entire response into one output stream before applying the next stage of response processing such as compression.

#### Shared Render Data

The parent template and its sub-template render using the same `Value` object.

For example, if the parent resolves:

```txt
{var:Lang[SiteName]}
```

and then reaches:

```txt
{st:template_id}
```

the selected sub-template can also resolve values from the same `Value` object.

This means application data does not need to be copied into a second value tree merely because rendering entered a sub-template.

#### Independent Loop Scope

Each sub-template render receives its own loop context. The parent and the sub-template share the same `Value` object, but their loop traversal state is independent.

This is important for nested templates because a loop variable or loop state from a parent template should not implicitly become the loop state of an unrelated sub-template render.

#### Sub-Template Availability Is Explicit

The renderer receives a collection of template definitions that may be used for sub-template resolution.

This collection does **not** need to contain every template in the application.

It only needs to contain the templates that may be selected or reached while rendering the current root template.

For example:

```cpp
TemplateDataChar sub_templates[2] = {
    {home_template, home_template_length, static_cast<unsigned>(TemplatesID::HomePage)},
    {about_template, about_template_length, static_cast<unsigned>(TemplatesID::AboutPage)}
};
```

If the current root template contains `{st:template_id}` and `template_id` can only select `HomePage` or `AboutPage`, these two definitions are sufficient.

There is no requirement to pass an application's entire template collection when most of those templates cannot be reached during the current render.

This distinction is important:

```txt
Global application templates
        |
        +-- many templates

Current render
        |
        +-- root template
        +-- templates that may be reached from it
```

The `sub_templates` collection is the **available set for the current rendering operation**, not a declaration that every template must always be present.

A nested sub-template can in turn reference another template through `{st:...}`. Consequently, the supplied set must include any template that the render can actually reach through its template-selection paths.

#### Template Cache

Template parsing is cached separately from the render data.

A `TemplateData` object describes a template using its source content, length, and ID. The corresponding `TemplateDataCache` stores the source information together with the parsed `TagBit` representation.

Conceptually:

```txt
TemplateData
    |
    +-- Content
    +-- Length
    +-- ID
          |
          v
TemplateDataCache[ID]
    |
    +-- Content
    +-- Length
    +-- Parsed Tags
```

When a template is first needed, its tags are parsed and stored in the cache. Later renders reuse the cached parsed representation rather than parsing the template again.

This is particularly useful with sub-templates because the same page component can be rendered repeatedly from different requests while retaining its parsed representation.

#### Nested Sub-Templates

Sub-template rendering is recursive.

A selected sub-template may itself contain another `{st:...}` tag:

```html
<!-- GenericPage -->
<main>
    {st:template_id}
</main>
```

The selected page could contain:

```html
<!-- SignInPage -->
<section>
    {st:form_id}
</section>
```

and the selected form could contain another sub-template:

```html
<!-- SignInForm -->
<form>
    {st:authentication_control}
</form>
```

The render tree therefore becomes:

```txt
GenericPage
    |
    +-- template_id -> SignInPage
                    |
                    +-- form_id -> SignInForm
                                      |
                                      +-- authentication_control -> Passkey
```

All of these templates can contribute to the same output stream.

The sub-template mechanism therefore provides a small template-composition system without requiring a separate layout language.

#### Invalid or Missing Template IDs

If the value referenced by `{st:...}` is not a valid numeric template ID, or the resulting ID does not correspond to an available cached template, the tag is not replaced by an arbitrary or invalid template.

Instead, Qentem falls back to rendering the original tag literally.

For example, if:

```txt
{st:template_id}
```

cannot resolve to a valid available template, the original tag remains in the generated output rather than causing the renderer to access an out-of-range template entry.

This provides a defined fallback for invalid template selection.

#### Sub-Template Rendering Example

A complete minimal example:

```cpp
#include "Qentem/JSON.hpp"
#include "Qentem/Template.hpp"
#include "Qentem/QConsole.hpp"

enum struct TemplatesID : unsigned {
    GenericPage,
    HomePage,
    AboutPage
};

int main() {
    using namespace Qentem;
    using TemplateDataChar = TemplateData<Value<char>>;

    StringStream<char> stream{};
    Array<TemplateDataCache<Value<char>>> templates_cache{};
    Value<char> value{};

    constexpr const char *generic_template = R"(
<html>
<body>
    <main id="container">{st:template_id}</main>
</body>
</html>
)";

    constexpr const char *home_template = R"(
<div>home page content</div>
)";

    constexpr const char *about_template = R"(
<div>about page content</div>
)";

    TemplateDataChar generic{
        generic_template,
        StringUtils::Count(generic_template),
        static_cast<unsigned>(TemplatesID::GenericPage)
    };

    /*
     * Only templates that may be reached by this render need to be supplied.
     * The entire application's template set is not required here.
     */
    TemplateDataChar sub_templates[2] = {
        {
            home_template,
            StringUtils::Count(home_template),
            static_cast<unsigned>(TemplatesID::HomePage)
        },
        {
            about_template,
            StringUtils::Count(about_template),
            static_cast<unsigned>(TemplatesID::AboutPage)
        }
    };

    constexpr unsigned sub_templates_count =
        sizeof(sub_templates) / sizeof(TemplateDataChar);

    // Render HomePage through GenericPage.
    value["template_id"] = static_cast<unsigned>(TemplatesID::HomePage);

    Template::Render(
        stream,
        templates_cache,
        value,
        &generic,
        sub_templates,
        sub_templates_count
    );

    stream.Clear();

    // Render AboutPage through the SAME GenericPage.
    value["template_id"] = static_cast<unsigned>(TemplatesID::AboutPage);

    Template::Render(
        stream,
        templates_cache,
        value,
        &generic,
        sub_templates,
        sub_templates_count
    );
}
```

The important part is that the root template remains unchanged:

```html
{st:template_id}
```

Only the value changes:

```cpp
value["template_id"] = static_cast<unsigned>(TemplatesID::HomePage);
```

then:

```cpp
value["template_id"] = static_cast<unsigned>(TemplatesID::AboutPage);
```

The same parent template can therefore host different child templates without producing an intermediate `PageContent` string.

#### HTTP Page Composition

Sub-templates are particularly useful for applications that have a common page layout.

A traditional page composition might look like:

```txt
About.html
    |
    v
render into temporary buffer
    |
    v
PageContent
    |
    v
GeneralPage.html
    |
    v
final response
```

The sub-template model instead allows:

```txt
GeneralPage.html
    |
    +-- {st:PageName}
             |
             v
        About.html
             |
             v
       same output stream
```

The application can therefore keep the page-specific template as a normal template while using a generic template as the response shell.

For example:

```html
<!DOCTYPE html>
<html lang="{var:Lang[LanguageCode]}" dir="{var:Lang[Direction]}">
<head>
    <title>{var:Lang[SiteName]} {if case="{var:SubPageTitle}" true="- {var:SubPageTitle}"}</title>
</head>
<body>
    <header>...</header>

    <main id="container">{st:PageName}</main>

    <footer>...</footer>
</body>
</html>
```

The application can select the page template through render data:

```txt
PageName -> AboutPage
```

or:

```txt
PageName -> SignInPage
```

while the generic layout remains unchanged.

This removes the need for a separate rendered `PageContent` value whose only purpose is to bridge one template render into another.

---

### Math Tag
```txt
{math:1+2*3}
{math:{var:a}+{var:b}}
{math:(5+3*(1+2)/2^2)}
```

Supports full arithmetic and logical evaluation inline.

**Supported operators:**
- **Arithmetic:** `+`, `-`, `*`, `/`, `%`, `^`
- **Logical:** `&&`, `||`, `==`, `!=`, `>`, `>=`, `<`, `<=`
- **Bitwise:** `&`, `|`

---

### Super Variable Tag
```txt
{svar:greeting_tpl, {var:name}, {var:place}}
```

Example:
```json
"greeting_tpl": "Welcome {0} to {1}."
```

Renders:
```txt
Welcome Alice to Qentem.
```

- Up to 10 placeholders (`{0}` through `{9}`) allowed.
- Substitution values can include `{var:...}`, `{raw:...}`, or `{math:...}` tags.

---

### Inline If Tag
```txt
{if case="{var:age} >= 18" true="Adult" false="Minor"}
```

Inline logic evaluation for conditional rendering.

- `case`: An expression or variable
- `true`/`false`: Can be variable, raw, or string literal

---

### If Condition Block
```txt
<if case="{var:score} > 90">
  Excellent!
<else if case="{var:score} > 75">
  Good job!
<else />
  Needs improvement.
</if>
```

Supports complex logic branching using `case`, `else`, and `else if`. Blocks may be nested.

**Evaluation order:**
1. Parentheses
2. Exponentiation, Remainder
3. Multiplication, Division
4. Addition, Subtraction
5. Bitwise
6. Comparisons
7. Logical `&&`, `||`

---

### Loop Tag
```txt
<loop set="products" value="product">{var:product[name]}</loop>
```

Iterates over arrays or objects.

**Optional attributes:**
- `group="field"`: Groups items by sub-key
- `sort="ascend|descend"`: Sorts items before rendering

**Nested loop example:**
```txt
<loop set="departments" value="dept">
  <loop set="dept[employees]" value="emp">
    {var:emp[name]}
  </loop>
</loop>
```

---

### Template Composition Guidelines

Sub-templates are most useful when templates have a stable outer structure and interchangeable inner content.

A common layout is:

```txt
+-------------------------------+
|          Header               |
+-------------------------------+
|                               |
|       {st:PageName}           |
|                               |
+-------------------------------+
|          Footer               |
+-------------------------------+
```

This pattern allows the application to reuse one generic page structure across many routes while selecting page-specific content at render time.

Good candidates for sub-templates include:

- Page bodies selected by a route.
- Sign-in, sign-up, and account pages sharing one site layout.
- Reusable sections that themselves select nested templates.
- Different response bodies that share a common document shell.
- Components where rendering directly into the current stream avoids a temporary rendered string.

A sub-template should generally represent a meaningful rendering unit rather than being split into extremely small pieces without a clear reason. The feature is designed for composition, not for turning every HTML element into a separate template file.

---

### Best Practices
- Always use `{var:...}` for browser-visible data to ensure escaping.
- Use `{raw:...}` sparingly with validated HTML snippets.
- Use `{st:...}` when the value represents a template selection, rather than rendering a child template into a temporary string and inserting that string with `{raw:...}`.
- Keep the `sub_templates` collection limited to templates that may actually be reached during the current render operation.
- Keep template IDs stable within the template set used by a render operation.
- Use a generic root template when multiple pages share the same document structure.
- Take advantage of nested `{st:...}` tags when a page component needs to compose another template.
- Remember that parent and sub-template renders share the same `Value` object, while loop context is independent for each sub-template render.
- Keep your template logic clean by offloading complex math to `{math:...}`.
- When formatting strings, use `{svar:...}` to reduce template verbosity.
- Prefer descriptive loop variable names to avoid confusion in nested scopes.

---

### Further Reading
- Visit [Qentem](https://github.com/Qentem/Qentem-Engine) for full source and changelogs.
- Explore [Examples](https://github.com/HaniAmmar/Qentem-Engine/tree/main/Examples) for practical usage and template composition examples.
- See `Examples/Template/Template19.cpp` for a focused example of dynamic sub-template selection and cached rendering.

