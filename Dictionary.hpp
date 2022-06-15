#include "HashMap.hpp"
#include <iterator>
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
using std::string;
using string_iterator = std::iterator<std::forward_iterator_tag, pair<string,
string>>
class InvalidKey : std::invalid_argument
{
 public:
  explicit InvalidKey (const string &what) : std::invalid_argument (what)
  {}
};
class Dictionary : public HashMap<string, string>
{
 public:
  bool erase (const string &key) override;
  void update (string_iterator start, string_iterator end)
  {
    for (string_iterator it = start; it != end; ++it)
    {
      insert((*it).first, (*it).second);
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
