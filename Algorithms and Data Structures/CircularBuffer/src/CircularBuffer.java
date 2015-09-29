/* 
 * A circular buffer, cyclic buffer or ring buffer is a data structure that uses a single, fixed-size buffer
 * as if it were connected end-to-end. This structure lends itself easily to buffering data streams.
 * Circular buffering makes a good implementation strategy for a Queue that has fixed maximum size.
 * For arbitrarily expanding queues, a linked list approach may be preferred instead.
 * A small disadvantage of relying on pointers or relative indices of the start and end of data is,
 * that in the case the buffer is entirely full, both pointers point to the same element.
 * To solve this confusion there are a number of solutions: This implementation always keeps one slot open.
 *
 */

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

public class CircularBuffer<T> {
	public CircularBuffer(int size) {
		SIZE = size;
		mReadIndex = 0;
		mWriteIndex = 0;
		mArray = new Object[SIZE];		
	}
	
	// Inserts the specified element into this queue and blocks to do so until there is enough space in the circular buffer to do so.
	public boolean offer(T element) {
		mLock.lock();
		try {
			while (isFull()) {
				mWriteCondVar.await();
			} 
			mArray[mWriteIndex] = element;
			mWriteIndex = (mWriteIndex + 1) % SIZE;
			mReadCondVar.signal();
			return true;
		} catch (InterruptedException e) {
			return false;
		} finally {
			mLock.unlock();
		}
	}
	
	// Retrieves, but does not remove, the head of this circular buffer. This method blocks until there is an element available.
	@SuppressWarnings("unchecked")
	T peek() {
		mLock.lock();
		try {
			while (isEmpty()) {
				mReadCondVar.await();
			}
			return (T) mArray[mReadIndex];
		} catch (InterruptedException e) {
			return null;
		} finally {
			mLock.unlock();
		}
	}
	
	// Retrieves and removes the head of this circular buffer. This method blocks until there is an element available.
	@SuppressWarnings("unchecked")
	T poll() {
		mLock.lock();
		try {
			while (isEmpty()) {
				mReadCondVar.await();
			}
			T element = (T) mArray[mReadIndex];
			mReadIndex = (mReadIndex + 1) % SIZE;
			mWriteCondVar.signal();
			return element;
		} catch (InterruptedException e) {
			return null;
		} finally {
			mLock.unlock();
		}
	}
	
	public boolean isEmpty() {
		return mReadIndex == mWriteIndex;		
	}
	
	public boolean isFull() {
		return (mWriteIndex + 1) % SIZE == mReadIndex;
	}
	
	private final int SIZE;
	private int mReadIndex;
	private int mWriteIndex;
	private Object[] mArray;
	private final Lock mLock = new ReentrantLock();
	private final Condition mReadCondVar = mLock.newCondition();
	private final Condition mWriteCondVar = mLock.newCondition();
}
