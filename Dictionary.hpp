#include "HashMap.hpp"
#include <iterator>
#include <vector>
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
#define INVALID_KEY "Invalid key"
using std::string;

class InvalidKey : std::invalid_argument
{
 public:
  InvalidKey(): InvalidKey (INVALID_KEY){}
  explicit InvalidKey (const string &what) : std::invalid_argument (what)
  {}
};

class Dictionary : public HashMap<string, string>
{
 public:

  bool erase (const string &key) override;
  template<typename K>
  void update (K start, K end)
  {
    while (start != end && start.operator->() != nullptr)
    {
      insert (start->first, start->second);
      ++start;
    }
  }


};
bool Dictionary::erase (const string &key)
{
  if (!HashMap::erase (key))
  {
    return true;
  }
  throw InvalidKey (KEY_NOT_FOUND_MSG);
}


#endif //_DICTIONARY_HPP_
