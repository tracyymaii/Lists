/**
 * Title:		Lab 02 - sorted_double_linked_list.cpp
 * Purpose:		Implements the insert method of a double linked list that
 *              keeps its elements sorted at all times.
 * Author:		Tracy Mai
 * Date:		April 12, 2023
 */

#include "sorted_double_linked_list.h"

using std::stoi;


// Implement ALL methods of class SortedDoubleLinkedList

/**
 * Sorted Double Linked List Constructor
 * Sets the head and tail to nullptr
 * @param none
 * @return none
 */
SortedDoubleLinkedList::SortedDoubleLinkedList(){
    _head = nullptr;
    _tail = nullptr;
}

/**
 * Insert
 * Traverses through the sorted double linked list and if the element in the
 * linked list is larger than the element, then the position of the larger element
 * is saved. Then the insertion for the new element will happen in this position
 * by calling on double linked list's insertion function.
 * @param the element to be inserted
 * @return true, the insertion should always be successful
 */
bool SortedDoubleLinkedList::Insert(Comparable* element) {

    size_t insertPosition = 0;
    if (_size == 0) {
        DoubleLinkedList::Insert(element, insertPosition);
        return true;
    }

    Node *temp = _head;

    for (insertPosition = 0; insertPosition < _size; ++insertPosition) {
        if (temp == nullptr) {
            break;
        }

        if (stoi(temp->data->ToString()) < stoi(element->ToString())) {
            temp = temp->next;
        } else {
            break;
        }
    }

    DoubleLinkedList::Insert(element, insertPosition);
    return true;
}
