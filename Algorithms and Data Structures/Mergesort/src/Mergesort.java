import java.util.Queue;
import java.util.LinkedList;

public class Mergesort {
	public static void mergesort(char[] string, int leftIndex, int rightIndex) {
		if (leftIndex < rightIndex) {
			int middleIndex = (leftIndex + rightIndex) / 2;
			mergesort(string, leftIndex, middleIndex);
			mergesort(string, middleIndex + 1, rightIndex);
			merge(string, leftIndex, middleIndex, rightIndex);
		}
	}
	
	static void merge(char[] string, int leftIndex, int middleIndex, int rightIndex) {
		Queue<Character> string1 = new LinkedList<Character>();
		Queue<Character> string2 = new LinkedList<Character>();
		
		for (int i = leftIndex; i <= middleIndex; i++) {
			string1.add(string[i]);
		}		
		for (int i = middleIndex + 1; i <= rightIndex; i++) {
			string2.add(string[i]);
		}
		
		int i = leftIndex;
		while (!string1.isEmpty() && !string2.isEmpty()) {
			if (string1.peek() <= string2.peek()) {
				string[i++] = string1.poll();
			} else {
				string[i++] = string2.poll();
			}
		}
		
		while (!string1.isEmpty()) {
			string[i++] = string1.poll();
		}
		while (!string2.isEmpty()) {
			string[i++] = string2.poll();
		}
	}
	
	public static void sort(char[] string) {
		mergesort(string, 0, string.length - 1);
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
