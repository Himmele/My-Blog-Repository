/*
 * Copyright (C) 2005 The Android Open Source Project
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
 *
 * Some refactorings by Daniel Himmelein
 */

#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <sys/types.h>
#include <cstring>
#include "android/os/Ref.h"
#include "android/os/Lock.h"

// Log all reference counting operations
#define PRINT_REFS 0
// Debugging operations
#define DEBUG_REFS 0
#define DEBUG_REFS_DO_REF_TRACKING 0
#define DEBUG_REFS_MEMORIZE_REF_OPERATIONS_DURING_REF_TRACKING 0

namespace android {
namespace os {

#define INITIAL_STRONG_REF_VALUE (1<<28)

Ref::Destroyer::~Destroyer() {
}

class Ref::WeakRefImpl : public Ref::WeakRef
{
public:
	// The reference-counted object will be deleted if mStrongRefCounter reaches 0 (if no lifetime flags are set)
    volatile int32_t mStrongRefCounter;
    // The WeakRefImpl object will be deleted if mWeakRefCounter reaches 0 (if no lifetime flags are set)
    volatile int32_t mWeakRefCounter;
    Ref* const mRef;
    volatile int32_t mFlags;
    Destroyer* mDestroyer;

#if !DEBUG_REFS
    WeakRefImpl(Ref* ref) :
    	mStrongRefCounter(INITIAL_STRONG_REF_VALUE),
        mWeakRefCounter(0),
        mRef(ref),
        mFlags(0),
        mDestroyer(NULL) {
    }

    void addStrongRef(const void* id) { }
    void removeStrongRef(const void* id) { }
    void addWeakRef(const void* id) { }
    void removeWeakRef(const void* id) { }
    void printRefs() const { }
    void trackRef(bool, bool) { }

#else
    WeakRefImpl(Ref* ref) :
    	mStrongRefCounter(INITIAL_STRONG_REF_VALUE),
        mWeakRefCounter(0),
        mRef(ref),
        mFlags(0),
        mDestroyer(NULL),
        mStrongRefs(NULL),
        mWeakRefs(NULL),
        mDoRefTracking(!!DEBUG_REFS_DO_REF_TRACKING),
        mMemorizeRefOperationsDuringRefTracking(!!DEBUG_REFS_MEMORIZE_REF_OPERATIONS_DURING_REF_TRACKING) {
    }
    
    ~WeakRefImpl() {
    	if (!mMemorizeRefOperationsDuringRefTracking) {
			ASSERT(mStrongRefs == NULL, "Strong references were not freed during reference tracking!");
			ASSERT(mWeakRefs == NULL, "Weak references were not freed during reference tracking!");
    	}
    }

    void addStrongRef(const void* id) {
        addRef(&mStrongRefs, id, mStrongRefCounter);
    }

    void removeStrongRef(const void* id) {
        if (!mMemorizeRefOperationsDuringRefTracking) {
            removeRef(&mStrongRefs, id);
        } else {
            addRef(&mStrongRefs, id, -mStrongRefCounter);
        }
    }

    void addWeakRef(const void* id) {
        addRef(&mWeakRefs, id, mWeakRefCounter);
    }

    void removeWeakRef(const void* id) {
        if (!mMemorizeRefOperationsDuringRefTracking) {
            removeRef(&mWeakRefs, id);
        } else {
            addRef(&mWeakRefs, id, -mWeakRefCounter);
        }
    }

    void trackRef(bool doTracking, bool memorizeRefOperationsDuringRefTracking) {
        mDoRefTracking = doTracking;
        mMemorizeRefOperationsDuringRefTracking = memorizeRefOperationsDuringRefTracking;
    }

