#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "android/os/Looper.h"
#include "android/os/Handler.h"
#include "android/os/Thread.h"
#include "android/os/Message.h"
#include "android/os/Closure.h"
#include "android/os/AsyncTask.h"

using namespace android::os;

class ExampleAsyncTask : public AsyncTask<int32_t, int32_t, int32_t>
{
public:
	virtual void onPreExecute() {
		printf("ExampleAsyncTask::onPreExecute on thread %d\n", (int32_t) pthread_self());
	}

	virtual int32_t doInBackground(int32_t param) {
		printf("ExampleAsyncTask::doInBackground on thread %d with param %d\n", (int32_t) pthread_self(), param);
		Thread::sleep(250);
		int32_t sum = 0;
		int32_t i;
		for (i = 0; i < param / 2; i++) {
			sum++;
		}
		publishProgress(sum);
		Thread::sleep(250);
		for (; i < param; i++) {
			sum++;
		}
		return sum;
	}

	virtual void onProgressUpdate(int32_t value) {
		printf("ExampleAsyncTask::onProgressUpdate on thread %d with value %d\n", (int32_t) pthread_self(), value);
	}

	virtual void onPostExecute(int32_t result) {
		printf("ExampleAsyncTask::onPostExecute on thread %d with result %d\n", (int32_t) pthread_self(), result);
	}

	virtual void onCancelled() {
		printf("ExampleAsyncTask::onCancelled on thread %d\n", (int32_t) pthread_self());
	}
};

class AsyncTaskSupervisor
{
public:
	AsyncTaskSupervisor() :
		mHandler(NULL) {
	}

	~AsyncTaskSupervisor() {
		delete mHandler;
	}

	void onLooperReady() {
		mHandler = new Handler();

		// AsyncTasks must be executed by a Looper thread because of the callback message handling
		ExampleAsyncTask* asyncTask1 = new ExampleAsyncTask();
		asyncTask1->execute(1234567);

		ExampleAsyncTask* asyncTask2 = new ExampleAsyncTask();
		asyncTask2->executeOnExecutor(AsyncTaskBase::THREAD_POOL_EXECUTOR, 123);

		mHandler->postDelayed(newRunnable(*this, &AsyncTaskSupervisor::shutdownLooper), 2000);
	}

	void shutdownLooper() {
		Looper::myLooper()->quit();
	}

private:
	Handler* mHandler;
};

int main() {
	AsyncTaskSupervisor asyncTaskSupervisor;

	Looper::prepare(newRunnable(asyncTaskSupervisor, &AsyncTaskSupervisor::onLooperReady));
	Looper::loop();

	return 0;
}
