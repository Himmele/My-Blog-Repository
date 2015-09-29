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

#include "android/os/Semaphore.h"
#include "android/os/Clock.h"

namespace android {
namespace os {

Semaphore::Semaphore(uint32_t value) {
	sem_init(&mSemaphore, 0, value);
}

Semaphore::~Semaphore() {
	sem_destroy(&mSemaphore);
}

void Semaphore::signal() {
	sem_post(&mSemaphore);
}

bool Semaphore::wait() {
	return (sem_wait(&mSemaphore) == 0);
}

bool Semaphore::wait(uint32_t timeout) {
	timespec tsAbsTimeout;
	clock_gettime(CLOCK_REALTIME, &tsAbsTimeout);
	uint64_t absTimeout = ((uint64_t)tsAbsTimeout.tv_nsec) + (uint64_t)(timeout * 1000000LL);
	tsAbsTimeout.tv_sec += absTimeout / 1000000000LL;
	tsAbsTimeout.tv_nsec = absTimeout % 1000000000LL;
	return (sem_timedwait(&mSemaphore, &tsAbsTimeout) == 0);
}

} /* namespace os */
} /* namespace android */
