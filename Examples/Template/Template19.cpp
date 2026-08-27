#include "Qentem/JSON.hpp"
#include "Qentem/Template.hpp"
#include "Qentem/QConsole.hpp"

/*
 * Sub-Template Rendering Example
 * ==============================
 *
 * Build:
 *
 *     mkdir Build
 *     c++ -O3 ./Examples/Template/Template19.cpp \
 *         -I ./Include \
 *         -o ./Build/QTest.bin
 *
 * Run:
 *
 *     ./Build/QTest.bin
 *
 * --------------------------------------------------------------------------
 *
 * This example demonstrates Qentem's Sub-Template Tag:
 *
 *     {st:template_id}
 *
 * A sub-template tag does NOT insert an already-rendered string.
 * Instead, it selects another template and renders that template directly
 * into the current output stream.
 *
 * This distinction is important.
 *
 * Traditional composition often looks like this:
 *
 *     Render child template
 *             |
 *             v
 *       temporary buffer
 *             |
 *             v
 *       PageContent string
 *             |
 *             v
 *     Render parent template
 *
 * Qentem's sub-template rendering instead does this:
 *
 *     Render parent template
 *             |
 *             v
 *        encounter {st:template_id}
 *             |
 *             v
 *       select child template
 *             |
 *             v
 *       render child directly
 *       into the SAME stream
 *             |
 *             v
 *       continue rendering parent
 *
 * Therefore, no intermediate rendered HTML string is required.
 *
 * --------------------------------------------------------------------------
 *
 * Template Selection
 * ------------------
 *
 * The syntax:
 *
 *     {st:template_id}
 *
 * means:
 *
 *     1. Resolve "template_id" from the current Value object.
 *     2. Interpret the resulting number as a template ID.
 *     3. Find that template in the supplied template cache.
 *     4. Render the selected template at this exact position.
 *
 * In this example:
 *
 *     TemplatesID::GenericPage
 *     TemplatesID::HomePage
 *     TemplatesID::AboutPage
 *
 * are the template IDs.
 *
 * The actual template selected by {st:template_id} is controlled by:
 *
 *     value["template_id"]
 *
 * This means the GenericPage template does not need to know whether its
 * content is HomePage, AboutPage, SignInPage, or any other page.
 *
 * The page selection is supplied by the render data.
 *
 * --------------------------------------------------------------------------
 *
 * Main Template and Sub-Templates
 * -------------------------------
 *
 * The distinction is contextual:
 *
 *     GenericPage
 *
 * is the root template because rendering starts there.
 *
 *     HomePage
 *     AboutPage
 *
 * are sub-templates because they are rendered from GenericPage.
 *
 * They are still complete templates. There is nothing intrinsically
 * "sub-template-only" about them.
 *
 * A template used as a sub-template can itself contain another {st:...}
 * tag and therefore render another template.
 *
 * This allows nesting such as:
 *
 *     GenericPage
 *         |
 *         +-- HomePage
 *                 |
 *                 +-- SomeNestedTemplate
 *
 * --------------------------------------------------------------------------
 *
 * TemplateData vs TemplateDataCache
 * ----------------------------------
 *
 * TemplateData describes a template supplied to the renderer:
 *
 *     Content
 *     Length
 *     ID
 *
 * TemplateDataCache stores the cached parsed representation of that
 * template:
 *
 *     Content
 *     Length
 *     Parsed TagBit array
 *
 * The template source is parsed only when its cache entry is first needed.
 * Subsequent renders reuse the parsed tags.
 *
 * Therefore:
 *
 *     TemplateData
 *          |
 *          v
 *     TemplateDataCache
 *          |
 *          v
 *     Parse once
 *          |
 *          v
 *     Render many times
 *
 * --------------------------------------------------------------------------
 *
 * Shared Render Data
 * ------------------
 *
 * Parent and sub-templates receive the same Value object.
 *
 * This means a value such as:
 *
 *     value["template_id"]
 *
 * is visible to both the parent and the selected sub-template.
 *
 * The loop state, however, is local to each TemplateCore render operation.
 * A sub-template therefore gets its own loop context while still sharing
 * the parent render's Value object.
 *
 * --------------------------------------------------------------------------
 *
 * Shared Output Stream
 * --------------------
 *
 * The most important property for server-side rendering is that the child
 * template receives the same StringStream as the parent.
 *
 * For example:
 *
 *     GenericPage:
 *
 *         <main>{st:template_id}</main>
 *
 * and template_id == AboutPage
 *
 * produces:
 *
 *     <main>
 *         <div>about page content</div>
 *     </main>
 *
 * without first rendering AboutPage into a separate string.
 *
 * This is what makes sub-templates useful for HTTP response composition:
 *
 *     template rendering
 *             |
 *             v
 *       one output stream
 *             |
 *             v
 *          compression
 *             |
 *             v
 *         HTTP output
 *
 * --------------------------------------------------------------------------
 *
 * Missing / Invalid Template IDs
 * ------------------------------
 *
 * If {st:...} resolves to a value that is not a valid template ID, Qentem
 * falls back to rendering the original tag literally rather than attempting
 * to render an invalid template.
 *
 * See Template.hpp for the complete Sub-Template Tag behavior.
 */

////////////////////////////////////////////////////////////////////
enum struct TemplatesID : unsigned { GenericPage, HomePage, AboutPage };
////////////////////////////////////////////////////////////////////

