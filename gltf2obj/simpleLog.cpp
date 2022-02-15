#include "simpleLog.h"
#include <string>

namespace gaic
{

void log(log_level const level,
         const std::string& message)
{
  std::cout << "[ " << static_cast<char>(level) << " ] " << message << std::endl;
}

}