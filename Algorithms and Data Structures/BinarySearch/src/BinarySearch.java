public class BinarySearch {
	public static int search(final char character, final char[] alphabet) {
        int leftIndex = 0;
        int rightIndex = alphabet.length - 1;        
 
        while (leftIndex <= rightIndex) {
            final int middleIndex = leftIndex + ((rightIndex - leftIndex) / 2);
            if (alphabet[middleIndex] < character) {
            	leftIndex = middleIndex + 1;
            } else if (alphabet[middleIndex] > character) {
            	rightIndex = middleIndex - 1;
            } else {
                return middleIndex;
            }
        }
 
        return -1;
    }
	
	public static int recuriveSearch(int leftIndex, int rightIndex, final char character, final char[] alphabet) { 
        if (leftIndex > rightIndex) {
        	return -1;        	
        }
        		
        final int middleIndex = leftIndex + ((rightIndex - leftIndex) / 2);
        if (alphabet[middleIndex] < character) {
        	return recuriveSearch(middleIndex + 1, rightIndex, character, alphabet);            	
        } else if (alphabet[middleIndex] > character) {
        	return recuriveSearch(leftIndex, middleIndex - 1, character, alphabet);            	
        } else {
            return middleIndex;
        }
    }
	
	public static void main(String[] args) {
		final char[] alphabet = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
				'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 
				'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };		
		char character = 'o';
		int position = BinarySearch.search(character, alphabet);
		System.out.println(character + " is at position " + (position + 1) + " of the alphabet");
		
		character = 'v';
		position = BinarySearch.recuriveSearch(0, alphabet.length - 1, character, alphabet);
		System.out.println(character + " is at position " + (position + 1) + " of the alphabet");
	}
}
