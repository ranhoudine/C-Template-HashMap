
#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#define MIN_CAPACITY 16
#define LOWER_LOAD_FACTOR 0.25
#define UPPER_LOAD_FACTOR 0.75
#define BAD_VECTORS_LENGTH_MSG "Constructor received vectors of different lengths"

#include <vector>
#include <stdexcept>
using std::vector;
using std::pair;
typedef int ValueT; // just for working with the ints
typedef int KeyT; // just for working with the ints
class HashMap // at first working with ints
{
 public:
  /*** Constructors and Destructors ***/

  HashMap ();
  HashMap (vector<int> keys, vector<int> values);
  HashMap (const HashMap &map);
  ~HashMap ();

  /*** Methods ***/

  int size () const;
  int capacity () const;
  bool empty () const;
  bool insert (KeyT key, ValueT value);
  bool contains_key (KeyT key) const;
  ValueT at (KeyT key);
  bool erase (KeyT key);
  double get_load_factor () const;
  int bucket_size (KeyT key) const;
  int bucket_index (KeyT key) const;
  void clear ();

  /*** Operators ***/
  HashMap &operator= (const HashMap &other);
  ValueT &operator[] (const KeyT &key);
  bool operator== (const HashMap &other);
  bool operator!= (const HashMap &other);

 private:
  unsigned int _capacity;
  unsigned int _size;
  double upper_load_factor = UPPER_LOAD_FACTOR;
  double lower_load_factor = LOWER_LOAD_FACTOR;
  vector<pair<KeyT, ValueT>> *buckets;
  std::hash<ValueT> value_hash;

  /*** private methods ***/

  void create_buckets ();
  unsigned int hash (KeyT key);
  void re_hash ();
  int first_power_of_2_larger_than (unsigned long size);
};

HashMap::HashMap () : HashMap( vector<KeyT>(), vector<ValueT>())
{}

/*** private methods ***/
void HashMap::create_buckets ()
{
  buckets = new vector<pair<KeyT, ValueT>>[_capacity];
  for (int i = 0; i < _capacity; ++i)
  {
    buckets[i] = vector<pair<KeyT, ValueT>> ();
  }
}

unsigned int HashMap::hash (KeyT key)
{
  unsigned int hash_value = value_hash (key);
  return hash_value & (_capacity - 1);
}

HashMap::HashMap (vector<KeyT> keys, vector<ValueT> values)
{
  if (keys.size () != values.size ())
  {
    throw std::length_error (BAD_VECTORS_LENGTH_MSG);
  }
  _capacity = (MIN_CAPACITY > keys.size()) ? MIN_CAPACITY :
      first_power_of_2_larger_than(keys.size());
  _size = keys.size();
  create_buckets();
  for (int i = 0; i < _size; ++i)
  {
    buckets[hash (keys[i])].push_back ({keys[i], values[i]});
  }
}

void HashMap::re_hash ()
{

}


#endif //_HASHMAP_HPP_
