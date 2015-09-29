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

#include "android/os/Looper.h"
#include "android/os/Handler.h"
#include "android/os/Message.h"

namespace android {
namespace os {

pthread_once_t Looper::sTlsOneTimeInitializer = PTHREAD_ONCE_INIT;
pthread_key_t Looper::sTlsKey;

Looper::Looper() :
	mOnLooperReadyRunnable(NULL) {
	mMessageQueue = new MessageQueue();
}

Looper::~Looper() {
}

void Looper::init()
{
	pthread_key_create(&sTlsKey, Looper::finalize);
}

void Looper::finalize(void* looper)
{
    delete (Looper*) looper;
}

bool Looper::prepare() {
	pthread_once(&sTlsOneTimeInitializer, Looper::init);
	Looper* looper = (Looper*) pthread_getspecific(sTlsKey);
	if (looper == NULL) {
		looper = new Looper();
		if (looper == NULL) {
			return false;
		} else {
			if (pthread_setspecific(sTlsKey, looper) != 0) {
				delete looper;
				return false;
			} else {
				return true;
			}
		}
	} else {
		// There should be only one Looper per thread.
		return false;
	}
}

bool Looper::prepare(const sp<Runnable>& onLooperReadyRunnable) {
	if (prepare()) {
		Looper* me = myLooper();
		if (me != NULL) {
			me->mOnLooperReadyRunnable = onLooperReadyRunnable;
		}
		return true;
	}
	return false;
}

Looper* Looper::myLooper() {
	pthread_once(&sTlsOneTimeInitializer, Looper::init);
	Looper* looper = (Looper*) pthread_getspecific(sTlsKey);
	return looper;
}

void Looper::loop() {
	Looper* me = myLooper();
	if (me != NULL) {
		sp<MessageQueue> mq = me->mMessageQueue;
		if (me->mOnLooperReadyRunnable != NULL) {
			me->mOnLooperReadyRunnable->run();
			me->mOnLooperReadyRunnable = NULL;
		}
		while (true) {
			sp<Message> message = mq->dequeueMessage();
			if (message->mHandler == NULL) {
				return;
			}
			message->mHandler->dispatchMessage(message);
		}
	}
}

void Looper::quit() {
	sp<Message> message = Message::obtain();
	mMessageQueue->enqueueMessage(message, 0);
}

} /* namespace os */
} /* namespace android */
