
#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_

#define MIN_CAPACITY 1
#define DEFAULT_CAPACITY 16
#define LOWER_LOAD_FACTOR 0.25
#define UPPER_LOAD_FACTOR 0.75
#define BAD_VECTORS_LENGTH_MSG "Constructor received"\
" vectors of different lengths"
#define KEY_NOT_FOUND_MSG "Key not found"

#include <vector>
#include <stdexcept>
using std::vector;
using std::pair;

template<class KeyT, class ValueT>
class HashMap
{
 public:
  /*** Constructors and Destructors ***/
  HashMap (const vector<KeyT> &keys, const vector<ValueT> &values)
  {
    if (keys.size () != values.size ())
    {
      throw std::length_error (BAD_VECTORS_LENGTH_MSG);
    }
    _capacity = (DEFAULT_CAPACITY > keys.size ()) ? DEFAULT_CAPACITY :
                first_power_of_2_larger_than (keys.size ());
    _size = 0;
    create_buckets ();
    for (size_t i = 0; i < keys.size (); ++i)
    {
      operator[] (keys[i]) = values[i];
    }
  }
  HashMap (const HashMap<KeyT, ValueT> &map)
  {
    _size = map._size;
    _capacity = map._capacity;
    create_buckets ();
    for (int i = 0; i < _capacity; ++i)
    {
      buckets[i] = map.buckets[i];
    }
  }
  HashMap () : HashMap (vector<KeyT> (), vector<ValueT> ())
  {}
  virtual ~HashMap ()
  {
    delete[] buckets;
  }

  /*** Methods ***/
  ValueT &at (const KeyT &key)
  {
    unsigned int hash_value = hash (key);
    for (size_t i = 0; i < buckets[hash_value].size (); ++i)
    {
      if (buckets[hash_value].at (i).first == key)
      {
        return buckets[hash_value].at (i).second;
      }
    }
    throw std::runtime_error (KEY_NOT_FOUND_MSG);
  }
  const ValueT &at (const KeyT &key) const
  {
    unsigned int hash_value = hash (key);
    for (size_t i = 0; i < buckets[hash_value].size (); ++i)
    {
      if (buckets[hash_value].at (i).first == key)
      {
        return buckets[hash_value].at (i).second;
      }
    }
    throw std::runtime_error (KEY_NOT_FOUND_MSG);
  }
  bool insert (const KeyT &key, const ValueT &value)
  {
    if (!contains_key (key))
    {
      unsigned int hash_value = hash (key);
      buckets[hash_value].push_back ({key, value});
      _size++;
      if (beyond_upper_load_factor ())
      {
        re_hash ();
      }
      return true;
    }
    return false;
  }
  bool contains_key (const KeyT &key) const
  {
    unsigned int hash_value = hash (key);
    size_t size = buckets[hash_value].size ();
    for (size_t i = 0; i < size; ++i)
    {
      if (buckets[hash_value].at (i).first == key)
      {
        return true;
      }
    }
    return false;
  }
  double get_load_factor () const
  {
    return (double) _size / (double) _capacity;
  }
  int bucket_size (const KeyT &key) const
  {
    if (!contains_key (key))
    {
      throw std::runtime_error (KEY_NOT_FOUND_MSG);
    }
    unsigned int hash_value = hash (key);
    return (int) buckets[hash_value].size ();
  }
  virtual bool erase (const KeyT &key)
  {
    if (contains_key (key))
    {
      unsigned int hash_value = hash (key);
      for (auto it = buckets[hash_value].begin (); it < buckets[hash_value].end
          (); it++)
      {
        if (it->first == key)
        {
          buckets[hash_value].erase (it);
          _size--;
          if (below_lower_load_factor ())
          {
            re_hash ();
          }
          return true;
        }
      }
    }
    return false;
  }
  int bucket_index (const KeyT &key) const
  {
    if (contains_key (key))
    {
      unsigned int hash_value = hash (key);
      return (int) hash_value;
    }
    throw std::runtime_error (KEY_NOT_FOUND_MSG);
  }
  void clear ()
  {
    for (int i = 0; i < _capacity; ++i)
    {
      _size -= buckets[i].size ();
      buckets[i].clear ();
    }
  }
  int size () const
  {
    return _size;
  }
  int capacity () const
  {
    return _capacity;
  }
  bool empty () const
  {
    return (_size == 0);
  }

