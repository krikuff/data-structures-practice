/**
 * Class CircularArray defines the data structure which can be used as a simple
 * array. Also it provides O(1) time for insertion at the front or back for
 * most time (O(n) in the worst case), O(1) for remove operation at the front 
 * or back.
 */

#ifndef COBSEA_LIBS_CIRCULAR_ARRAY_H_
#define COBSEA_LIBS_CIRCULAR_ARRAY_H_

namespace cobsea_libs {

template <typename ValueType>
class CircularArray {
public:
  CircularArray();

  /**
   * Allocates space for _capacity elements in CircularArray
   */
  explicit CircularArray(size_t _capacity);

  /**
   * Stores _capacity elements in CircularArray with value _val.
   * Be careful with RAII classes
   */
  CircularArray(size_t _capacity, ValueType _val);

  CircularArray(CircularArray<ValueType> const & other);
  CircularArray(CircularArray<ValueType> const && other) noexcept;

  ~CircularArray();

  void PushFront(ValueType _val);
  void PushBack(ValueType _val);

  /**
   * Removes the first element (no return)
   */
  void PopFront();

  /**
   * Removes the last element (no return)
   */
  void PopBack();

  /**
   * Returns the first element
   */
  ValueType& Front() const;

  /**
   * Returns the last element
   */
  ValueType& Back() const;

  /**
   * Adds _val at _pos index of the container. Further elements will be moved
   * forward. No check of index correctness
   */
  void InsertAt(ValueType _val, size_t _pos);

  /**
   * Removes element from a given position. No check of index correctness.
   */
  void RemoveAt(size_t _pos);

  /**
   * Returns number of elements in the container
   */
  size_t Size() const;

  /**
   * Returns size of the allocated array
   */
  size_t Capacity() const;

  /**
   * Returns true if CircularArray is empty
   */
  bool Empty() const;

  /**
   * Gives access to any arbitrary element in the container
   */
  ValueType& operator[](size_t _index) const;

  class iterator : public std::iterator<std::random_access_iterator_tag, ValueType>
  {
  public:
    friend class CircularArray<ValueType>;

    typedef ValueType value_type;
    typedef ValueType& reference;
    typedef ValueType* pointer;
    typedef ValueType difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    CircularArray<value_type>* array_ptr;
    size_t idx;

    iterator (CircularArray<value_type>* _ptr, size_t _idx) : 
      array_ptr(_ptr),
      idx(_idx)
    {

    };

    bool operator== (iterator const& other) const 
    {
      return array_ptr == other.array_ptr && idx == other.idx;
    };

    bool operator!= (iterator const& other) const 
    {
      return array_ptr != other.array_ptr || idx != other.idx;
    };

    reference operator*() const 
    {
      return (*array_ptr)[idx];
    };

    iterator& operator++() 
    {
      idx++;
      return *this;
    };

    iterator& operator++(int) 
    {
      iterator tmp(array_ptr, idx);
      idx++;
      return tmp;
    };

    iterator& operator--() 
    {
      idx--;
      return *this;
    };

    iterator& operator--(int) 
    {
      iterator tmp(array_ptr, idx);
      idx--;
      return tmp;
    };
  };

  iterator begin()
  {
    return iterator(this, 0);
  };

  iterator end() 
  {
    return iterator(this, size_);
  };

private:
  void ExtandArray();

  static constexpr size_t kDefaultCapacity = 20;

