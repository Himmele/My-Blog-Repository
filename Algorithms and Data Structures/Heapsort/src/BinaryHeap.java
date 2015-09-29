// A (unordered) binary tree is a tree data structure in which each node has at most two child nodes, usually distinguished as "left" and "right".
// Nodes with children are parent nodes, and child nodes may contain references to their parents.
// A heap is always a perfectly-balanced fully-populated tree data structure that satisfies the heap property:
// If B is a child node of A, then key(A) ³ key(B). This implies that an element with the greatest key is always in the root node,
// and so such a heap is sometimes called a max-heap.

public class BinaryHeap {
	static int parentIndex(int i) {
		return (i + 1) / 2 - 1;
	}
	
	static int leftChildIndex(int i) {
		return 2 * i + 1;
	}
	
	static int rightChildIndex(int i) {
		return 2 * i + 2;
	}
	
	// When maxHeapify is called, it is assumed that the binary tree rooted at leftChildIndex(i) and rightChildIndex(i) are max-heaps.
	static void maxHeapify(int[] array, int heapSize, int i) {
		int leftChildIndex = leftChildIndex(i);
		int rightChildIndex = rightChildIndex(i);
		int largestElementIndex;
		if (leftChildIndex < heapSize && array[leftChildIndex] > array[i]) {
			largestElementIndex = leftChildIndex;
		} else {
			largestElementIndex = i;
		}
		if (rightChildIndex < heapSize && array[rightChildIndex] > array[largestElementIndex]) {
			largestElementIndex = rightChildIndex;
		}
		if (largestElementIndex != i) {
			int tmpValue = array[i];
			array[i] = array[largestElementIndex];
			array[largestElementIndex] = tmpValue;
			maxHeapify(array, heapSize, largestElementIndex);
		}
	}
	
	static void buildMaxHeap(int[] array) {
		int heapSize = array.length;
		for (int i = heapSize / 2; i >= 0; i--) {
			maxHeapify(array, heapSize, i);
		}
	}
	
	// O(n log(n))
	static void heapsort(int[] array) {
		buildMaxHeap(array);
		for (int heapSize = array.length; heapSize > 0; heapSize--) {
			int tmpValue = array[heapSize - 1];
			array[heapSize - 1] = array[0];
			array[0] = tmpValue;	
			maxHeapify(array, heapSize - 1, 0);
		}
	}
	
	public static void main(String[] args) {
		int[] array1 = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
		buildMaxHeap(array1);
		System.out.print("Max-Heap: ");
		for (int i : array1) {
			System.out.print(i + " ");
		}
		System.out.println();
		
		int[] array2 = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
		heapsort(array2);
		System.out.print("Heapsort: ");
		for (int i : array2) {
			System.out.print(i + " ");
		}
		System.out.println();
	}
}
