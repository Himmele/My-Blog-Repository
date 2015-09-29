// A (unordered) binary tree is a tree data structure in which each node has at most two child nodes, usually distinguished as "left" and "right".
// Nodes with children are parent nodes, and child nodes may contain references to their parents.
// A heap is always a perfectly-balanced fully-populated tree data structure that satisfies the heap property:
// If B is a child node of A, then key(A) ³ key(B). This implies that an element with the greatest key is always in the root node,
// and so such a heap is sometimes called a max-heap.

public class BinaryHeap {
	private Integer[] mArray;
	private int mHeapSize;
	
	public BinaryHeap(int maxSize) {
		mArray = new Integer[maxSize];
		mHeapSize = 0;
	}
	
	int parentIndex(int i) {
		return (i + 1) / 2 - 1;
	}
	
	int leftChildIndex(int i) {
		return 2 * i + 1;
	}
	
	int rightChildIndex(int i) {
		return 2 * i + 2;
	}
	
	// When maxHeapify is called, it is assumed that the binary tree rooted at leftChildIndex(i) and rightChildIndex(i) are max-heaps.
	// Worst-case performance: O(log n).
	void maxHeapify(int i) {
		int leftChildIndex = leftChildIndex(i);
		int rightChildIndex = rightChildIndex(i);
		int largestElementIndex;
		if (leftChildIndex < mHeapSize && mArray[leftChildIndex] > mArray[i]) {
			largestElementIndex = leftChildIndex;
		} else {
			largestElementIndex = i;
		}
		if (rightChildIndex < mHeapSize && mArray[rightChildIndex] > mArray[largestElementIndex]) {
			largestElementIndex = rightChildIndex;
		}
		if (largestElementIndex != i) {
			int tmpValue = mArray[i];
			mArray[i] = mArray[largestElementIndex];
			mArray[largestElementIndex] = tmpValue;
			maxHeapify(largestElementIndex);
		}
	}
	
	void buildMaxHeap() {
		int heapSize = mArray.length;
		for (int i = heapSize / 2; i >= 0; i--) {
			maxHeapify(i);
		}
	}	
	
	public int max() {
		return mArray[0];
	}
	
	// Worst-case performance: O(log n).
	public int extractMax() {
		int max = mArray[0];
		mArray[0] = mArray[mHeapSize - 1];
		mArray[mHeapSize - 1] = null;
		mHeapSize--;
		maxHeapify(0);
		return max;
	}
	
	// Worst-case performance: O(log n).
	void increaseKey(int i, int newValue) throws Exception {
		if (newValue < mArray[i]) {
			throw new Exception("New value is smaller than current value");
		}
		mArray[i] = newValue;
		while (i > 0 && mArray[parentIndex(i)] < mArray[i]) {
			int tmpValue = mArray[parentIndex(i)];
			mArray[parentIndex(i)] = mArray[i];
			mArray[i] = tmpValue;
			i = parentIndex(i);
		}		
	}
	
	// Worst-case performance: O(log n).
	public boolean insert(int value) {
		if (mHeapSize < mArray.length) {
			mHeapSize++;
			mArray[mHeapSize - 1] = value;
			try {
				increaseKey(mHeapSize - 1, value);
			} catch (Exception e) {
				return false;
			}
			return true;
		} else {
			return false;
		}
	}
	
	public int size() {
		return mHeapSize;
	}
	
	public String toString() {
		String string = "";
		for (int i = 0; i < mHeapSize; i++) {
			string += mArray[i] + " ";
		}
		return string;
	}
	
	public static void main(String[] args) {
		Integer[] array = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
		
		BinaryHeap heap = new BinaryHeap(100);
		for (int i : array) {
			heap.insert(i);
		}
		
		System.out.println(heap.toString());
		int heapSize = heap.size();
		for (int i = 0; i < heapSize; i++) {
			System.out.print(heap.extractMax() + " ");
		}
		System.out.println();
		
		for (int i : array) {
			heap.insert(i);
		}
		heap.insert(15);
		heap.insert(100);
		heap.insert(17);
		heap.insert(42);
		
		heapSize = heap.size();
		for (int i = 0; i < heapSize; i++) {
			System.out.print(heap.extractMax() + " ");
		}
		System.out.println();
	}
}
