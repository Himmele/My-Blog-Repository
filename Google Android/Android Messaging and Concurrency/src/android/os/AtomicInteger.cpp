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

#include "android/os/AtomicInteger.h"
#include "android/os/Lock.h"

namespace android {
namespace os {

#ifdef __X86_CPU_ARCH__

int32_t AtomicInteger::addAndGet(int32_t value, volatile int32_t* ptr) {
	__asm__ __volatile__ ("lock; xaddl %0, %1"
						  : "+r" (value), "+m" (*ptr)
						  :
						  : "memory");
	/* value now holds the old value of *ptr */
	return value;
}

int32_t AtomicInteger::incrementAndGet(volatile int32_t* address) {
	return addAndGet(1, address);
}

int32_t AtomicInteger::decrementAndGet(volatile int32_t* address) {
	return addAndGet(-1, address);
}

int32_t AtomicInteger::andAndGet(int32_t value, volatile int32_t* ptr) {
	int32_t prevValue, status;
	do {
		prevValue = *ptr;
		status = compareAndSwap(prevValue, prevValue & value, ptr);
	} while (__builtin_expect(status != 0, 0));
	return prevValue;
}

int32_t AtomicInteger::orAndGet(int32_t value, volatile int32_t* ptr) {
	int32_t prevValue, status;
	do {
		prevValue = *ptr;
		status = compareAndSwap(prevValue, prevValue | value, ptr);
	} while (__builtin_expect(status != 0, 0));
	return prevValue;
}

int32_t AtomicInteger::compareAndSwap(int32_t oldValue, int32_t newValue, volatile int32_t* ptr) {
	int32_t prevValue;
	__asm__ __volatile__ ("lock; cmpxchgl %1, %2"
						  : "=a" (prevValue)
						  : "q" (newValue), "m" (*ptr), "0" (oldValue)
						  : "memory");
	return prevValue != oldValue;
}

#elif defined __ARM_CPU_ARCH__

#ifdef __ARMv6_CPU_ARCH__
#define dmb() __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" : : "r" (0) : "memory"); // ARMv6 memory barrier
#else
#define dmb() __asm__ __volatile__ ("dmb" : : : "memory"); // ARMv7 memory barrier: DMB - Data Memory Barrier
#endif

int32_t AtomicInteger::addAndGet(int32_t value, volatile int32_t* ptr) {
	int32_t prevValue, tmpValue, status;
	dmb();
	do {
		__asm__ __volatile__ ("ldrex %0, [%4]\n"
							  "add %1, %0, %5\n"
							  "strex %2, %1, [%4]"
							  : "=&r" (prevValue), "=&r" (tmpValue),
								"=&r" (status), "+m" (*ptr)
							  : "r" (ptr), "Ir" (value)
							  : "cc");
	} while (__builtin_expect(status != 0, 0));
	return prevValue;
}

int32_t AtomicInteger::incrementAndGet(volatile int32_t* address) {
	return addAndGet(1, address);
}

int32_t AtomicInteger::decrementAndGet(volatile int32_t* address) {
	return addAndGet(-1, address);
}

int32_t AtomicInteger::andAndGet(int32_t value, volatile int32_t* ptr) {
	int32_t prevValue, tmpValue, status;
	dmb();
	do {
		__asm__ __volatile__ ("ldrex %0, [%4]\n"
							  "and %1, %0, %5\n"
							  "strex %2, %1, [%4]"
							  : "=&r" (prevValue), "=&r" (tmpValue),
								"=&r" (status), "+m" (*ptr)
							  : "r" (ptr), "Ir" (value)
							  : "cc");
	} while (__builtin_expect(status != 0, 0));
	return prevValue;
}

int32_t AtomicInteger::orAndGet(int32_t value, volatile int32_t* ptr) {
	int32_t prevValue, tmpValue, status;
	dmb();
	do {
		__asm__ __volatile__ ("ldrex %0, [%4]\n"
							  "orr %1, %0, %5\n"
							  "strex %2, %1, [%4]"
							  : "=&r" (prevValue), "=&r" (tmpValue),
								"=&r" (status), "+m" (*ptr)
							  : "r" (ptr), "Ir" (value)
							  : "cc");
	} while (__builtin_expect(status != 0, 0));
	return prevValue;
}

int32_t AtomicInteger::compareAndSwap(int32_t oldValue, int32_t newValue, volatile int32_t* ptr) {
	int32_t prevValue, status;
	dmb();
	do {
		__asm__ __volatile__ ("ldrex %0, [%3]\n"
							  "mov %1, #0\n"
							  "teq %0, %4\n"
							  "strexeq %1, %5, [%3]"
							  : "=&r" (prevValue), "=&r" (status), "+m"(*ptr)
							  : "r" (ptr), "Ir" (oldValue), "r" (newValue)
							  : "cc");
	} while (__builtin_expect(status != 0, 0));
	return prevValue != oldValue;
}

#else

#warning "AtomicInteger.cpp -> Unknown CPU architecture!"

static Lock sLock;

int32_t AtomicInteger::addAndGet(int32_t value, volatile int32_t* ptr) {
	AutoLock autoLock(sLock);
	int32_t oldValue = *ptr;
	*ptr += value;
	return oldValue;
}

int32_t AtomicInteger::incrementAndGet(volatile int32_t* address) {
	return addAndGet(1, address);
}

int32_t AtomicInteger::decrementAndGet(volatile int32_t* address) {
	return addAndGet(-1, address);
}

int32_t AtomicInteger::andAndGet(int32_t value, volatile int32_t* ptr) {
	AutoLock autoLock(sLock);
	int32_t prevValue = *ptr;
	*ptr &= value;
	return prevValue;
}

int32_t AtomicInteger::orAndGet(int32_t value, volatile int32_t* ptr) {
	AutoLock autoLock(sLock);
	int32_t prevValue = *ptr;
	*ptr |= value;
	return prevValue;
}

int32_t AtomicInteger::compareAndSwap(int32_t oldValue, int32_t newValue, volatile int32_t* ptr) {
	AutoLock autoLock(sLock);
	int32_t prevValue = *ptr;
	if (*ptr == oldValue) {
		*ptr = newValue;
	}
	return prevValue != oldValue;
}

#endif

} /* namespace os */
} /* namespace android */
