public class Quicksort {
	public static void quicksort(char[] string, int leftIndex, int rightIndex) {
		if (leftIndex < rightIndex) {
			int pivotIndex = partition(string, leftIndex, rightIndex);
			quicksort(string, leftIndex, pivotIndex - 1);
			quicksort(string, pivotIndex + 1, rightIndex);
		}
	}
	
	static int partition(char[] string, int leftIndex, int rightIndex) {		
		int pivotIndex = rightIndex; /* pivot element index */
		int storageIndex = leftIndex; /* divider index for the pivot element */
		
		for (int i = leftIndex; i < rightIndex; i++) {
			if (string[i] < string[pivotIndex]) {
				swap(string, i, storageIndex);
				storageIndex++;
			}
		}
		swap(string, pivotIndex, storageIndex);
		
		return storageIndex;
	}
	
	static void swap(char[] string, int index1, int index2) {
		char tempValue = string[index2];
		string[index2] = string[index1];
		string[index1] = tempValue;	
	}
	
	public static void sort(char[] string) {
		quicksort(string, 0, string.length - 1);
	}
	
	public static void main(String[] args) {
		char[] string = { 'x', 'c', 'b', 'a', 'g', 'h', 'i' };
		sort(string);
		for (int i = 0; i < string.length; i++) {
			if (i > 0) {
				System.out.print(", ");
			}
			System.out.print(string[i]);
		}
		System.out.println();
	}
}
