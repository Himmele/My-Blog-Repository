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
 * Some extenstions and refactorings by Daniel Himmelein
 */

#ifndef ANDROID_OS_REF_H_
#define ANDROID_OS_REF_H_

#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>
#include <assert.h>
#include "android/os/Utils.h"
#include "android/os/AtomicInteger.h"

#define ASSERT(cond, ...) if (!(cond)) { printf(__VA_ARGS__); printf("\n"); assert(cond); }
#define DEBUG_INFO(...) printf(__VA_ARGS__)
#define ERROR_INFO(...) printf(__VA_ARGS__)

namespace android {
namespace os {

template<typename T> class sp;
template<typename T> class wp;

#define COMPARE_WEAK_REF(_operator_)                            \
inline bool operator _operator_ (const sp<T>& o) const {        \
    return mPointer _operator_ o.mPointer;                      \
}                                                               \
inline bool operator _operator_ (const T* o) const {            \
    return mPointer _operator_ o;                               \
}                                                               \
template<typename U>                                            \
inline bool operator _operator_ (const sp<U>& o) const {        \
    return mPointer _operator_ o.mPointer;                      \
}                                                               \
template<typename U>                                            \
inline bool operator _operator_ (const U* o) const {            \
    return mPointer _operator_ o;                               \
}

#define COMPARE_REF(_operator_)                                 \
COMPARE_WEAK_REF(_operator_)                                    \
inline bool operator _operator_ (const wp<T>& o) const {        \
    return mPointer _operator_ o.mPointer;                      \
}                                                               \
template<typename U>                                            \
inline bool operator _operator_ (const wp<U>& o) const {        \
    return mPointer _operator_ o.mPointer;                      \
}


class Ref
{
public:
	void incStrongRef(const void* id) const;
	void decStrongRef(const void* id) const;

    class WeakRef
    {
    public:
        Ref* ref() const;
        void incWeakRef(const void* id);
        void decWeakRef(const void* id);
        bool tryIncStrongRef(const void* id);

        // Debugging APIs
        // This is only safe if OBJECT_INFINITE_LIFETIME is set
        bool tryIncWeakRef(const void* id);
        int32_t getWeakRefCount() const;
        void printRefs() const;
        // Object tracking
        void trackRef(bool doTracking, bool memorizeRefOperationsDuringRefTracking);
    };
    
    WeakRef* createWeakRef(const void* id) const;
    WeakRef* getWeakRef() const;

    // Debugging APIs
    int32_t getStrongRefCount() const;
    // Print all references for this object
    inline  void printRefs() const {
    	getWeakRef()->printRefs();
    }
    inline  void trackRef(bool doTracking, bool memorizeRefOperationsDuringRefTracking) {
        getWeakRef()->trackRef(doTracking, memorizeRefOperationsDuringRefTracking);
    }

    // Ref destroyer
    class Destroyer {
    public:
        virtual ~Destroyer();

    protected:
        template <typename T>
        sp<T> reviveObject(Ref* ref);

    private:
        virtual void destroy(Ref* ref) = 0;

        friend class Ref;
    };

    void setDestroyer(Destroyer* destroyer);

protected:
    Ref();
    virtual ~Ref();

    enum {
        OBJECT_WEAK_REF_LIFETIME = 0x0001,
        OBJECT_INFINITE_LIFETIME = 0x0003
    };
    
    void adjustObjectLifetime(int32_t mode) const;

    enum {
        FIRST_INC_STRONG_REF = 0x0001
    };
    
    virtual void onFirstRef();
    virtual void onLastStrongRef(const void* id);
    virtual bool onIncStrongRefAttempt(uint32_t flags, const void* id);
    virtual void onLastWeakRef(const void* id);

private:
    class WeakRefImpl;
    WeakRefImpl* const mRefImpl;

    friend class WeakRef;
    
    template <typename Y>
    friend sp<Y> Ref::Destroyer::reviveObject(Ref* ref);

    NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Ref)
};


template <class T>
class LightweightRef
{
public:
    inline LightweightRef() : mRefCounter(0) { }

    inline void incStrongRef(const void* id) const {
    	AtomicInteger::incrementAndGet(&mRefCounter);
    }

    inline void decStrongRef(const void* id) const {
        if (AtomicInteger::decrementAndGet(&mRefCounter) == 1) {
            delete static_cast<const T*>(this);
        }
    }

    // Debugging APIs
    inline int32_t getStrongRefCount() const {
        return mRefCounter;
    }

protected:
    inline ~LightweightRef() { }

private:
    mutable volatile int32_t mRefCounter;
};


template <typename T>
class sp
{
public:
    typedef typename Ref::WeakRef WeakRef;
    
    inline sp() : mPointer(NULL) { }

    sp(T* other);
    sp(const sp<T>& other);
    template<typename U> sp(U* other);
    template<typename U> sp(const sp<U>& other);

    ~sp();
    
    // Assignment operators
    sp& operator= (T* other);
    sp& operator= (const sp<T>& other);
    
