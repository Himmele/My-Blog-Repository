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

#ifndef ANDROID_OS_ATOMICINTEGER_H_
#define ANDROID_OS_ATOMICINTEGER_H_

#include <stdint.h>
#include "android/os/Utils.h"

namespace android {
namespace os {

class AtomicInteger
{
public:
	static int32_t addAndGet(int32_t value, volatile int32_t* ptr);
	static int32_t incrementAndGet(volatile int32_t* address);
	static int32_t decrementAndGet(volatile int32_t* address);
	static int32_t andAndGet(int32_t value, volatile int32_t* ptr);
	static int32_t orAndGet(int32_t value, volatile int32_t* ptr);
	static int32_t compareAndSwap(int32_t oldValue, int32_t newValue, volatile int32_t* ptr);

private:
	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(AtomicInteger)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_ATOMICINTEGER_H_ */
