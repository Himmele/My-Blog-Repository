#ifndef IBINDERTESTSERVICE_H_
#define IBINDERTESTSERVICE_H_

#include <utils/RefBase.h>
#include <utils/String8.h>
#include <binder/IInterface.h>
#include <binder/MemoryHeapBase.h>

namespace android {

// Binder Test Service Interface
class IBinderTestService :
	public IInterface
{
public:
    DECLARE_META_INTERFACE(BinderTestService);

    virtual int32_t test1() = 0;
    virtual int32_t test2(int32_t value) = 0;
    virtual sp<IMemoryHeap> test3(const String8& message) = 0;
};

// Binder Test Service
class BnBinderTestService :
	public BnInterface<IBinderTestService> // -> public IBinderTestService, public BBinder
{
public:
	enum {
		TEST1 = IBinder::FIRST_CALL_TRANSACTION,
		TEST2,
		TEST3,
	};

	BnBinderTestService();

	virtual status_t onTransact(uint32_t code,
		const Parcel& data,
		Parcel* reply,
		uint32_t flags = 0);
};

} /* namespace android */

#endif /* IBINDERTESTSERVICE_H_ */
