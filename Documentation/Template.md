## Qentem Template Engine — Advanced Documentation

### Overview
Qentem Engine's templating system is a high-performance, lightweight solution designed to dynamically generate structured content such as HTML, JSON, or plaintext. It provides flexible data access, inline logic evaluation, math expressions, and powerful looping—all embedded directly in your templates.

This guide thoroughly documents all supported tags and features, offering syntax examples, behavior details, and best practices.

---

### Tags Summary

| Tag | Purpose | Supports |
|------|---------|----------|
| `{var:...}` | Access and safely render a value (with HTML escaping). | Nested keys/indexes, strings, numbers |
| `{raw:...}` | Render raw (unescaped) values directly. | Same as `var`, but no escaping |
| `{math:...}` | Perform arithmetic, logic, or bitwise operations. | Numbers, variables, nested expressions |
| `{svar:template, ...}` | Substitutes `{0}` to `{9}` in a string template. | Up to 10 variables, expressions, or raw values |
| `{if case="..." true="..." false="..."}` | Inline conditional rendering. | Variables, math, constants |
| `<if case="...">...</if>` | Conditional block with full logic flow. | `elseif`, `else`, nested blocks |
| `<loop set="..." value="..." group="..." sort="...">...</loop>` | Iterates over collections with sorting/grouping. | Arrays, objects |

**Legend:**
- `...` means an expression, key, or identifier.
- Supports single or double-quoted attribute values.

---

### Variable Tag
```txt
{var:key}            // Access top-level key
{var:user[name]}     // Nested object
{var:items[0][price]} // Indexed array access
```
Safely outputs data by resolving the provided path. HTML-sensitive characters are escaped.

---

### Raw Variable Tag
```txt
{raw:html_block}
{raw:user[signature]}
```
Outputs raw data without escaping. Only use this with trusted inputs like HTML fragments.

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

### Best Practices
- Always use `{var:...}` for browser-visible data to ensure escaping.
- Use `{raw:...}` sparingly with validated HTML snippets.
- Keep your template logic clean by offloading complex math to `{math:...}`.
- When formatting strings, use `{svar:...}` to reduce template verbosity.
- Prefer descriptive loop variable names to avoid confusion in nested scopes.

---

### Further Reading
- Visit [Qentem](https://github.com/Qentem/Qentem-Engine) for full source and changelogs.
- Explore [Examples](https://github.com/HaniAmmar/Qentem-Engine/tree/main/Examples) for practical usage demonstrations and real-world template applications.
