#include <iostream>

namespace gaic
{
enum class log_level : char
{
  Info = 'I',
  Warning = 'W',
  Error = 'E'
};

void log(log_level const level,
         std::string_view const message,
         std::source_location const source = std::source_location const source = std::source_location::current());
}