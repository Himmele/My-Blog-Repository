LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	src/IBinderTestService.cpp \
	src/BinderTestService.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
	liblog

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_MODULE:= BinderTestService

include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	src/IBinderTestService.cpp \
	src/BinderTestClient.cpp \
	src/android/os/Thread.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
	liblog

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include

LOCAL_MODULE:= BinderTestClient

include $(BUILD_EXECUTABLE)
