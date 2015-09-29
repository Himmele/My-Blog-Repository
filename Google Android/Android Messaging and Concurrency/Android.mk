LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	src/android/os/Thread.cpp \
	src/android/os/Semaphore.cpp \
	src/android/os/MessageQueue.cpp \
	src/android/os/Message.cpp \
	src/android/os/Looper.cpp \
	src/android/os/Lock.cpp \
	src/android/os/CondVar.cpp \
	src/android/os/Handler.cpp \
	src/android/os/Clock.cpp \
	src/android/os/AsyncTask.cpp \
	src/android/os/SerialExecutor.cpp \
	src/android/os/ThreadPoolExecutor.cpp \
	src/android/os/AtomicInteger.cpp.arm \
	src/android/os/Ref.cpp

LOCAL_CFLAGS := -D__ANDROID__ -D__ARM_CPU_ARCH__

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE:= libAndroidOS

include $(BUILD_SHARED_LIBRARY)

# ----

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	Test.cpp

LOCAL_CFLAGS := -D__ANDROID__ -D__ARM_CPU_ARCH__

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	libAndroidOS
	
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_MODULE:= AndroidMessagingAndConcurrencyTest

include $(BUILD_EXECUTABLE)

# ----

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	LooperThreadExample.cpp

LOCAL_CFLAGS := -D__ANDROID__ -D__ARM_CPU_ARCH__

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	libAndroidOS
	
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_MODULE:= LooperThreadExample

include $(BUILD_EXECUTABLE)

# ----

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	AsyncTaskExample.cpp

LOCAL_CFLAGS := -D__ANDROID__ -D__ARM_CPU_ARCH__

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	libAndroidOS
	
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_MODULE:= AsyncTaskExample

include $(BUILD_EXECUTABLE)

# ----

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	DelegateExample.cpp

LOCAL_CFLAGS := -D__ANDROID__ -D__ARM_CPU_ARCH__

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	libstlport \
	libAndroidOS
	
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

include external/stlport/libstlport.mk

LOCAL_MODULE:= DelegateExample

include $(BUILD_EXECUTABLE)

# ----

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	RefCountingTest.cpp

LOCAL_CFLAGS := -D__ANDROID__ -D__ARM_CPU_ARCH__

LOCAL_SHARED_LIBRARIES := \
	libutils \
	libcutils \
	libstlport \
	libAndroidOS
	
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_MODULE:= RefCountingTest

include $(BUILD_EXECUTABLE)
