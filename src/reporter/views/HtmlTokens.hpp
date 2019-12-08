#ifndef HTMLTOKENS_HPP
#define HTMLTOKENS_HPP

namespace reporter {
namespace views {
static constexpr char kTableStart[] = "<table>";
static constexpr char kTableRowStart[] = "<tr>";
static constexpr char kTableHeaderStart[] = "<th>";
static constexpr char kTableColumnStart[] = "<td>";

static constexpr char kTableEnd[] = "</table>";
static constexpr char kTableRowEnd[] = "</tr>";
static constexpr char kTableHeaderEnd[] = "</th>";
static constexpr char kTableColumnEnd[] = "</td>";

namespace defaultclass {

static constexpr char kTableStart[] = "<table class=default>";
static constexpr char kTableRowStart[] = "<tr class=default>";
static constexpr char kTableHeaderStart[] = "<th class=default>";
static constexpr char kTableColumnStart[] = "<td class=default>";

}  // namespace defaultclass
}  // namespace views
}  // namespace reporter

#endif  // HTMLTOKENS_HPP
