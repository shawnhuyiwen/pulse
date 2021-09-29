#include "PulseBuildInformation.h"
#include "PulseBuildConfiguration.h"

#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>

namespace {
  std::string dateTimeCode(std::string date, std::string time)
  {
    std::tm t = {};
    auto in = std::istringstream(date + " " + time);
    in.imbue(std::locale("en-US"));
    in >> std::get_time(&t, "%b %d %Y %H:%M:%S");
  
    auto out = std::ostringstream();
    out.imbue(std::locale("en-US"));
    out << std::put_time(&t, "%Y%m%d%H%M%S");
    return out.str();
  }
}

std::string PulseBuildInformation::time()
{
    return dateTimeCode(__DATE__, __TIME__);
}

std::string PulseBuildInformation::hash()
{
    return PULSE_GIT_HASH;
}

std::string PulseBuildInformation::version()
{
    return PULSE_VERSION;
}
