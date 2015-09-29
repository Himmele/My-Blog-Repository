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

#ifndef ANDROID_OS_ASYNCTASK_H_
#define ANDROID_OS_ASYNCTASK_H_

#include "android/os/Utils.h"
#include "android/os/Handler.h"
#include "android/os/SerialExecutor.h"
#include "android/os/ThreadPoolExecutor.h"
#include "android/os/Lock.h"

namespace android {
namespace os {

class AsyncTaskBase
{
public:
	static SerialExecutor SERIAL_EXECUTOR;
	static ThreadPoolExecutor THREAD_POOL_EXECUTOR;

protected:
	static Lock sLock;

private:
	static const uint32_t THREAD_POOL_SIZE = 4;
};


template<typename Params, typename Progress, typename Result>
class AsyncTask :
	public AsyncTaskBase,
	public Ref
{
public:
	AsyncTask() :
		mExecutor(NULL),
		mCancelled(false) {
		mHandler = new InternalHandler();
		mWorkerRunnable = new WorkerRunnable(*this);
	}

	virtual ~AsyncTask() {
	}

	sp< AsyncTask<Params, Progress, Result> > execute(Params params) {
		if (mExecutor == NULL) {
			mExecutor = &SERIAL_EXECUTOR;
			onPreExecute();
			mWorkerRunnable->mParams = params;
			mExecutor->execute(mWorkerRunnable);
			return this;
		} else {
			return NULL;
		}
	}

	sp< AsyncTask<Params, Progress, Result> > executeOnExecutor(Executor& executor, Params params) {
		if (mExecutor == NULL) {
			mExecutor = &executor;
			onPreExecute();
			mWorkerRunnable->mParams = params;
			mExecutor->execute(mWorkerRunnable);
			return this;
		} else {
			return NULL;
		}
	}

	bool cancel() {
		bool isAlreadyCancelled = isCancelled();
		AutoLock autoLock(sLock);
		if (mExecutor != NULL && !isAlreadyCancelled) {
			bool result = mExecutor->cancel(mWorkerRunnable);
			if (result) {
				mCancelled = true;
				Message& message = mHandler->obtainMessage(ON_TASK_CANCELLED_MESSAGE);
				message.obj = mWorkerRunnable.getPointer();
				message.sendToTarget();
			}
			return result;
		} else {
			return false;
		}
	}

	bool isCancelled() {
		AutoLock autoLock(sLock);
		return mCancelled;
	}

protected:
	// runs within a thread pool's thread context.
	virtual Result doInBackground(Params params) = 0;

	// runs within the Handler's thread context.
	virtual void onPreExecute() {}
	virtual void onProgressUpdate(Progress values) {}
	virtual void onPostExecute(Result result) {}
	virtual void onCancelled() {}

	void publishProgress(Progress values) {
		sp<Message> message = mHandler->obtainMessage(ON_PROGRESS_UPDATE_MESSAGE);
		message->obj = new AsyncTaskResult(*this, values);
		message->sendToTarget();
	}

	Params params() {
		return mWorkerRunnable->mParams;
	}

	Result result() {
		return mWorkerRunnable->mResult;
	}

private:
	class InternalHandler : public Handler
	{
	public:
		virtual void handleMessage(const sp<Message>& message) {
			switch (message->what) {
			case ON_POST_EXECUTE_MESSAGE: {
				WorkerRunnable* runnable = (WorkerRunnable*) message->obj;
				runnable->mTask.onPostExecute(runnable->mResult);
				break;
			}
			case ON_PROGRESS_UPDATE_MESSAGE: {
				AsyncTaskResult* result = (AsyncTaskResult*) message->obj;
				result->mTask.onProgressUpdate(result->mData);
				delete result;
				break;
			}
			case ON_TASK_CANCELLED_MESSAGE: {
				WorkerRunnable* runnable = (WorkerRunnable*) message->obj;
				runnable->mTask.onCancelled();
				break;
			}
			}
		}
	};

	class WorkerRunnable : public Runnable
	{
	public:
		WorkerRunnable(AsyncTask<Params, Progress, Result>& task) :
			mTask(task) {
		}

		virtual void run() {
			mResult = mTask.doInBackground(mParams);
			sp<Message> message = mTask.mHandler->obtainMessage(ON_POST_EXECUTE_MESSAGE);
			message->obj = this;
			message->sendToTarget();
		}

		Params mParams;
		Result mResult;

	private:
		AsyncTask<Params, Progress, Result>& mTask;

		friend class InternalHandler;
	};

	class AsyncTaskResult
	{
	public:
		AsyncTaskResult(AsyncTask<Params, Progress, Result>& task, Progress& data) :
			mTask(task),
			mData(data) {
		}

	private:
		AsyncTask<Params, Progress, Result>& mTask;
		Progress mData;

		friend class InternalHandler;
	};

	Executor* mExecutor;
	sp<InternalHandler> mHandler;
	sp<WorkerRunnable> mWorkerRunnable;
	bool mCancelled;

	static const uint32_t ON_POST_EXECUTE_MESSAGE = 1;
	static const uint32_t ON_PROGRESS_UPDATE_MESSAGE = 2;
	static const uint32_t ON_TASK_CANCELLED_MESSAGE = 3;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(AsyncTask)
};

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_ASYNCTASK_H_ */