int main() {
    using namespace Qentem;

    /*
     * TemplateData<Value<char>> describes one renderable template.
     *
     * The template ID is the identity used by {st:...} to select a
     * sub-template.
     */
    using TemplateDataChar = TemplateData<Value<char>>;

    /*
     * All templates render into this one stream.
     *
     * Notice that there is no separate stream for HomePage or AboutPage.
     * When GenericPage reaches {st:template_id}, the selected page is rendered
     * directly into this same stream.
     */
    StringStream<char> stream{};

    /*
     * Parsed template cache.
     *
     * The cache is indexed by TemplatesID.
     *
     * Each entry contains the source pointer/length and, most importantly,
     * the parsed TagBit array.
     *
     * A template is parsed only once and can then be rendered repeatedly.
     */
    Array<TemplateDataCache<Value<char>>> templates_cache{};

    /*
     * Render data shared by the parent template and its sub-template.
     *
     * template_id is the selector used by:
     *
     *     {st:template_id}
     *
     * The value does not contain HTML.
     * It contains the ID of the template that should be rendered there.
     */
    Value<char> value{};

    ////////////////////////////////////////////////////////////////
    // Generic/main page template
    ////////////////////////////////////////////////////////////////

    /*
     * GenericPage is the root template.
     *
     * It owns the common document structure and provides a slot where
     * another template can be rendered.
     *
     * The important line is:
     *
     *     {st:template_id}
     *
     * The actual template rendered there is determined at runtime from
     * value["template_id"].
     *
     * GenericPage therefore does not need separate versions such as:
     *
     *     GenericHomePage
     *     GenericAboutPage
     *     GenericSignInPage
     *
     * One generic template can host all of them.
     */
    constexpr const char *generic_template = R"(
<html>
<body>
    <main id="container">{st:template_id}</main>
</body>
</html>
)";

    constexpr SizeT generic_template_length = StringUtils::Count(generic_template);

    ////////////////////////////////////////////////////////////////
    // Home page sub-template
    ////////////////////////////////////////////////////////////////

    /*
     * This template contains only the page-specific content.
     *
     * It does not need to know anything about GenericPage.
     */
    constexpr const char *home_template = R"(
        <div>home page content</div>
    )";

    constexpr SizeT home_template_length = StringUtils::Count(home_template);

    ////////////////////////////////////////////////////////////////
    // About page sub-template
    ////////////////////////////////////////////////////////////////

    /*
     * This template is structurally independent from GenericPage.
     *
     * It can be rendered as a sub-template here, or it can be used as
     * the root template in a completely different render operation.
     */
    constexpr const char *about_template = R"(
        <div>about page content</div>
    )";

    constexpr SizeT about_template_length = StringUtils::Count(about_template);

    ////////////////////////////////////////////////////////////////
    // Template definitions
    ////////////////////////////////////////////////////////////////

    /*
     * GenericPage is the root/main template for this example.
     */
    TemplateDataChar generic{generic_template, generic_template_length,
                             static_cast<unsigned>(TemplatesID::GenericPage)};

    /*
     * These are the templates made available to this render operation for
     * sub-template resolution.
     *
     * The array does NOT need to contain every template in the application.
     * It only needs to contain the templates that may be selected or reached
     * while rendering the current root template.
     *
     * For example, GenericPage contains:
     *
     *     {st:template_id}
     *
     * and template_id may resolve to either HomePage or AboutPage. Therefore those
     * two templates are supplied here.
     *
     * Their IDs are the values that can be stored in:
     *
     *     value["template_id"]
     *
     * For example:
     *
     *     value["template_id"] = TemplatesID::HomePage;
     *
     * causes {st:template_id} to select and render HomePage.
     *
     * Templates that cannot be reached during this render do not need to be
     * supplied. This allows the caller to provide only the templates relevant
     * to the current render operation instead of maintaining or passing the
     * complete application's template set.
     */
    TemplateDataChar sub_templates[] = {
        {home_template, home_template_length, static_cast<unsigned>(TemplatesID::HomePage)},
        {about_template, about_template_length, static_cast<unsigned>(TemplatesID::AboutPage)}};

    constexpr unsigned sub_templates_count = (sizeof(sub_templates) / sizeof(TemplateDataChar));

    ////////////////////////////////////////////////////////////////
    // Render HomePage through GenericPage
    ////////////////////////////////////////////////////////////////

    /*
     * Select HomePage.
     *
     * GenericPage itself is unchanged.
     * Only the render data changes.
     *
     * GenericPage:
     *
     *     <main id="container">{st:template_id}</main>
     *
     * template_id:
     *
     *     HomePage
     *
     * therefore becomes:
     *
     *     <main id="container">
     *         [HomePage rendered here]
     *     </main>
     *
     * Everything is written into the same stream.
     */
    value["template_id"] = static_cast<unsigned>(TemplatesID::HomePage);

    Template::Render(stream, templates_cache, value, &generic, &(sub_templates[0]), sub_templates_count);

    QConsole::Print(stream, '\n');
    QConsole::Print("----------------------------\n");

    ////////////////////////////////////////////////////////////////
    // Render AboutPage through the SAME GenericPage
    ////////////////////////////////////////////////////////////////

    /*
     * Clear only the output stream.
     *
     * The template cache is deliberately NOT cleared.
     *
     * This is important:
     *
     *     GenericPage, HomePage and AboutPage were parsed during the first
     *     render when their cache entries were needed.
     *
     * The second render reuses those parsed TagBit arrays.
     *
     * In other words:
     *
     *     first render  -> parse + render
     *     later renders -> render only
     */
    stream.Clear();

    /*
     * Select AboutPage instead.
     *
     * The root template remains GenericPage.
     *
     * Only the template selected by {st:template_id} changes.
     */
    value["template_id"] = static_cast<unsigned>(TemplatesID::AboutPage);

    Template::Render(stream, templates_cache, value, &generic, &sub_templates[0], sub_templates_count);

    QConsole::Print(stream, '\n');
}
