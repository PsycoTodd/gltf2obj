#ifndef GAIC_SIMPLE_LOG_H
#define GAIC_SIMPLE_LOG_H

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
         const std::string&  message);
}

#endif