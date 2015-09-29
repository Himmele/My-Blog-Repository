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

#ifndef ANDROID_OS_LOOPER_H_
#define ANDROID_OS_LOOPER_H_

#include <pthread.h>
#include "android/os/Utils.h"
#include "android/os/MessageQueue.h"

namespace android {
namespace os {

class Runnable;

class Looper
{
public:
	virtual ~Looper();
	static bool prepare();
	static bool prepare(const sp<Runnable>& onLooperReadyRunnable);
	static Looper* myLooper();
	static void loop();
	void quit();
	sp<MessageQueue> myMessageQueue() {
		return mMessageQueue;
	}

private:
	Looper();
	static void init();
	static void finalize(void* looper);

	sp<MessageQueue> mMessageQueue;
	sp<Runnable> mOnLooperReadyRunnable;
	static pthread_once_t sTlsOneTimeInitializer;
	static pthread_key_t sTlsKey;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Looper)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_LOOPER_H_ */
