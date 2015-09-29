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

Message* Message::sMessagePool = NULL;
uint32_t Message::sMessagePoolSize = 0;
Lock Message::sMessagePoolLock;

Message::Message() :
	what(0),
	arg1(0),
	arg2(0),
	mExecTimestamp(0),
	mHandler(NULL),
	mCallback(NULL),
	mNextMessage(NULL) {
}

Message& Message::obtain() {
	AutoLock autoLock(Message::sMessagePoolLock);
	if (sMessagePool != NULL) {
		Message* message = sMessagePool;
		sMessagePool = message->mNextMessage;
		message->mNextMessage = NULL;
		sMessagePoolSize--;
		return *message;
	}
	Message* message = new Message();
	assert(message != NULL);
	return *message;
}

Message& Message::obtain(Handler& handler) {
	Message& message = obtain();
	message.mHandler = &handler;
	return message;
}

Message& Message::obtain(Handler& handler, int32_t what) {
	Message& message = obtain();
	message.mHandler = &handler;
	message.what = what;
	return message;
}

Message& Message::obtain(Handler& handler, int32_t what, int32_t arg1, int32_t arg2) {
	Message& message = obtain();
	message.mHandler = &handler;
	message.what = what;
	message.arg1 = arg1;
	message.arg2 = arg2;
	return message;
}

Message& Message::obtain(Handler& handler, Runnable& callback) {
	Message& message = obtain();
	message.mHandler = &handler;
	message.mCallback = &callback;
	return message;
}

void Message::recycle() {
	AutoLock autoLock(Message::sMessagePoolLock);
	if (sMessagePoolSize < MAX_MESSAGE_POOL_SIZE) {
		clear();
		mNextMessage = sMessagePool;
		sMessagePool = this;
		sMessagePoolSize++;
	} else {
		delete this;
	}
}

void Message::clear() {
	what = 0;
	arg1 = 0;
	arg2 = 0;
	mExecTimestamp = 0;
	mHandler = NULL;
	mCallback = NULL;
}

uint64_t Message::getExecTimestamp() const {
	return mExecTimestamp;
}

void Message::setHandler(Handler& handler) {
	mHandler = &handler;
}

Handler* Message::getHandler() {
	return mHandler;
}

Runnable* Message::getCallback() {
	return mCallback;
}

bool Message::sendToTarget() {
	if (mHandler != NULL) {
		return mHandler->sendMessage(*this);
	} else {
		return false;
	}
}

} /* namespace os */
} /* namespace android */
