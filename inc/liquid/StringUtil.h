#pragma once

#include <string_view>

namespace liquid
{

/**
 * Case insensitive comparison of two string views.
 * @param a String view A to compare.
 * @param b String view B to compare.
 * @return True if a == b with case insensitive comparison.
 */
auto string_view_icompare(std::string_view a, std::string_view b) -> bool;

} // namespace liquid
