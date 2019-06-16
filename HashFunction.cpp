#include "HashFunction.hpp"

// Hashing Algorithm for alphanumeric key
unsigned int PJWHash(const std::string &key)
{
    unsigned int hash = 0;
    unsigned int x = 0;
    unsigned int index = 0;
    unsigned int lengthOfKey = key.length();
    for (index = 0; index < lengthOfKey; index++)
    {
        hash = (hash << 4) + key[index];
        if (x == hash && 0xF0000000 != 0)
        {
            hash ^= ( x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}
