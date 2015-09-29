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
#include <unistd.h>
#include <string.h>
#include "android/os/Thread.h"

namespace android {
namespace os {

Thread::Thread() :
	mRunnable(this),
	mInterrupted(false) {
}

Thread::Thread(Runnable* runnable) :
	mRunnable(runnable),
	mInterrupted(false) {
}

bool Thread::start() {
	int32_t errorCode = -1;
	if (mRunnable != NULL) {
		errorCode = pthread_create(&mThread, NULL, &Thread::exec, this);
	}
	return (errorCode == 0);
}

void Thread::sleep(uint32_t milliseconds) {
	::usleep((milliseconds % 1000) * 1000);
	::sleep(milliseconds / 1000);
}

void Thread::join() const {
	pthread_join(mThread, NULL);
}

void* Thread::exec(void* args) {
	Thread* const self = (Thread*) args;
	self->mRunnable->run();
	return NULL;
}

void Thread::interrupt() {
	mInterrupted = true;
}

bool Thread::isInterrupted() const {
	return mInterrupted;
}

void Thread::setSchedulingParams(int32_t policy, int32_t priority) {
	sched_param schedulingParameters;
	memset(&schedulingParameters, 0, sizeof(schedulingParameters));
	schedulingParameters.sched_priority = priority;
	pthread_setschedparam(mThread, policy, &schedulingParameters);
}

} /* namespace os */
} /* namespace android */
