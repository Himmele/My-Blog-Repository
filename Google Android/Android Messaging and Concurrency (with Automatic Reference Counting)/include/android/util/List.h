/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Some refactorings by Daniel Himmelein
 */

// STL-like list class

#ifndef ANDROID_UTIL_LIST_H_
#define ANDROID_UTIL_LIST_H_

#include <stddef.h>
#include <stdint.h>
#include "android/os/Ref.h"

namespace android {
namespace util {

template<typename T>
class List :
	public android::os::Ref
{
protected:
    class Node
    {
    public:
        explicit Node(const T& value) : mValue(value) {}
        ~Node() {}
        inline T& getRefValue() { return mValue; }
        inline const T& getRefValue() const { return mValue; }
        inline Node* getPrevNode() const { return mPrevNode; }
        inline Node* getNextNode() const { return mNextNode; }
        inline void setValue(const T& value) { mValue = value; }
        inline void setPrevNode(Node* node) { mPrevNode = node; }
        inline void setNextNode(Node* node) { mNextNode = node; }

    private:
        friend class List;
        friend class ListIterator;
        T mValue;
        Node* mPrevNode;
        Node* mNextNode;
    };

    template <typename TYPE>
    struct CONST_ITERATOR {
        typedef Node const* NodePtr;
        typedef const TYPE Type;
    };

    template <typename TYPE>
    struct ITERATOR {
        typedef Node* NodePtr;
        typedef TYPE Type;
    };

    template<typename U, template <class> class Constness>
    class ListIterator
    {
        typedef ListIterator<U, Constness> Iterator;
        typedef typename Constness<U>::NodePtr NodePtr;
        typedef typename Constness<U>::Type Type;

        explicit ListIterator(NodePtr node) : mNode(node) {}

    public:
        ListIterator() {}
        ListIterator(const Iterator& rhs) : mNode(rhs.mNode) {}
        ~ListIterator() {}

        // Handle conversions from iterator to const_iterator.
        template<typename V> explicit
        ListIterator(const V& rhs) : mNode(rhs.mpNode) {}

        Type& operator*() const { return mNode->getRefValue(); }
        Type* operator->() const { return &(mNode->getRefValue()); }

        inline bool operator==(const Iterator& rhs) const {
            return mNode == rhs.mNode;
        }

        inline bool operator!=(const Iterator& rhs) const {
            return mNode != rhs.mNode;
        }

        // Handle comparisons between iterator and const_iterator.
        template<typename OTHER>
        inline bool operator==(const OTHER& rhs) const {
            return mNode == rhs.mNode;
        }

        template<typename OTHER>
        inline bool operator!=(const OTHER& rhs) const {
            return mNode != rhs.mNode;
        }

        inline Iterator& operator++() { // ++itr
            mNode = mNode->getNextNode();
            return *this;
        }

        const Iterator operator++(int) { // itr++
            Iterator tmp(*this);
            mNode = mNode->getNextNode();
            return tmp;
        }

        inline Iterator& operator--() { // --itr
            mNode = mNode->getPrevNode();
            return *this;
        }

        const Iterator operator--(int) { // itr--
        	Iterator tmp(*this);
            mNode = mNode->getPrevNode();
            return tmp;
        }

        inline NodePtr getNode() const { return mNode; }

        NodePtr mNode;
    private:
        friend class List;
    };

public:
    List() {
    	init();
    }

    virtual ~List() {
        clear();
        delete[] (uint8_t*) mMiddleNode;
    }

    typedef ListIterator<T, ITERATOR> iterator;
    typedef ListIterator<T, CONST_ITERATOR> const_iterator;

    inline bool empty() const { return mMiddleNode->getNextNode() == mMiddleNode; }

    size_t size() const {
        return size_t(distance(begin(), end()));
    }

    // Return the first element for begin or one past the last element for end.
    inline iterator begin() {
        return iterator(mMiddleNode->getNextNode());
    }

    inline const_iterator begin() const {
        return const_iterator(const_cast<Node const*>(mMiddleNode->getNextNode()));
    }

    inline iterator end() {
        return iterator(mMiddleNode);
    }

    inline const_iterator end() const {
        return const_iterator(const_cast<Node const*>(mMiddleNode));
    }

    void push_front(const T& value) { insert(begin(), value); }
    void push_back(const T& value) { insert(end(), value); }

    // Insert an item before the current node and return an iterator to the new node.
    iterator insert(iterator itr, const T& value) {
        Node* newNode = new Node(value);
        newNode->setNextNode(itr.getNode());
        newNode->setPrevNode(itr.getNode()->getPrevNode());
        itr.getNode()->getPrevNode()->setNextNode(newNode);
        itr.getNode()->setPrevNode(newNode);
        return iterator(newNode);
    }

    // Insert a range of elements before the current node.
    void insert(iterator curItr, iterator firstItr, iterator lastItr) {
		for ( ; firstItr != lastItr; ++firstItr) {
			insert(curItr, *firstItr);
		}
	}

    void insert(iterator curItr, const_iterator firstItr, const_iterator lastItr) {
        for ( ; firstItr != lastItr; ++firstItr) {
            insert(curItr, *firstItr);
        }
    }

    // Remove one entry from the list and return an iterator to the next node.
    iterator erase(iterator itr) {
        Node* nextNode = itr.getNode()->getNextNode();
        Node* prevNode = itr.getNode()->getPrevNode();
        prevNode->setNextNode(nextNode);
        nextNode->setPrevNode(prevNode);
        delete itr.getNode();
        return iterator(nextNode);
    }

    // Remove a range of items.
    iterator erase(iterator firstItr, iterator lastItr) {
        while (firstItr != lastItr) {
            erase(firstItr++);
        }
        return iterator(lastItr);
    }

    // Remove all items from the list.
    void clear() {
        Node* curNode = mMiddleNode->getNextNode();
        Node* nextNode;

        while (curNode != mMiddleNode) {
        	nextNode = curNode->getNextNode();
            delete curNode;
            curNode = nextNode;
        }
        mMiddleNode->setPrevNode(mMiddleNode);
        mMiddleNode->setNextNode(mMiddleNode);
    }

    /*
     * Measure the distance between two iterators.
     * The iterators must refer to the same list.
     */
    template<
        typename U,
        template <class> class CL,
        template <class> class CR
    >
    ptrdiff_t distance(ListIterator<U, CL> firstItr, ListIterator<U, CR> lastItr) const {
        ptrdiff_t count = 0;
        while (firstItr != lastItr) {
            ++firstItr;
            ++count;
        }
        return count;
    }

private:
    /*
     * The mMiddleNode does not need to hold valid data.
     * Furthermore T's constructor should not be called for mMiddleNode
     * since this may have side-effects or it may require arguments.
     */
    void init() {
        mMiddleNode = (Node*) new uint8_t[sizeof(Node)];
        mMiddleNode->setPrevNode(mMiddleNode);
        mMiddleNode->setNextNode(mMiddleNode);
    }

    /*
     * This node plays the role of "pointer to head node" and "pointer to tail node".
     * It sits in the middle of a circular list of nodes.
     * The iterator runs around the circle until it encounters this one.
     */
    Node* mMiddleNode;

    NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(List<T>)
};

} /* namespace util */
} /* namespace android */

#endif /*ANDROID_UTIL_LIST_H_*/