  /*** Operators ***/
  HashMap<KeyT, ValueT> &operator= (const HashMap<KeyT, ValueT> &other)
  {
    if (this == &other)
    {
      return *this;
    }
    delete[] buckets;
    _capacity = other._capacity;
    _size = other._size;
    create_buckets ();
    for (unsigned int i = 0; i < _capacity; ++i)
    {
      buckets[i] = other.buckets[i];
    }
    return *this;
  }
  ValueT &operator[] (const KeyT &key)
  {
    if (contains_key (key))
    {
      return at (key);
    }
    insert (key, ValueT ());
    return at (key);
  }
  bool operator== (const HashMap<KeyT, ValueT> &other)
  {
    if (size () != other.size ())
    { return false; }
    for (const auto &p: *this)
    {
      if (!other.contains_key (p.first))
      {
        return false;
      }
      if (other.at (p.first) != p.second)
      {
        return false;
      }
    }
    return true;
  }
  bool operator!= (const HashMap<KeyT, ValueT> &other)
  {
    return (!operator== (other));
  }

  /*** Private methods***/
 private:
  void create_buckets ()
  {
    buckets = new vector<pair<KeyT, ValueT>>[_capacity];
    for (int i = 0; i < _capacity; ++i)
    {
      buckets[i] = vector<pair<KeyT, ValueT>> ();
    }
  }
  unsigned int hash (KeyT key) const
  {
    unsigned int hash_value = value_hash (key);
    return hash_value & (_capacity - 1);
  }
  void re_hash ()
  {
    vector<pair<KeyT, ValueT>> *original_buckets = buckets;
    unsigned int original_capacity = _capacity;
    adjust_capacity_to_balance_load_factor ();
    create_buckets ();
    _size = 0;
    for (unsigned int i = 0; i < original_capacity; i++)
    {
      vector<pair<KeyT, ValueT>> v = original_buckets[i];
      for (size_t j = 0; j < v.size (); ++j)
      {
        unsigned int hash_value = hash (v[j].first);
        buckets[hash_value].push_back ({v[j].first, v[j].second});
        _size++;
      }
    }
    delete[] original_buckets;
  }
  void adjust_capacity_to_balance_load_factor ()
  {
    if (_size == 0)
    {
      _capacity = 1;
      return;
    }
    if (get_load_factor () < lower_load_factor)
    {
      while (get_load_factor () < lower_load_factor
             && _capacity > MIN_CAPACITY)
      {
        _capacity /= 2;
      }
      return;
    }
    else
    {
      while (get_load_factor () > upper_load_factor)
      {
        _capacity *= 2;
      }
    }
  }
  bool beyond_upper_load_factor ()
  {
    return get_load_factor () > upper_load_factor;
  }
  bool below_lower_load_factor ()
  {
    return (get_load_factor () < lower_load_factor);
  }
  int first_power_of_2_larger_than (int size)
  {
    int num = 1;
    while (num < size)
    {
      num *= 2;
    }
    return num;
  }
  /*** Forward declarations ***/
 public:
  class ConstIterator;

  /*** Typedefs***/

  typedef const pair<KeyT, ValueT> value_type;
  typedef const pair<KeyT, ValueT> &reference;
  typedef const pair<KeyT, ValueT> *pointer;
  typedef std::ptrdiff_t difference_type;
  typedef std::forward_iterator_tag iterator_category;