  ValueType *data_array_;
  size_t head_,
         tail_,
         size_,
         capacity_;
}; // class CircularArray

// Definitions

template <typename ValueType>
CircularArray<ValueType>::CircularArray() :
  data_array_ (nullptr),
  head_ (0),
  tail_ (0),
  size_ (0),
  capacity_ (0)
{

};

template <typename ValueType>
CircularArray<ValueType>::CircularArray(size_t _capacity) :
  data_array_ (new ValueType[_capacity]),
  head_ (0),
  tail_ (0),
  size_ (0),
  capacity_ (_capacity)
{

};

template <typename ValueType>
CircularArray<ValueType>::CircularArray(size_t _capacity, ValueType _val) :
  data_array_ (new ValueType[_capacity]),
  head_ (0),
  tail_ (_capacity - 1),
  size_ (_capacity),
  capacity_ (_capacity)
{
  for (size_t i = 0; i < _capacity; i++)
    data_array_[i] = _val;
};

template <typename ValueType>
CircularArray<ValueType>::CircularArray(CircularArray<ValueType> const & other) :
  data_array_ (new ValueType[other.size_]),
  head_ (0),
  tail_ (other.size_ - 1),
  size_ (other.size_),
  capacity_ (other.size_)
{
  for (size_t i = 0; i < other.size_; i++) {
    data_array_[i] = other[i];
  }
};

template <typename ValueType>
CircularArray<ValueType>::CircularArray(CircularArray<ValueType> const && other) noexcept :
  data_array_ (other.data_array_),
  head_ (other.head_),
  tail_ (other.tail_),
  size_ (other.size_),
  capacity_ (other.capacity_)
{
  other.data_array_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
};

template <typename ValueType>
CircularArray<ValueType>::~CircularArray()
{
  if (data_array_ != nullptr)
    delete [] data_array_;
};

template <typename ValueType>
void CircularArray<ValueType>::PushFront(ValueType _val)
{
  if (size_ == capacity_) {
    ExtandArray();
  };
  if (head_ == 0)
    head_ = capacity_ - 1;
  else
    head_--;
  data_array_[head_] = _val;
  size_++;
};

template <typename ValueType>
void CircularArray<ValueType>::PushBack(ValueType _val)
{
  if (size_ == capacity_)
    ExtandArray();
  data_array_[tail_] = _val;
  tail_ = (tail_ + 1) % capacity_;
  size_++;
};

template <typename ValueType>
void CircularArray<ValueType>::PopFront()
{
  head_ = (head_ + 1) % capacity_;
  size_--;
};

template <typename ValueType>
void CircularArray<ValueType>::PopBack()
{
  if (tail_ == 0)
    tail_ = capacity_ - 1;
  else
    tail_--;
  size_--;
};

template <typename ValueType>
ValueType& CircularArray<ValueType>::Front() const
{
  return data_array_[head_];
};

template <typename ValueType>
ValueType& CircularArray<ValueType>::Back() const
{
  return data_array_[tail_ != 0 ? tail_ - 1 : capacity_ - 1];
};

template <typename ValueType>
void CircularArray<ValueType>::InsertAt(ValueType _val, size_t _pos)
{
  if (size_ == capacity_)
    ExtandArray();
  for (size_t i = size_; i > _pos; i--)
    data_array_[(head_ + i) % capacity_] = (*this)[i - 1];
  data_array_[(head_ + _pos) % capacity_] = _val;
  tail_ = (tail_ + 1) % capacity_;
  size_++;
};

template <typename ValueType>
void CircularArray<ValueType>::RemoveAt(size_t _pos)
{
  for (size_t i = _pos; i < size_; i++)
    data_array_[(head_ + i) % capacity_] = (*this)[i + 1];
  PopBack();
};

template <typename ValueType>
size_t CircularArray<ValueType>::Size() const
{
  return size_;
};

template <typename ValueType>
size_t CircularArray<ValueType>::Capacity() const
{
  return capacity_;
};

template <typename ValueType>
bool CircularArray<ValueType>::Empty() const
{
  return size_ == 0;
};

template <typename ValueType>
ValueType& CircularArray<ValueType>::operator[] (size_t _index) const
{
  return data_array_[(head_ + _index) % capacity_];
};

template <typename ValueType>
void CircularArray<ValueType>::ExtandArray()
{
  if (capacity_ == 0)
    capacity_ = 1;
  ValueType *new_array = new ValueType[capacity_ * 2];
  for (size_t i = 0; i < size_; i++)
    new_array[i] = data_array_[(head_ + i) % capacity_];

  if (data_array_ != nullptr)
    delete [] data_array_;
  data_array_ = new_array;
  head_ = 0;
  tail_ = size_;
  capacity_ *= 2;
};

}; // namespace cobsea_libs

#endif // HOTCHOCOLATEHANG_LIBS_CIRCULAR_ARRAY_H_