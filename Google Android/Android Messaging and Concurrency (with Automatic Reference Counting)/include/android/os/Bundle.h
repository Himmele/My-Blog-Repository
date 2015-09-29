/*
 * Copyright (C) 2012 Daniel Himmelein
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

#ifndef ANDROID_OS_BUNDLE_H_
#define ANDROID_OS_BUNDLE_H_

#include <stdint.h>
#include "android/os/Utils.h"
#include "android/os/Ref.h"
#include "android/util/List.h"
#include "android/lang/String.h"

namespace android {
namespace os {

class Bundle :
	public Ref
{
public:
	Bundle();
	virtual ~Bundle();

	size_t size() const { return mKeyValuePairs->size(); }
	bool empty() const { return mKeyValuePairs->empty(); }
	void clear();
	bool containsKey(const char* key);
	void remove(const char* key);

	void putBool(const char* key, bool value);
	void putByte(const char* key, uint8_t value);
	void putChar(const char* key, char value);
	void putInt16(const char* key, int16_t value);
	void putUInt16(const char* key, uint16_t value);
	void putInt32(const char* key, int32_t value);
	void putUInt32(const char* key, uint32_t value);
	void putInt64(const char* key, int64_t value);
	void putUInt64(const char* key, uint64_t value);
	void putFloat(const char* key, float value);
	void putDouble(const char* key, double value);
	void putString(const char* key, const char* string);
	void putString(const char* key, const android::lang::String& string);
	void putObject(const char* key, const sp<Ref>& object);

	bool getBool(const char* key, const bool defaultValue) const;
	uint8_t getByte(const char* key, const uint8_t defaultValue) const;
	char getChar(const char* key, const char defaultValue) const;
	int16_t getInt16(const char* key, const int16_t defaultValue) const;
	uint16_t getUInt16(const char* key, const uint16_t defaultValue) const;
	int32_t getInt32(const char* key, const int32_t defaultValue) const;
	uint32_t getUInt32(const char* key, const uint32_t defaultValue) const;
	int64_t getInt64(const char* key, const int64_t defaultValue) const;
	uint64_t getUInt64(const char* key, const uint64_t defaultValue) const;
	float getFloat(const char* key, const float defaultValue) const;
	double getDouble(const char* key, const double defaultValue) const;
	android::lang::String getString(const char* key) const;
	template<typename T>
	sp<T> getObject(const char* key) const;

	bool fillBool(const char* key, bool* value) const;
	bool fillByte(const char* key, uint8_t* value) const;
	bool fillChar(const char* key, char* value) const;
	bool fillInt16(const char* key, int16_t* value) const;
	bool fillUInt16(const char* key, uint16_t* value) const;
	bool fillInt32(const char* key, int32_t* value) const;
	bool fillUInt32(const char* key, uint32_t* value) const;
	bool fillInt64(const char* key, int64_t* value) const;
	bool fillUInt64(const char* key, uint64_t* value) const;
	bool fillFloat(const char* key, float* value) const;
	bool fillDouble(const char* key, double* value) const;
	bool fillString(const char* key, android::lang::String* string) const;
	template<typename T>
	bool fillObject(const char* key, sp<T>* object) const;

private:
	class Variant :
		public Ref
	{
	public:
		enum Type {
			Null,
			Bool,
			Byte,
			Char,
			Int16,
			UInt16,
			Int32,
			UInt32,
			Int64,
			UInt64,
			Float,
			Double,
			String,
			Object
		};

		inline Variant() : mType(Null) { }

		inline Variant(bool value) : mType(Bool) {
			mVariant.boolValue = value;
		}

		inline Variant(uint8_t value) : mType(Byte) {
			mVariant.byteValue = value;
		}

		inline Variant(char value) : mType(Char) {
			mVariant.charValue = value;
		}

		inline Variant(int16_t value) : mType(Int16) {
			mVariant.int16Value = value;
		}

		inline Variant(uint16_t value) : mType(UInt16) {
			mVariant.uint16Value = value;
		}

		inline Variant(int32_t value) : mType(Int32) {
			mVariant.int32Value = value;
		}

		inline Variant(uint32_t value) : mType(UInt32) {
			mVariant.uint32Value = value;
		}

		inline Variant(int64_t value) : mType(Int64) {
			mVariant.int64Value = value;
		}

		inline Variant(uint64_t value) : mType(UInt64) {
			mVariant.uint64Value = value;
		}

		inline Variant(float value) : mType(Float) {
			mVariant.floatValue = value;
		}

		inline Variant(double value) : mType(Double) {
			mVariant.doubleValue = value;
		}

		inline Variant(const char* string) : mType(String) {
			mVariant.string = new android::lang::String(string);
		}

		inline Variant(const android::lang::String& string) : mType(String) {
			mVariant.string = new android::lang::String(string.c_str());
		}

		inline Variant(const sp<Ref>& object) : mType(Object) {
			if (object != NULL) {
				object->incStrongRef(this);
			}
			mVariant.object = object.getPointer();
		}

		inline ~Variant() {
			switch (mType) {
			case Variant::String:
				delete mVariant.string;
				break;
			case Variant::Object:
				if (mVariant.object != NULL) {
					mVariant.object->decStrongRef(this);
				}
				break;
			default:
				break;
			}
		}

		inline Type getType() const {
			return mType;
		}

		inline bool getBool() const {
			return mVariant.boolValue;
		}

		inline uint8_t getByte() const {
			return mVariant.byteValue;
		}

		inline char getChar() const {
			return mVariant.charValue;
		}

		inline int16_t getInt16() const {
			return mVariant.int16Value;
		}

		inline uint16_t getUInt16() const {
			return mVariant.uint16Value;
		}

		inline int32_t getInt32() const {
			return mVariant.int32Value;
		}

		inline uint32_t getUInt32() const {
			return mVariant.uint32Value;
		}

		inline int64_t getInt64() const {
			return mVariant.int64Value;
		}

		inline uint64_t getUInt64() const {
			return mVariant.uint64Value;
		}

		inline float getFloat() const {
			return mVariant.floatValue;
		}

		inline double getDouble() const {
			return mVariant.doubleValue;
		}

		inline android::lang::String getString() const {
			return *mVariant.string;
		}

		template<typename T>
		inline sp<T> getObject() const {
			return static_cast<T*>(mVariant.object);
		}

	private:
		typedef union {
			bool boolValue;
			uint8_t byteValue;
			char charValue;
			int16_t int16Value;
			uint16_t uint16Value;
			int32_t int32Value;
			uint32_t uint32Value;
			int64_t int64Value;
			uint64_t uint64Value;
			float floatValue;
			double doubleValue;
			android::lang::String* string;
			Ref* object;
		} Value;

		Type mType;
		Value mVariant;

		NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Variant)
	};

	struct KeyValuePair {
		android::lang::String key;
		sp<Variant> value;
	};

	android::util::List<KeyValuePair>::const_iterator findValue(const char* key) const;
	android::os::sp< android::util::List<KeyValuePair> > mKeyValuePairs;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Bundle)
};

template<typename T>
sp<T> Bundle::getObject(const char* key) const {
	android::util::List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Object) {
			return itr->value->getObject<T>();
		} else {
			return NULL;
		}
	}
	return NULL;
}

template<typename T>
bool Bundle::fillObject(const char* key, sp<T>* object) const {
	android::util::List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Object) {
			*object = itr->value->getObject<T>();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_BUNDLE_H_ */
