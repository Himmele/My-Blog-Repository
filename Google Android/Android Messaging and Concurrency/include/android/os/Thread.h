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

#ifndef ANDROID_OS_THREAD_H_
#define ANDROID_OS_THREAD_H_

#include <pthread.h>
#include "android/os/Utils.h"
#include "android/os/Runnable.h"

namespace android {
namespace os {

class Thread :
	public Runnable
{
public:
	Thread();
	Thread(Runnable* runnable);
	virtual ~Thread() {}
	virtual void run() {}
	bool start();
	static void sleep(uint32_t milliseconds);
	void interrupt();
	bool isInterrupted() const;
	void join() const;
	void setSchedulingParams(int32_t policy, int32_t priority);

private:
	static void* exec(void* args);

	Runnable* mRunnable;
	pthread_t mThread;
	bool mInterrupted;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Thread)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_THREAD_H_ */