    template<typename U> sp& operator= (const sp<U>& other);
    template<typename U> sp& operator= (U* other);
    
    // Accessor methods
    inline T& operator*() const { return *mPointer; }
    inline T* operator->() const { return mPointer; }
    inline T* getPointer() const { return mPointer; }
    inline T const* getConstPointer() const { return mPointer; }

    // Operators
    COMPARE_REF(==)
    COMPARE_REF(!=)
    COMPARE_REF(>)
    COMPARE_REF(<)
    COMPARE_REF(<=)
    COMPARE_REF(>=)

    void clear();

private:    
    template<typename Y> friend class sp;
    template<typename Y> friend class wp;

    void setPointer(T* pointer);
    
    T* mPointer;

    template <typename Y>
    friend sp<Y> Ref::Destroyer::reviveObject(Ref* ref);
};

template <typename T>
class wp
{
public:
    typedef typename Ref::WeakRef WeakRef;
    
    inline wp() : mPointer(NULL) { }

    wp(T* other);
    wp(const wp<T>& other);
    wp(const sp<T>& other);
    template<typename U> wp(U* other);
    template<typename U> wp(const sp<U>& other);
    template<typename U> wp(const wp<U>& other);

    ~wp();
    
    sp<T> toStrongRef() const;

    // Assignment operators
    wp& operator= (T* other);
    wp& operator= (const wp<T>& other);
    wp& operator= (const sp<T>& other);
    
    template<typename U> wp& operator= (U* other);
    template<typename U> wp& operator= (const wp<U>& other);
    template<typename U> wp& operator= (const sp<U>& other);

    // Operators
    COMPARE_WEAK_REF(==)
    COMPARE_WEAK_REF(!=)
    COMPARE_WEAK_REF(>)
    COMPARE_WEAK_REF(<)
    COMPARE_WEAK_REF(<=)
    COMPARE_WEAK_REF(>=)

    void clear();

    inline bool operator== (const wp<T>& o) const {
        return (mPointer == o.mPointer) && (mRef == o.mRef);
    }

    template<typename U>
    inline bool operator== (const wp<U>& o) const {
        return mPointer == o.mPointer;
    }

    inline bool operator> (const wp<T>& o) const {
        return (mPointer == o.mPointer) ? (mRef > o.mRef) : (mPointer > o.mPointer);
    }

    template<typename U>
    inline bool operator> (const wp<U>& o) const {
        return (mPointer == o.mPointer) ? (mRef > o.mRef) : (mPointer > o.mPointer);
    }

    inline bool operator< (const wp<T>& o) const {
        return (mPointer == o.mPointer) ? (mRef < o.mRef) : (mPointer < o.mPointer);
    }

    template<typename U>
    inline bool operator< (const wp<U>& o) const {
        return (mPointer == o.mPointer) ? (mRef < o.mRef) : (mPointer < o.mPointer);
    }

    inline bool operator!= (const wp<T>& o) const { return mRef != o.mRef; }

    template<typename U> inline bool operator!= (const wp<U>& o) const { return !operator == (o); }

    inline bool operator<= (const wp<T>& o) const { return !operator > (o); }

    template<typename U> inline bool operator<= (const wp<U>& o) const { return !operator > (o); }

    inline bool operator>= (const wp<T>& o) const { return !operator < (o); }

    template<typename U> inline bool operator>= (const wp<U>& o) const { return !operator < (o); }

private:
    template<typename Y> friend class sp;
    template<typename Y> friend class wp;

