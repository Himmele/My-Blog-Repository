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
#include "android/os/Handler.h"
#include "android/os/Message.h"
#include "android/os/MessageQueue.h"
#include "android/os/Looper.h"
#include "android/os/Clock.h"

namespace android {
namespace os {

Handler::Handler() {
	Looper* looper = Looper::myLooper();
	assert(looper != NULL);
	mMessageQueue = &looper->myMessageQueue();
}

Handler::Handler(Looper& looper) {
	mMessageQueue = &looper.myMessageQueue();
}

Handler::~Handler() {
}

void Handler::dispatchMessage(Message& message) {
	if (message.mCallback != NULL) {
		handleCallback(message);
	} else {
		handleMessage(message);
	}
}

void Handler::handleMessage(Message& message) {
}

bool Handler::sendMessage(Message& message) {
	return sendMessageAtTime(message, Clock::monotonicTime());
}

bool Handler::sendMessageDelayed(Message& message, uint32_t delay) {
	return sendMessageAtTime(message, Clock::monotonicTime() + delay * 1000000LL);
}

bool Handler::sendMessageAtTime(Message& message, uint64_t execTimestamp) {
	message.mHandler = this;
	return mMessageQueue->enqueueMessage(message, execTimestamp);
}

bool Handler::post(Runnable* runnable) {
	if (runnable != NULL) {
		Message& message = getPostMessage(*runnable);
		return sendMessage(message);
	} else {
		return false;
	}
}

bool Handler::postDelayed(Runnable* runnable, uint32_t delay) {
	if (runnable != NULL) {
		Message& message = getPostMessage(*runnable);
		return sendMessageDelayed(message, delay);
	} else {
		return false;
	}
}

bool Handler::postAtTime(Runnable* runnable, uint64_t execTimestamp) {
	if (runnable != NULL) {
		Message& message = getPostMessage(*runnable);
		return sendMessageAtTime(message, execTimestamp);
	} else {
		return false;
	}
}

Message& Handler::getPostMessage(Runnable& runnable) {
	Message& message = Message::obtain();
	message.mCallback = &runnable;
	return message;
}

bool Handler::removeMessages(int32_t what) {
	return mMessageQueue->removeMessages(this, what);
}

bool Handler::removeCallbacks(Runnable* runnable) {
	return mMessageQueue->removeCallbacks(this, runnable);
}

bool Handler::removeCallbacksAndMessages() {
	return mMessageQueue->removeCallbacksAndMessages(this);
}

} /* namespace os */
} /* namespace android */