    void printRefs() const {
        char* buffer = (char*) malloc(1);
        buffer[0] = '\0';

        {
            AutoLock autoLock(const_cast<WeakRefImpl*>(this)->mLock);

            char tmpBuffer[128];
            sprintf(tmpBuffer, "Strong references on Ref %p (WeakRef %p):\n", mRef, this);
            buffer = (char*) realloc(buffer, strlen(buffer) + strlen(tmpBuffer) + 1);
            strcat(buffer, tmpBuffer);
            buffer = printRefInfos(buffer, mStrongRefs);
            sprintf(tmpBuffer, "Weak references on Ref %p (WeakRef %p):\n", mRef, this);
            buffer = (char*) realloc(buffer, strlen(buffer) + strlen(tmpBuffer) + 1);
            strcat(buffer, tmpBuffer);
            buffer = printRefInfos(buffer, mWeakRefs);
        }

        DEBUG_INFO("%s", buffer);

        free(buffer);
    }

private:
    struct RefInfo
    {
        RefInfo* nextRef;
        const void* id;
        int32_t refCounter;
    };

    void addRef(RefInfo** refs, const void* id, int32_t refCounter)
    {
        if (mDoRefTracking) {
            AutoLock autoLock(mLock);
            RefInfo* ref = new RefInfo;
            ref->refCounter = refCounter;
            ref->id = id;
            ref->nextRef = *refs;
            *refs = ref;
        }
    }

    void removeRef(RefInfo** refs, const void* id)
    {
        if (mDoRefTracking) {
            AutoLock autoLock(mLock);
            
            RefInfo* ref = *refs;
            while (ref != NULL) {
                if (ref->id == id) {
                    *refs = ref->nextRef;
                    delete ref;
                    return;
                }
                
                refs = &ref->nextRef;
                ref = *refs;
            }
            
            ERROR_INFO("Ref: removing id %p on Ref %p (WeakRef %p) that doesn't exist!\n", id, mRef, this);
        }
    }

    char* printRefInfos(char* buffer, const RefInfo* refs) const {
		char tmpBuffer[128];
		while (refs) {
			char rcDir = refs->refCounter >= 0 ? '+' : '-';
			sprintf(tmpBuffer, "\t%c ID %p (refCounter %d):\n", rcDir, refs->id, refs->refCounter);
			buffer = (char*) realloc(buffer, strlen(buffer) + strlen(tmpBuffer) + 1);
			strcat(buffer, tmpBuffer);
			refs = refs->nextRef;
		}
		return buffer;
	}