    T* mPointer;
    WeakRef* mRef;
};

#undef COMPARE_REF
#undef COMPARE_WEAK_REF


template<typename T>
sp<T>::sp(T* other) : mPointer(other) {
    if (other) other->incStrongRef(this);
}

template<typename T>
sp<T>::sp(const sp<T>& other) : mPointer(other.mPointer) {
    if (mPointer) mPointer->incStrongRef(this);
}

template<typename T> template<typename U>
sp<T>::sp(U* other) : mPointer(other) {
    if (other) ((T*)other)->incStrongRef(this);
}

template<typename T> template<typename U>
sp<T>::sp(const sp<U>& other) : mPointer(other.mPointer) {
    if (mPointer) mPointer->incStrongRef(this);
}

template<typename T>
sp<T>::~sp() {
    if (mPointer) mPointer->decStrongRef(this);
}

template<typename T>
sp<T>& sp<T>::operator= (const sp<T>& other) {
    T* otherPointer(other.mPointer);
    if (otherPointer) otherPointer->incStrongRef(this);
    if (mPointer) mPointer->decStrongRef(this);
    mPointer = otherPointer;
    return *this;
}

template<typename T>
sp<T>& sp<T>::operator= (T* other) {
    if (other) other->incStrongRef(this);
    if (mPointer) mPointer->decStrongRef(this);
    mPointer = other;
    return *this;
}

template<typename T> template<typename U>
sp<T>& sp<T>::operator= (const sp<U>& other) {
    T* otherPointer(other.mPointer);
    if (otherPointer) otherPointer->incStrongRef(this);
    if (mPointer) mPointer->decStrongRef(this);
    mPointer = otherPointer;
    return *this;
}

template<typename T> template<typename U>
sp<T>& sp<T>::operator= (U* other) {
    if (other) ((T*)other)->incStrongRef(this);
    if (mPointer) mPointer->decStrongRef(this);
    mPointer = other;
    return *this;
}

template<typename T>
void sp<T>::clear() {
    if (mPointer) {
    	// If decStrongRef triggers the sp<T>'s dtor make sure that mPointer is already set to NULL.
    	T* pointer = mPointer;
    	mPointer = NULL;
    	pointer->decStrongRef(this);
    }
}

template<typename T>
void sp<T>::setPointer(T* pointer) {
    mPointer = pointer;
}


template<typename T>
wp<T>::wp(T* other) : mPointer(other) {
    if (other) mRef = other->createWeakRef(this);
}

template<typename T>
wp<T>::wp(const wp<T>& other) : mPointer(other.mPointer), mRef(other.mRef) {
    if (mPointer) mRef->incWeakRef(this);
}

template<typename T>
wp<T>::wp(const sp<T>& other) : mPointer(other.mPointer) {
    if (mPointer) {
        mRef = mPointer->createWeakRef(this);
    }
}

template<typename T> template<typename U>
wp<T>::wp(U* other) : mPointer(other) {
    if (other) mRef = other->createWeakRef(this);
}

template<typename T> template<typename U>
wp<T>::wp(const wp<U>& other) : mPointer(other.mPointer) {
    if (mPointer) {
        mRef = other.mRef;
        mRef->incWeakRef(this);
    }
}

template<typename T> template<typename U>
wp<T>::wp(const sp<U>& other) : mPointer(other.mPointer) {
    if (mPointer) {
        mRef = mPointer->createWeakRef(this);
    }
}

template<typename T>
wp<T>::~wp() {
    if (mPointer) mRef->decWeakRef(this);
}

template<typename T>
wp<T>& wp<T>::operator= (T* other) {
    WeakRef* newRef = other ? other->createWeakRef(this) : NULL;
    if (mPointer) mRef->decWeakRef(this);
    mPointer = other;
    mRef = newRef;
    return *this;
}

template<typename T>
wp<T>& wp<T>::operator= (const wp<T>& other) {
    WeakRef* otherRef(other.mRef);
    T* otherPointer(other.mPointer);
    if (otherPointer) otherRef->incWeakRef(this);
    if (mPointer) mRef->decWeakRef(this);
    mPointer = otherPointer;
    mRef = otherRef;
    return *this;
}

template<typename T>
wp<T>& wp<T>::operator= (const sp<T>& other) {
    WeakRef* newRef = other != NULL ? other->createWeakRef(this) : NULL;
    T* otherPointer(other.mPointer);
    if (mPointer) mRef->decWeakRef(this);
    mPointer = otherPointer;
    mRef = newRef;
    return *this;
}

template<typename T> template<typename U>
wp<T>& wp<T>::operator= (U* other) {
    WeakRef* newRef = other ? other->createWeakRef(this) : NULL;
    if (mPointer) mRef->decWeakRef(this);
    mPointer = other;
    mRef = newRef;
    return *this;
}

template<typename T> template<typename U>
wp<T>& wp<T>::operator= (const wp<U>& other) {
    WeakRef* otherRef(other.mRef);
    U* otherPointer(other.mPointer);
    if (otherPointer) otherRef->incWeakRef(this);
    if (mPointer) mRef->decWeakRef(this);
    mPointer = otherPointer;
    mRef = otherRef;
    return *this;
}

template<typename T> template<typename U>
wp<T>& wp<T>::operator= (const sp<U>& other) {
    WeakRef* newRef = other != NULL ? other->createWeakRef(this) : NULL;
    U* otherPointer(other.mPointer);
    if (mPointer) mRef->decWeakRef(this);
    mPointer = otherPointer;
    mRef = newRef;
    return *this;
}

template<typename T>
sp<T> wp<T>::toStrongRef() const {
    sp<T> newRef;
    if (mPointer && mRef->tryIncStrongRef(&newRef)) {
        newRef.setPointer(mPointer);
    }
    return newRef;
}

template<typename T>
void wp<T>::clear() {
    if (mPointer) {
    	// If decWeakRef triggers the wp<T>'s dtor make sure that mPointer is already set to NULL.
    	mPointer = NULL;
        mRef->decWeakRef(this);
    }
}

template <typename T>
sp<T> Ref::Destroyer::reviveObject(Ref* ref) {
	sp<T> newRef;
	if (ref != NULL) {
		ref->adjustObjectLifetime(OBJECT_WEAK_REF_LIFETIME);
		if (ref->getWeakRef()->tryIncStrongRef(&newRef)) {
			newRef.setPointer(static_cast<T*>(ref));
		}
		ref->adjustObjectLifetime(0);
	}
	return newRef;
}

} /* namespace os */
} /* namespace android */

#endif /* ANDROID_OS_REF_H_ */
