#include "IBinderTestService.h"
#include <binder/IServiceManager.h>
#include "android/os/Thread.h"
#include "android/os/Closure.h"

using namespace android;

void callTest1(sp<IBinderTestService> binderTestService) {
	LOGI("Test1 is starting...");
	int32_t res = binderTestService->test1();
	LOGI("Test1 result: %d", res);
}

void callTest2(sp<IBinderTestService> binderTestService, int32_t value) {
	LOGI("Test2 starting...");
	int32_t res = binderTestService->test2(value);
	LOGI("Test2 result: %d", res);
}

int main()
{
	const String16 name("BinderTestService");
	sp<IBinderTestService> mBinderTestService;
	if (getService(name, &mBinderTestService) != NO_ERROR) {
		LOGE("BinderTestService not available");
	}

	android::os::Thread thread1(android::os::newRunnable(&callTest1, mBinderTestService));
	thread1.start();

	android::os::Thread thread2(android::os::newRunnable(&callTest2, mBinderTestService, 123));
	thread2.start();

	android::os::Thread thread3(android::os::newRunnable(&callTest1, mBinderTestService));
	thread3.start();

	android::os::Thread thread4(android::os::newRunnable(&callTest2, mBinderTestService, 1234567));
	thread4.start();

	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();

	sp<IMemoryHeap> ashmem = mBinderTestService->test3(String8("Hello Reference-Counted Android Shared Memory"));
	LOGI("Android Shared Memory Contents: %s\n", (char*)ashmem->base());


//	Call stack for exchanging the IMemoryHeap Binder IPC channel
//
//	Client side:
//		BpBinderTestService.test3
//
//	Service side:
//	BinderTestService.test3 -> returns the MemoryHeapBase service object
//		Parcel.writeStrongBinder -> flatten_binder with type BINDER_TYPE_BINDER
//			-> Binder Linux Kernel module (function binder_transaction) changes the type from BINDER_TYPE_BINDER to BINDER_TYPE_HANDLE
//
//	Client side:
//	Parcel.readStrongBinder -> unflatten_binder with type BINDER_TYPE_HANDLE -> creates the BpMemoryHeap proxy object which mmaps the shared memory
//		BpBinderTestService.test3
//
//	Client side:
//	BpMemoryHeap.assertReallyMapped
//		BpBinder.transact(HEAP_ID)
//
//	Service side:
//	BnMemoryHeap.onTransact -> writes shared memory file descriptor, size and flags into the outgoing parcel -> BINDER_TYPE_FD
//
//	Client side:
//	BpMemoryHeap.assertReallyMapped
//		Parcel.readFileDescriptor
//			mmap Android shared memory

	return 0;
}
