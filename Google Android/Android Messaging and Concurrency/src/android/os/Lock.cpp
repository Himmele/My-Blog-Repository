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

#include <stdlib.h>
#include "android/os/Lock.h"
#include "android/os/Clock.h"

namespace android {
namespace os {

Lock::Lock() {
	pthread_mutex_init(&mMutex, NULL);
}

Lock::~Lock() {
	pthread_mutex_destroy(&mMutex);
}

bool Lock::lock() {
	return (pthread_mutex_lock(&mMutex) == 0);
}

#ifndef __ANDROID__
bool Lock::lock(uint32_t timeout) {
	timespec tsAbsTimeout;
	clock_gettime(CLOCK_REALTIME, &tsAbsTimeout);
	uint64_t absTimeout = ((uint64_t)tsAbsTimeout.tv_nsec) + (uint64_t)(timeout * 1000000LL);
	tsAbsTimeout.tv_sec += absTimeout / 1000000000LL;
	tsAbsTimeout.tv_nsec = absTimeout % 1000000000LL;
	return (pthread_mutex_timedlock(&mMutex, &tsAbsTimeout) == 0);
}
#endif

void Lock::unlock() {
	pthread_mutex_unlock(&mMutex);
}

} /* namespace os */
} /* namespace android */
