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

#include "android/os/Bundle.h"

using namespace android::util;
using namespace android::lang;

namespace android {
namespace os {

Bundle::Bundle() {
	mKeyValuePairs = new List<KeyValuePair>();
}

Bundle::~Bundle() {
	clear();
}

void Bundle::clear() {
	mKeyValuePairs.clear();
}

bool Bundle::containsKey(const char* key) {
	List<KeyValuePair>::iterator itr = mKeyValuePairs->begin();
	while (itr != mKeyValuePairs->end()) {
		if (itr->key == key) {
			return true;
		}
		++itr;
	}
	return false;
}

void Bundle::remove(const char* key) {
	List<KeyValuePair>::iterator itr = mKeyValuePairs->begin();
	while (itr != mKeyValuePairs->end()) {
		if (itr->key == key) {
			itr = mKeyValuePairs->erase(itr);
			break;
		}
		++itr;
	}
}

void Bundle::putBool(const char* key, bool value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putByte(const char* key, uint8_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putChar(const char* key, char value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putInt16(const char* key, int16_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putUInt16(const char* key, uint16_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putInt32(const char* key, int32_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putUInt32(const char* key, uint32_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putInt64(const char* key, int64_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putUInt64(const char* key, uint64_t value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putFloat(const char* key, float value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putDouble(const char* key, double value) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(value);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putString(const char* key, const char* string) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(string);
	mKeyValuePairs->push_back(keyValuePair);
}

void Bundle::putString(const char* key, const android::lang::String& string) {
	putString(key, string.c_str());
}

void Bundle::putObject(const char* key, const sp<Ref>& object) {
	KeyValuePair keyValuePair;
	keyValuePair.key = key;
	keyValuePair.value = new Variant(object);
	mKeyValuePairs->push_back(keyValuePair);
}

bool Bundle::getBool(const char* key, const bool defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Bool) {
			return itr->value->getBool();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

uint8_t Bundle::getByte(const char* key, const uint8_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Byte) {
			return itr->value->getByte();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

char Bundle::getChar(const char* key, const char defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Char) {
			return itr->value->getChar();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

int16_t Bundle::getInt16(const char* key, const int16_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Int16) {
			return itr->value->getInt16();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

uint16_t Bundle::getUInt16(const char* key, const uint16_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::UInt16) {
			return itr->value->getUInt16();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

int32_t Bundle::getInt32(const char* key, const int32_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Int32) {
			return itr->value->getInt32();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

uint32_t Bundle::getUInt32(const char* key, const uint32_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::UInt32) {
			return itr->value->getUInt32();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

int64_t Bundle::getInt64(const char* key, const int64_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Int64) {
			return itr->value->getInt64();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

uint64_t Bundle::getUInt64(const char* key, const uint64_t defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::UInt64) {
			return itr->value->getUInt64();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

float Bundle::getFloat(const char* key, const float defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Float) {
			return itr->value->getFloat();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

double Bundle::getDouble(const char* key, const double defaultValue) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Double) {
			return itr->value->getDouble();
		} else {
			return defaultValue;
		}
	}
	return defaultValue;
}

String Bundle::getString(const char* key) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::String) {
			return itr->value->getString();
		} else {
			return String(NULL);
		}
	}
	return String(NULL);
}

bool Bundle::fillBool(const char* key, bool* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Bool) {
			*value = itr->value->getBool();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillByte(const char* key, uint8_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Byte) {
			*value = itr->value->getByte();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillChar(const char* key, char* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Char) {
			*value = itr->value->getChar();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillInt16(const char* key, int16_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Int16) {
			*value = itr->value->getInt16();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillUInt16(const char* key, uint16_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::UInt16) {
			*value = itr->value->getUInt16();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillInt32(const char* key, int32_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Int32) {
			*value = itr->value->getInt32();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillUInt32(const char* key, uint32_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::UInt32) {
			*value = itr->value->getUInt32();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillInt64(const char* key, int64_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Int64) {
			*value = itr->value->getInt64();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillUInt64(const char* key, uint64_t* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::UInt64) {
			*value = itr->value->getUInt64();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillFloat(const char* key, float* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Float) {
			*value = itr->value->getFloat();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillDouble(const char* key, double* value) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::Double) {
			*value = itr->value->getFloat();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool Bundle::fillString(const char* key, android::lang::String* string) const {
	List<KeyValuePair>::const_iterator itr = findValue(key);
	if (itr != mKeyValuePairs->end()) {
		if (itr->value->getType() == Variant::String) {
			*string = itr->value->getString();
			return true;
		} else {
			return false;
		}
	}
	return false;
}

List<Bundle::KeyValuePair>::const_iterator Bundle::findValue(const char* key) const {
	List<KeyValuePair>::const_iterator itr = mKeyValuePairs.getConstPointer()->begin();
	while (itr != mKeyValuePairs->end()) {
		if (itr->key == key) {
			return itr;
		}
		++itr;
	}
	return mKeyValuePairs.getConstPointer()->end();
}

} /* namespace os */
} /* namespace android */
