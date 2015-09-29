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

#include <stddef.h>
#include <assert.h>
#include "android/os/Message.h"
#include "android/os/Handler.h"
#include "android/os/Lock.h"

namespace android {
namespace os {

MessagePool Message::sMessagePool;

MessagePool::MessagePool() :
	MAX_SIZE(10) {
}

MessagePool::~MessagePool() {
	AutoLock autoLock(mLock);
	// Avoid memory leaks if new message objects are allocated after the MessagePool is shut down.
	MAX_SIZE = 0;
	sp<Message> curMessage = mHeadMessage;
	while (curMessage != NULL) {
		curMessage.getPointer()->setDestroyer(NULL);
		curMessage = curMessage->mNextMessage;
	}
	mHeadMessage = NULL;
}


Message::Message() :
	what(0),
	arg1(0),
	arg2(0),
	obj(NULL),
	mExecTimestamp(0),
	mHandler(NULL),
	mCallback(NULL),
	mData(NULL),
	mNextMessage(NULL) {
}

Message::~Message() {
}

sp<Message> Message::obtain() {
	{
		AutoLock autoLock(sMessagePool.mLock);
		if (sMessagePool.mHeadMessage != NULL) {
			sp<Message> message = sMessagePool.mHeadMessage;
			sMessagePool.mHeadMessage = message->mNextMessage;
			message->mNextMessage = NULL;
			sMessagePool.mSize--;
			return message;
		}
	}
	Message* message = new Message();
	assert(message != NULL);
	message->setDestroyer(message);
	return message;
}

sp<Message> Message::obtain(const sp<Handler>& handler) {
	sp<Message> message = obtain();
	message->mHandler = handler;
	return message;
}

sp<Message> Message::obtain(const sp<Handler>& handler, int32_t what) {
	sp<Message> message = obtain();
	message->mHandler = handler;
	message->what = what;
	return message;
}

sp<Message> Message::obtain(const sp<Handler>& handler, int32_t what, int32_t arg1, int32_t arg2) {
	sp<Message> message = obtain();
	message->mHandler = handler;
	message->what = what;
	message->arg1 = arg1;
	message->arg2 = arg2;
	return message;
}

sp<Message> Message::obtain(const sp<Handler>& handler, const sp<Runnable>& callback) {
	sp<Message> message = obtain();
	message->mHandler = handler;
	message->mCallback = callback;
	return message;
}

sp<Message> Message::obtain(const Message* message) {
	sp<Message> dupMessage = obtain();
	dupMessage->what = message->what;
	dupMessage->arg1 = message->arg1;
	dupMessage->arg2 = message->arg2;
	dupMessage->obj = message->obj;
	dupMessage->mExecTimestamp = 0;
	dupMessage->mHandler = message->mHandler;
	dupMessage->mCallback = message->mCallback;
	dupMessage->mData = message->mData;
	dupMessage->mNextMessage = NULL;
	return dupMessage;
}

uint64_t Message::getExecTimestamp() const {
	return mExecTimestamp;
}

void Message::setHandler(const sp<Handler>& handler) {
	mHandler = handler;
}

sp<Handler> Message::getHandler() const {
	return mHandler;
}

sp<Runnable> Message::getCallback() const {
	return mCallback;
}

void Message::setData(sp<Bundle> data) {
	mData = data;
}

sp<Bundle> Message::getData() const {
	return mData;
}

bool Message::sendToTarget() {
	if (mHandler != NULL) {
		return mHandler->sendMessage(this);
	} else {
		return false;
	}
}

sp<Message> Message::dup() const {
	return Message::obtain(this);
}

void Message::destroy(Ref* ref) {
	{
		// The reentranceMessageGuard keeps the mNextMessage until we release the sMessagePoolLock since sMessagePoolLock is not a recursive mutex.
		sp<Message> reentranceMessageGuard;
		AutoLock autoLock(sMessagePool.mLock);
		if (sMessagePool.mSize < sMessagePool.MAX_SIZE) {
			// Make sure that the ref message object is alive before it may appear as rvalue (e.g. as sMessagePool or mNextMessage).
			sp<Message> message = reviveObject<Message>(ref);
			reentranceMessageGuard = mNextMessage;
			clear();
			mNextMessage = sMessagePool.mHeadMessage;
			sMessagePool.mHeadMessage = message;
			sMessagePool.mSize++;
			return;
		}
	}
	setDestroyer(NULL);
	delete this;
}

void Message::clear() {
	what = 0;
	arg1 = 0;
	arg2 = 0;
	obj = NULL;
	mExecTimestamp = 0;
	mHandler = NULL;
	mCallback = NULL;
	mData = NULL;
	mNextMessage = NULL;
}

} /* namespace os */
} /* namespace android */
