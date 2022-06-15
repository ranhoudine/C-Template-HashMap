#include "HashMap.hpp"

#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
using std::string;
class InvalidKey: std::invalid_argument
{
 public:
  explicit InvalidKey(const string& what): std::invalid_argument(what) {}
};
class Dictionary : public HashMap<string, string>
{
 public:
  bool erase (const string &key);
  void update (HashMap::ConstIterator& start, const
  HashMap::ConstIterator& end );

};
bool Dictionary::erase (const string &key)
{
  if (!HashMap::erase(key))
  {
    return true;
  }
  throw InvalidKey(KEY_NOT_FOUND_MSG);
}
void
Dictionary::update (HashMap::ConstIterator &start,
                    const HashMap::ConstIterator &end)
{
  while (start != end && start.operator->() != nullptr)
  {
    insert (start->first, start->second);
    ++start;
  }
}
#endif //_DICTIONARY_HPP_
