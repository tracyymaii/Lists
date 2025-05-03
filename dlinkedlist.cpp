/**
 * Title:		Lab 02 - dlinkedlist.cpp
 * Purpose:		Implements all the methods of a double linked list.
 * Author:		Tracy Mai
 * Date:		April 12, 2023
 */

#include "object.h"
#include "dlinkedlist.h"

#include <string>
#include <iostream>
#include <sstream>

using std::stringstream;
using std::string;


// Implement ALL methods of DoubleLinkedList class

/**
 * Double Linked List Constructor
 * Makes an empty double linked list by setting the head and tail to nullptr
 * @param: none
 * @return: none
 */
DoubleLinkedList::DoubleLinkedList(): _head(nullptr), _tail(nullptr) {
}

/**
 * Double Linked List Copy Constructor
 * Makes a deep copy of the other linked list by traversing the other linked
 * list and cloning and inserting it into our list. By using the Clone function
 * from the Object class, we can ensure that we are making a deep copy instead of a shallow copy
 * @param: other linked list
 * @return: nothing
 */
DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other) : _head(nullptr), _tail(nullptr) {
    for (Node *temp = other._head; temp != nullptr; temp = temp->next) {
        Insert(temp->data->Clone(), _size);
    }
}

/**
 * Double Linked List Copy Assignment Operator
 * Enables the deep copy assignment with this operator
 * @param rhs linked list
 * @return *this to make copies
 */
DoubleLinkedList& DoubleLinkedList::operator=(const DoubleLinkedList& rhs) {

    if (&rhs == this) {
        return *this;
    }

    Clear();
    delete[] _head;
    _size = rhs._size;

    for (Node *temp = rhs._head; temp != nullptr; temp = temp->next) {
        Insert(temp->data->Clone(), _size);
    }

    return *this;
}

/**
 * Double Linked List Destructor
 * Uses the Remove function to always be deallocating and deleting the head
 * until the size of the linked list is 0. Explicitly makes a pointer to whatever the
 * current head is, in order to effectively delete and not have memory leaks.
 * @param none
 * @return none
 */
DoubleLinkedList:: ~DoubleLinkedList() {
    while (_size != 0) {
        Object* temp = Remove(0);
        delete temp;
    }
}

/**
 * Insert
 * Creates a new node and puts in the given element into its data. Then inserts
 * this new node into its requested position. Increases the size as needed
 * Things to consider: inserting when empty, at the head, in the middle, and at the end.
 * More detailed comments are in the code
 * @param the element and the position it should inserted into
 * @return a bool, depending on whether or not the insertion is successful
 */
bool DoubleLinkedList::Insert(Object* element, size_t position) {
    if (position > _size) {
        return false;
    }

    Node* added = new Node;
    added->data = element;
    added->next = nullptr;
    added->previous = nullptr;

    /**
     * To insert in an empty list
     */
    if (_size == 0) {
        _head = added;

    /**
     * To insert at the head in a non-empty list
     */
    } else if (position == 0) {
        added->next = _head;
        _head = added;

    /**
     * Inserts in the middle or at the end
     */
    } else {

        Node* temp1 = _head;
        for (size_t i = 0; i < position-1; ++i) {
            temp1 = temp1->next;
        }

        added->next = temp1->next;
        temp1->next = added;

        /**
         * Different switching of pointers depending on if the insertion
         * is in the middle or at the end
         */
        if (added->next == nullptr) {
            _tail = added;
            _tail->previous = temp1;
        }

        if (position < _size - 1) {
            added->previous = temp1;
            (added->next)->previous = added;
        }
    }

    ++_size;
    return true;
}

/**
 * Index Of
 * Traverses through the linked list to find the needed element
 * Cases to think about: if the element does not exist, or in an empty list,
 * both will return -1
 * @param element searching for
 * @return the index of the element if it exists in the linked list,
 * and -1 if it doesn't
 */
int DoubleLinkedList::IndexOf(const Object* element)const {

    if (_size == 0 ) {
        return -1;
    }

    Node* temp = _head;

    for (size_t i = 0; i < _size; ++i) {
        if ((temp->data)->Equals(*element)) {
            int index = i;
            return index;
        } else {
            temp = temp->next;
        }
    }

    return -1;
}

/**
 * Remove
 * Removes and deallocates the element at the given position, and returns the element removed
 * Decreases the size as needed
 * Cases to think about: If the linked list does not exist, or if the position
 * is greater than size, both of will return nullptr
 * @param the position where the element will be removed
 * @return the object* that was removed, or nullptr if position could not be found
 */
Object* DoubleLinkedList::Remove(size_t position) {

    if (_size == 0 || position > _size - 1) {
        return nullptr;
    }

    Node* temp;
    Object* retValue;

    /**
     * Removes the only element in the linked list
     */
    if (_size == 1) {
        retValue = _head->data;
        _head->data = nullptr;
        delete _head;
        --_size;
        return retValue;

    /**
     * Removes the element at position 0
     */
    } else if (position == 0) {
        retValue = _head->data;
        temp = _head->next;

        _head->data = nullptr;

        _head->next = nullptr;

        delete _head;

        _head = temp;
        --_size;
        return retValue;
    }

    temp = _head;

    /**
     * If removing in the middle or at the end
     */
    if (position > 0) {

        for (size_t i = 0; i < position - 1; ++i) {
            temp = temp->next;
        }
        Node *removePos = temp->next;
        retValue = temp->next->data;

        removePos->data = nullptr;
        removePos->previous = nullptr;
        temp->next = nullptr;

        /**
         * Check for more rearranging of pointers, if removing from the middle
         */
        if (position != _size -1) {
            temp->next = removePos->next;
            removePos->next = nullptr;
            (temp->next)->previous = temp;
        }

        delete removePos;
    }
    --_size;
    return retValue;
}

/**
 * Get
 * Retrieves the object at a certain position if the position exists
 * Cases to think about: if the position is greater than size or if the list
 * does not exist
 * @param position that we will get the object from
 * @return the object if the position exists
 */
Object* DoubleLinkedList::Get(size_t position)const {
    if (position > _size || _size == 0) {
        return nullptr;
    }

    Node* temp = _head;
    for (size_t i = 0; i <= position; ++i) {
        if (i == position) {
            return temp->data;
        }
        temp = temp->next;
    }

    return nullptr;
}

/**
 * To String
 * Prints out the linked list in a string format
 * @param none
 * @return the linked list as a string format
 */
string DoubleLinkedList::ToString()const {

    stringstream ss;
    ss << "{";

    if (_size == 0) {
        ss << " }";
        return ss.str();
    } else if (_size > 1){

        for (Node* temp = _head; temp->next != nullptr; temp = temp->next) {
            ss << (temp->data) << ", ";
        }
        ss << _tail << "}";
        return ss.str();
    } else {
        ss << _head << "}";
        return ss.str();
    }
}

/**
 * Clear
 * Calls the Remove function to clear the linked list as long as the size is not 0.
 * @param none
 * @return none
 */
void DoubleLinkedList::Clear() {
    while (_size != 0) {
        Object* temp = Remove(0);
        delete temp;
    }
    _head = nullptr;
    _tail = nullptr;

}