    Lock mLock;
    RefInfo* mStrongRefs;
    RefInfo* mWeakRefs;
    bool mDoRefTracking;
    bool mMemorizeRefOperationsDuringRefTracking;
#endif
};

void Ref::incStrongRef(const void* id) const {
    WeakRefImpl* const refImpl = mRefImpl;
    refImpl->incWeakRef(id);
    refImpl->addStrongRef(id);
    const int32_t oldRefCounter = AtomicInteger::incrementAndGet(&refImpl->mStrongRefCounter);
    ASSERT(oldRefCounter > 0, "Ref::incStrongRef() called on %p with reference counter < 0", refImpl);
#if PRINT_REFS
    DEBUG_INFO("Ref::incStrongRef() of %p from %p: refCounter = %d\n", this, id, oldRefCounter);
#endif
    if (oldRefCounter != INITIAL_STRONG_REF_VALUE)  {
        return;
    }
    AtomicInteger::addAndGet(-INITIAL_STRONG_REF_VALUE, &refImpl->mStrongRefCounter);
    const_cast<Ref*>(this)->onFirstRef();
}

void Ref::decStrongRef(const void* id) const {
    WeakRefImpl* const refImpl = mRefImpl;
    refImpl->removeStrongRef(id);
    const int32_t oldRefCounter = AtomicInteger::decrementAndGet(&refImpl->mStrongRefCounter);
#if PRINT_REFS
    DEBUG_INFO("Ref::decStrongRef() of %p from %p: refCounter = %d\n", this, id, oldRefCounter);
#endif
    ASSERT(oldRefCounter >= 1, "Ref::decStrongRef() called on %p too many times", refImpl);
    if (oldRefCounter == 1) {
        const_cast<Ref*>(this)->onLastStrongRef(id);
        if ((refImpl->mFlags & OBJECT_WEAK_REF_LIFETIME) != OBJECT_WEAK_REF_LIFETIME) {
            if (refImpl->mDestroyer) {
            	refImpl->mDestroyer->destroy(const_cast<Ref*>(this));
            } else {
                delete this;
            }
        }
    }
    refImpl->decWeakRef(id);
}

int32_t Ref::getStrongRefCount() const {
    return mRefImpl->mStrongRefCounter;
}

void Ref::setDestroyer(Ref::Destroyer* destroyer) {
	mRefImpl->mDestroyer = destroyer;
}

Ref* Ref::WeakRef::ref() const {
    return static_cast<const WeakRefImpl*>(this)->mRef;
}

void Ref::WeakRef::incWeakRef(const void* id) {
    WeakRefImpl* const refImpl = static_cast<WeakRefImpl*>(this);
    refImpl->addWeakRef(id);
    const int32_t oldRefCounter = AtomicInteger::incrementAndGet(&refImpl->mWeakRefCounter);
    ASSERT(oldRefCounter >= 0, "Ref::WeakRef::incWeakRef called on %p with reference counter < 0", this);
}

void Ref::WeakRef::decWeakRef(const void* id) {
    WeakRefImpl* const refImpl = static_cast<WeakRefImpl*>(this);
    refImpl->removeWeakRef(id);
    const int32_t oldRefCounter = AtomicInteger::decrementAndGet(&refImpl->mWeakRefCounter);
    ASSERT(oldRefCounter >= 1, "Ref::WeakRef::decWeakRef called on %p too many times", this);
    if (oldRefCounter != 1) return;
    
    if ((refImpl->mFlags & OBJECT_WEAK_REF_LIFETIME) != OBJECT_WEAK_REF_LIFETIME) {
        if (refImpl->mStrongRefCounter == INITIAL_STRONG_REF_VALUE) {
            if (refImpl->mRef) {
                if (refImpl->mDestroyer) {
                	refImpl->mDestroyer->destroy(refImpl->mRef);
                } else {
                    delete refImpl->mRef;
                }
            }
        }
        delete refImpl;
    } else {
    	refImpl->mRef->onLastWeakRef(id);
        if ((refImpl->mFlags & OBJECT_INFINITE_LIFETIME) != OBJECT_INFINITE_LIFETIME) {
            if (refImpl->mRef) {
                if (refImpl->mDestroyer) {
                	refImpl->mDestroyer->destroy(refImpl->mRef);
                } else {
                    delete refImpl->mRef;
                }
            }
        }
    }
}

bool Ref::WeakRef::tryIncStrongRef(const void* id) {
    incWeakRef(id);
    
    WeakRefImpl* const refImpl = static_cast<WeakRefImpl*>(this);
    
    int32_t curRefCounter = refImpl->mStrongRefCounter;
    ASSERT(curRefCounter >= 0, "Ref::WeakRef::tryIncStrong called on %p with reference counter < 0", this);
    while (curRefCounter > 0 && curRefCounter != INITIAL_STRONG_REF_VALUE) {
        if (AtomicInteger::compareAndSwap(curRefCounter, curRefCounter + 1, &refImpl->mStrongRefCounter) == 0) {
            break;
        }
        curRefCounter = refImpl->mStrongRefCounter;
    }
    
    if (curRefCounter <= 0 || curRefCounter == INITIAL_STRONG_REF_VALUE) {
        bool allowStrongRef;
        if (curRefCounter == INITIAL_STRONG_REF_VALUE) {
            // Attempting to acquire the first strong reference.
            // This is allowed if the object does NOT have a longer lifetime,
        	// or if the Ref class allows it.
        	allowStrongRef = (refImpl->mFlags & OBJECT_WEAK_REF_LIFETIME) != OBJECT_WEAK_REF_LIFETIME
                  || refImpl->mRef->onIncStrongRefAttempt(FIRST_INC_STRONG_REF, id);
        } else {
            // Attempting to revive the object.
            // This is allowed if the object DOES have a longer lifetime,
            // and or if the Ref class allows it.
        	allowStrongRef = (refImpl->mFlags & OBJECT_WEAK_REF_LIFETIME) == OBJECT_WEAK_REF_LIFETIME
                  && refImpl->mRef->onIncStrongRefAttempt(FIRST_INC_STRONG_REF, id);
        }
        if (!allowStrongRef) {
            decWeakRef(id);
            return false;
        }
        curRefCounter = AtomicInteger::incrementAndGet(&refImpl->mStrongRefCounter);

        // If the strong reference count has already been incremented by someone else,
        // the implementor of onIncStrongRefAttempt() is holding an unneeded reference.
        // So call onLastStrongRef() here to remove it. (No, this is not pretty.)
        // Note that we MUST NOT do this if we are in fact acquiring the first reference.
        if (curRefCounter > 0 && curRefCounter < INITIAL_STRONG_REF_VALUE) {
        	refImpl->mRef->onLastStrongRef(id);
        }
    }

    refImpl->addStrongRef(id);

#if PRINT_REFS
    DEBUG_INFO("Ref::WeakRef::tryIncStrongRef of %p from %p: refCounter = %d\n", this, id, curRefCounter);
#endif

    if (curRefCounter == INITIAL_STRONG_REF_VALUE) {
    	AtomicInteger::addAndGet(-INITIAL_STRONG_REF_VALUE, &refImpl->mStrongRefCounter);
    	refImpl->mRef->onFirstRef();
    }
    
    return true;
}

bool Ref::WeakRef::tryIncWeakRef(const void* id) {
    WeakRefImpl* const refImpl = static_cast<WeakRefImpl*>(this);
    
    int32_t curRefCounter = refImpl->mWeakRefCounter;
    ASSERT(curRefCounter >= 0, "Ref::WeakRef::tryIncWeakRef called on %p with reference counter < 0", this);
    while (curRefCounter > 0) {
        if (AtomicInteger::compareAndSwap(curRefCounter, curRefCounter + 1, &refImpl->mWeakRefCounter) == 0) {
            break;
        }
        curRefCounter = refImpl->mWeakRefCounter;
    }

    if (curRefCounter > 0) {
    	refImpl->addWeakRef(id);
    }

    return curRefCounter > 0;
}

int32_t Ref::WeakRef::getWeakRefCount() const {
    return static_cast<const WeakRefImpl*>(this)->mWeakRefCounter;
}

void Ref::WeakRef::printRefs() const {
    static_cast<const WeakRefImpl*>(this)->printRefs();
}

void Ref::WeakRef::trackRef(bool doTracking, bool memorizeRefOperationsDuringRefTracking) {
    static_cast<WeakRefImpl*>(this)->trackRef(doTracking, memorizeRefOperationsDuringRefTracking);
}

Ref::WeakRef* Ref::createWeakRef(const void* id) const {
	mRefImpl->incWeakRef(id);
    return mRefImpl;
}

Ref::WeakRef* Ref::getWeakRef() const {
    return mRefImpl;
}

Ref::Ref() : mRefImpl(new WeakRefImpl(this)) {
}

Ref::~Ref() {
    if ((mRefImpl->mFlags & OBJECT_WEAK_REF_LIFETIME) == OBJECT_WEAK_REF_LIFETIME) {
        if (mRefImpl->mWeakRefCounter == 0) {
            delete mRefImpl;
        }
    }
}

void Ref::adjustObjectLifetime(int32_t mode) const {
	AtomicInteger::orAndGet(mode, &mRefImpl->mFlags);
}

void Ref::onFirstRef() {
}

void Ref::onLastStrongRef(const void* id) {
}

bool Ref::onIncStrongRefAttempt(uint32_t flags, const void* id) {
    return (flags & FIRST_INC_STRONG_REF) ? true : false;
}

void Ref::onLastWeakRef(const void* id) {
}

} // namespace os
} // namespace android
