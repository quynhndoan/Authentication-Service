#ifndef HASHFUNCTION_HPP
#define HASHFUNCTION_HPP

#include <functional>
#include <string>

typedef std::function<unsigned int(const std::string &key)> Hashfunction;

unsigned int PJWHash(const std::string &key);

#endif
