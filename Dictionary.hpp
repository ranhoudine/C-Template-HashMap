#include "HashMap.hpp"
#include <iterator>
#include <vector>
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
#define INVALID_KEY "Invalid key"
using std::string;

class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey () : InvalidKey (INVALID_KEY)
  {}
  explicit InvalidKey (const string &what) : std::invalid_argument (what)
  {}
};

class Dictionary : public HashMap<string, string>
{
 public:
  Dictionary (){}
  Dictionary (const vector<string>& keys, const vector<string>& values) :
  HashMap<string,string> (keys, values)
  {}

  /***
   * overrides erase of hashmap. throws an error if the key is not found
   * @param key
   * @return
   */
  bool erase (const string &key) override
  {
    if (HashMap::erase (key))
    {
      return true;
    }
    throw InvalidKey (KEY_NOT_FOUND_MSG);
  }
  /***
   * function template for updating the dictionary using two iterators of
   * <string, string> pairs.
   * @tparam K
   * @param start
   * @param end
   */
  template<typename K>
  void update (K start, K end)
  {
    while (start != end && start.operator-> () != nullptr)
    {
      operator[](start->first) = (start->second);
      ++start;
    }
  }

};


#endif //_DICTIONARY_HPP_