  typedef ConstIterator const_iterator;

  /*** Begins and Ends ***/
  const_iterator begin () const
  {
    if (empty ())
    { return end (); }
    int bucket_index = 0;
    while (buckets[bucket_index].empty ())
    {
      bucket_index++;
    }
    return ConstIterator (&(buckets[bucket_index][0]), this, 0, bucket_index);
  }
  const_iterator end () const
  {
    return HashMap<KeyT, ValueT>::const_iterator (nullptr, this, 0, 0);
  }
  const_iterator cbegin () const
  {
    if (empty ())
    { return end (); }
    int bucket_index = 0;
    while (buckets[bucket_index].empty ())
    {
      bucket_index++;
    }
    return ConstIterator (&(buckets[bucket_index][0]), this, 0, bucket_index);
  }
  const_iterator cend () const
  {
    return HashMap<KeyT, ValueT>::const_iterator (nullptr, this, 0, 0);
  }

 private:
  int _capacity;
  int _size;
  double upper_load_factor = UPPER_LOAD_FACTOR;
  double lower_load_factor = LOWER_LOAD_FACTOR;
  vector<pair<KeyT, ValueT>> *buckets;
  std::hash<KeyT> value_hash;

 public:
  class ConstIterator
  {
   public:
    /*** Constructors ***/
    ConstIterator (const pair<KeyT, ValueT> *_p,
                   const HashMap<KeyT, ValueT> *map, int i_w,
                   int b_i) : p (_p), associated_map (map),
                              index_within_bucket
                                  (i_w), bucket_index (b_i)
    {}

    /*** Operators ***/

    const_iterator &operator++ ()
    {
      if ((int) index_within_bucket + 1
          < associated_map->bucket_size (p->first))
      {
        index_within_bucket++;
        p = &(associated_map->buckets[bucket_index][index_within_bucket]);
        return *this;
      }
      index_within_bucket = 0;
      bucket_index++;
      while (bucket_index < associated_map->_capacity &&
             associated_map->buckets[bucket_index].empty ())
      {
        bucket_index++;
      }
      if (bucket_index < associated_map->_capacity)
      {
        p = &(associated_map->buckets[bucket_index][index_within_bucket]);
        return *this;
      }
      this->p = nullptr;
      this->bucket_index = 0;
      this->index_within_bucket = 0;
      return *this;
    }
    const_iterator operator++ (int)
    {
      HashMap<KeyT, ValueT>::ConstIterator it (p, associated_map,
                                               index_within_bucket,
                                               bucket_index);
      if (index_within_bucket + 1 < associated_map->bucket_size (p->first)) //
        // if it's not the last pair in the bucket
      {
        index_within_bucket++;
        p = &(associated_map->buckets[bucket_index][index_within_bucket]);
        return it;
      }
      index_within_bucket = 0;
      bucket_index++;
      while (bucket_index < associated_map->_capacity &&
             associated_map->buckets[bucket_index].empty ())
      {
        bucket_index++;
      }
      if (bucket_index + 1 <= associated_map->_capacity)
      {
        p = &(associated_map->buckets[bucket_index][index_within_bucket]);
        return it;
      }
      this->p = nullptr;
      this->bucket_index = 0;
      this->index_within_bucket = 0;
      return it;
    }
    bool operator== (const HashMap<KeyT, ValueT>::ConstIterator &other)
    {
      return p == other.p && associated_map == other.associated_map;
    }
    bool operator!= (const HashMap<KeyT, ValueT>::ConstIterator &other)
    {
      return !(operator== (other));
    }
    reference operator* ()
    {
      return *p;
    }
    pointer operator-> ()
    {
      return p;
    }

   private:
    const pair<KeyT, ValueT> *p;
    const HashMap<KeyT, ValueT> *associated_map;
    int index_within_bucket;
    int bucket_index;

  };
};
#endif //_HASHMAP_HPP_
