#include "IBinderTestService.h"
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <string.h>

namespace android {

// Binder Test Service Proxy
class BpBinderTestService :
	public BpInterface<IBinderTestService> // -> public IBinderTestService, public BpRefBase
{
public:
    BpBinderTestService(const sp<IBinder>& impl) :
    	BpInterface<IBinderTestService>(impl) {
    }

    virtual int32_t test1() {
		Parcel data, reply;
		data.writeInterfaceToken(IBinderTestService::getInterfaceDescriptor());
		remote()->transact(BnBinderTestService::TEST1, data, &reply);
		return reply.readInt32();
    }

    virtual int32_t test2(int32_t value) {
    	Parcel data, reply;
		data.writeInterfaceToken(IBinderTestService::getInterfaceDescriptor());
		data.writeInt32(value);
		remote()->transact(BnBinderTestService::TEST2, data, &reply);
		return reply.readInt32();
    }

    virtual sp<IMemoryHeap> test3(const String8& message) {
    	Parcel data, reply;
		data.writeInterfaceToken(IBinderTestService::getInterfaceDescriptor());
		data.writeString8(message);
		remote()->transact(BnBinderTestService::TEST3, data, &reply);
		return interface_cast<IMemoryHeap>(reply.readStrongBinder());
    }
};

IMPLEMENT_META_INTERFACE(BinderTestService, "android.tests.BinderTestService");

// Binder Test Service
BnBinderTestService::BnBinderTestService() {
}

status_t BnBinderTestService::onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
	switch (code) {
		case TEST1: {
			CHECK_INTERFACE(IBinderTestService, data, reply);
			status_t res = test1();
			reply->writeInt32(res);
		} break;
		case TEST2: {
			CHECK_INTERFACE(IBinderTestService, data, reply);
			int32_t value = data.readInt32();
			status_t res = test2(value);
			reply->writeInt32(res);
		} break;
		case TEST3: {
			CHECK_INTERFACE(IBinderTestService, data, reply);
			String8 message = data.readString8();
			sp<IBinder> b = test3(message)->asBinder();
			reply->writeStrongBinder(b);
		} break;
		default:
		    return BBinder::onTransact(code, data, reply, flags);
	}
	return NO_ERROR;
}

} /* namespace android */
