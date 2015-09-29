/*
 * Copyright (C) 2011 Daniel Himmelein
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
 */

#ifndef ANDROID_OS_LINKEDBLOCKINGQUEUE_H_
#define ANDROID_OS_LINKEDBLOCKINGQUEUE_H_

#include <stdint.h>
#include "android/os/Utils.h"
#include "android/os/Lock.h"
#include "android/os/CondVar.h"

namespace android {
namespace os {

template<typename T>
class LinkedBlockingQueue
{
public:
	LinkedBlockingQueue() :
		mHeadNode(NULL),
		mCondVar(mCondVarLock) {
	}

	~LinkedBlockingQueue() {
	}

	bool put(T item) {
		AutoLock autoLock(mCondVarLock);
		Node* node = new Node(item);
		Node* curNode = mHeadNode;
		if (curNode == NULL) {
			node->nextNode = curNode;
			mHeadNode = node;
			mCondVar.notify();
		} else {
			Node* prevNode = NULL;
			while (curNode != NULL) {
				prevNode = curNode;
				curNode = curNode->nextNode;
			}
			node->nextNode = prevNode->nextNode;
			prevNode->nextNode = node;
			mCondVar.notify();
		}
		return true;
	}

	T take() {
		while (true) {
			AutoLock autoLock(mCondVarLock);
			Node* node = getNextNode();
			if (node != NULL) {
				T item = node->item;
				delete node;
				return item;
			} else {
				mCondVar.wait();
			}
		}
	}

	bool remove(T item) {
		bool foundItem = false;
		AutoLock autoLock(mCondVarLock);
		Node* curNode = mHeadNode;
		// remove all matching messages at the front of the message queue.
		while (curNode != NULL && curNode->item == item) {
			foundItem = true;
			Node* nextNode = curNode->nextNode;
			mHeadNode = nextNode;
			delete curNode;
			curNode = nextNode;
		}

		// remove all matching messages after the front of the message queue.
		while (curNode != NULL) {
			Node* nextNode = curNode->nextNode;
			if (nextNode != NULL) {
				if (nextNode->item == item) {
					foundItem = true;
					Node* nextButOneNode = nextNode->nextNode;
					delete nextNode;
					curNode->nextNode = nextButOneNode;
					continue;
				}
			}
			curNode = nextNode;
		}
		return foundItem;
	}

private:
	struct Node
	{
		T item;

		Node* nextNode;

		Node(T t) { item = t; }
	};

	Node* getNextNode() {
		Node* node = mHeadNode;
		if (node != NULL) {
			mHeadNode = node->nextNode;
			return node;
		}
		return NULL;
	}

	Node* mHeadNode;
	Lock mCondVarLock;
	CondVar mCondVar;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(LinkedBlockingQueue)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_LINKEDBLOCKINGQUEUE_H_ */
