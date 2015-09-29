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

#ifndef ANDROID_OS_LOOPERTHREAD_H_
#define ANDROID_OS_LOOPERTHREAD_H_

#include "android/os/Utils.h"
#include "android/os/Thread.h"
#include "android/os/Looper.h"
#include "android/os/Handler.h"
#include "android/os/Lock.h"
#include "android/os/CondVar.h"

namespace android {
namespace os {

/*
 * Handy class for starting a new Thread that has a Looper and a Handler.
 * See LooperThreadExample.cpp for details.
 * T must be a subclass of class Handler.
 */
template<class T /*extends Handler*/>
class LooperThread :
	public Thread
{
public:
	LooperThread() :
		mLooper(NULL),
		mHandler(NULL),
		mCondVar(mLock),
		mIsDone(false) {
	}

	virtual ~LooperThread() {
	}

	virtual void run() {
		Looper::prepare();
		mLock.lock();
		mLooper = Looper::myLooper();
		mHandler = new T();
		mCondVar.notifyAll();
		mLock.unlock();
		Looper::loop();
		mLock.lock();
		mIsDone = true;
		mHandler->removeCallbacksAndMessages();
		delete mHandler;
		mLooper = NULL;
		mHandler = NULL;
		mLock.unlock();
	}

	Looper* getLooper() {
		AutoLock autoLock(mLock);
		if (!mIsDone && mLooper == NULL) {
			mCondVar.wait();
		}
		return mLooper;
	}

	T* getHandler() {
		AutoLock autoLock(mLock);
		if (!mIsDone && mHandler == NULL) {
			mCondVar.wait();
		}
		return mHandler;
	}

private:
	Looper* mLooper;
	T* mHandler;
	Lock mLock;
	CondVar mCondVar;
	bool mIsDone;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(LooperThread)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_LOOPERTHREAD_H_ */
