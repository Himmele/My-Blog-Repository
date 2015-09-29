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

#ifndef ANDROID_OS_LOCK_H_
#define ANDROID_OS_LOCK_H_

#include <stdint.h>
#include <pthread.h>
#include "android/os/Utils.h"

namespace android {
namespace os {

class Lock
{
public:
	Lock();
	~Lock();
	bool lock();
#ifndef __ANDROID__
	bool lock(uint32_t timeout);
#endif
	void unlock();

private:
	pthread_mutex_t mMutex;

	friend class CondVar;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Lock)
};

class AutoLock
{
public:
	AutoLock(Lock& lock) :
		mLock(lock) {
		mLock.lock();
	}

	~AutoLock() {
		mLock.unlock();
	}

private:
	Lock& mLock;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(AutoLock)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_LOCK_H_ */
