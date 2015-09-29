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

#include "android/os/Clock.h"

namespace android {
namespace os {

uint64_t Clock::monotonicTime() {
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return (((uint64_t)now.tv_sec * 1000000000LL) + now.tv_nsec);
}

uint64_t Clock::realTime() {
	timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return (((uint64_t)now.tv_sec * 1000000000LL) + now.tv_nsec);
}

} /* namespace os */
} /* namespace android */
