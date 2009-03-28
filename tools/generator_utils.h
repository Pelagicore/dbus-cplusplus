#ifndef __DBUSXX_TOOLS_GENERATOR_UTILS_H
#define __DBUSXX_TOOLS_GENERATOR_UTILS_H

#include <string>
#include <sstream>
#include <iomanip>

const char *atomic_type_to_string(char t);
std::string stub_name(std::string name);
std::string signature_to_type(const std::string &signature);
void _parse_signature(const std::string &signature, std::string &type, unsigned int &i);
void underscorize(std::string &str);

/// create std::string from any number
template <typename T>
std::string toString (const T &thing, int w = 0, int p = 0)
{
  std::ostringstream os;
  os << std::setw(w) << std::setprecision(p) << thing;
  return os.str();
}

#endif//__DBUSXX_TOOLS_GENERATOR_UTILS_H
