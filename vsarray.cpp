/**
 * Title:		Lab 02 - vsarray.cpp
 * Purpose:		Implements all the methods of an array list.
 * Author:		Tracy Mai
 * Date:		April 12, 2023
 */

#include "object.h"
#include "list.h"
#include "vsarray.h"

#include <tgmath.h>
#include <string>
#include <sstream>

using std::stringstream;

// Implement ALL methods of VSArray class

/**
 * VS Array Copy Constructor
 * Makes a deep copy of the other array by traversing the other array
 * list and cloning and inserting it into our list. By using the Clone function
 * from the Object class, we can ensure that we are making a deep copy instead of a shallow copy
 * @param other array
 * @return: nothing
 */
VSArray::VSArray(const VSArray& other): _capacity(other._capacity) {
    _size = other._size;
    _data = new Object*[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = other.Get(i)->Clone();
    }

    for (size_t i = _size; i < _capacity; i++) {
        _data[i] = nullptr;
    }
}

/**
 * VS Array Constructor
 * Constructs a new empty array, and has the capacity and its size increased
 * prepared to be used when necessary
 * @param the capacity of the array and increasePercentage for how much larger
 * it should be increased by
 * @return nothing
 */
VSArray::VSArray(size_t capacity, double increasePercentage): _capacity(capacity), _delta(increasePercentage) {
    _data = new Object*[_capacity];
}

/**
 * VS Array Copy Assignment Operator
 * Enables the deep copy assignment with this operator
 * @param rhs
 * @return *this to make copies
 */
const VSArray& VSArray::operator=(const VSArray& rhs) {
    if (&rhs == this) {
        return *this;
    }

    Clear();
    delete[] _data;
    _size = rhs._size;
    _capacity = rhs._capacity;
    _data = new Object*[_capacity];
    for (size_t i = 0; i < _size; ++i) {
        _data[i] = rhs.Get(i)->Clone();
    }

    for (size_t i = _size; i < _capacity; ++i) {
        _data[i] = nullptr;
    }

    return *this;
}

/**
 * VS Array Destructor
 * Deletes the array by deleting every data in the indices, then the pointer
 * to the array itself
 * @param none
 * @return none
 */
VSArray::~VSArray() {
    for (size_t i = 0; i < _size; i++) {
        delete _data[i];
    }
    delete[] _data;
}

/**
 * Resize
 * Creates a new array 1.5 times the original capacity and move the objects
 * from the original array to the newly created one. Then deletes the old array
 * and makes the original array point to the newly created and resized array
 * @param none
 * @return none
 */
void VSArray::Resize() {

    size_t newCapacity = static_cast<size_t>(ceil(_capacity * (1 + _delta)));

    Object **resized = new Object*[newCapacity];

    for (size_t i = 0; i <_size; ++i) {
        resized[i] = _data[i]->Clone();
    }

    for (size_t i = _size; i < newCapacity; ++i) {
        resized[i] = nullptr;
    }

    /**
     * Deleting and deallocating the old array
     */
    for (size_t i = 0; i < _size; ++i) {
        delete _data[i];
    }

    delete[] _data;
    _data = resized;
    _capacity = newCapacity;
}

/**
 * Insert
 * Inserts the given element into the given position, shifts the array and
 * increases the size as necessary.
 * Calls the Resize function if the array size is equal to the capacity.
 * Things to consider: if the position is greater than the size, of which will
 * return false
 * @param the element to be inserted, and the position it will be inserted at
 * @return a bool depending on whether or not the insertion was successful
 */
bool VSArray::Insert(Object* element, size_t position) {
    if (position > _size) {
        return false;
    }

    if (_size == _capacity) {
        Resize();
    }

    if (position == _size) {
        _data[position] = element;
    } else {

        /**
         * Shifts the elements from the end of the array over one index if the
         * element is being inserted into a position that already has an object
         * in it.
         */
        for (size_t i = _size-1; i >= position && i < _size; --i) {
            _data[i+1] = _data[i];
        }
        _data[position] = element;
    }
    ++_size;
    return true;
}

/**
 * Index Of
 * Traverses through the array to find the index of the element given if it exists
 * Cases to consider: if the element given is nullptr or if the element cannot
 * be found then it will return -1,
 * @param the element we will be searching for
 * @return the index that the element is at in the array, and -1 if it cannot
 * be found
 */
int VSArray::IndexOf(const Object* element)const {

    if (element == nullptr) {
        return -1;
    }

    for (size_t i = 0; i < _size; ++i) {
        if (_data[i]->Equals(*element)) { //
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * Remove
 * Removes and deallocates the element at the position, and shifts over the rest of the array
 * Then decreases the size, if the remove was successful
 * @param position that the element will be removed
 * @return the object that was removed or nullptr if it was unsuccessful
 */
Object* VSArray::Remove(size_t position) {

    if (position > _size-1) {
        return nullptr;
    }
    Object* retVal = _data[position];
    _data[position] = nullptr;
    delete _data[position];

    /**
     * Shifts the element to the left after it was removed
     */
    for (size_t i = position + 1; i < _size; i++) {
          _data[i-1] = _data[i];
    }
    --_size;
    return retVal;
}

/**
 * Get
 * Gets the object at the given position
 * Cases to consider: if the position was greater than the size,
 * then return nullptr
 * @param the position to receive the object from
 * @return the object at the position, or nullptr if the position does not exist
 */
Object* VSArray::Get(size_t position)const {
    if (position > _size) {
        return nullptr;
    }
    return _data[position];
}

/**
 * To String
 * Prints out the array in a string format
 * @param none
 * @return the array as a string format
 */

string VSArray::ToString()const {
    stringstream ss;
    ss << "{";

    if (_size == 0) {
        ss << " }";
        return ss.str();
    } else if (_size > 1){

        for (size_t i = 0; i < _size-1; ++i) {
            ss << (_data[i]) << ", ";
        }
        ss << _data[_size-1] << "}";
        return ss.str();
    } else {
        ss << _data[0] << "}";
        return ss.str();
    }
}

/**
 * Clear
 * Clears by deleting each index of the array, then setting the size to 0.
 * @param none
 * @return none
 */
void VSArray::Clear() {
    for (size_t i = 0; i < _size; i++) {
        delete _data[i];
    }
    _size = 0;
}

/**
 * Get Capacity
 * Returns the capacity of the array, which is the largest amount the array can hold
 * @param none
 * @return the capacity of the array as a size_t
 */
size_t VSArray::GetCapacity()const {
    return _capacity;
}