public class SmithWaterman {
	char[] mSeqA;
	char[] mSeqB;
	int[][] mD;
	int mScore;
	String mAlignmentSeqA = "";
	String mAlignmentSeqB = "";
	
	void init(char[] seqA, char[] seqB) {
		mSeqA = seqA;
		mSeqB = seqB;
		mD = new int[mSeqA.length + 1][mSeqB.length + 1];
		for (int i = 0; i <= mSeqA.length; i++) {
			mD[i][0] = 0;			
		}
		for (int j = 0; j <= mSeqB.length; j++) {
			mD[0][j] = 0;
		}
	}
	
	void process() {
		for (int i = 1; i <= mSeqA.length; i++) {
			for (int j = 1; j <= mSeqB.length; j++) {
				int scoreDiag = mD[i-1][j-1] + weight(i, j);
				int scoreLeft = mD[i][j-1] - 1;
				int scoreUp = mD[i-1][j] - 1;
				mD[i][j] = Math.max(Math.max(Math.max(scoreDiag, scoreLeft), scoreUp), 0);
			}
		}
	}
	
	void backtrack() {
		int i = 1;
		int j = 1;
		int max = mD[i][j];

		for (int k = 1; k <= mSeqA.length; k++) {
			for (int l = 1; l <= mSeqB.length; l++) {
				if (mD[k][l] > max) {
					i = k;
					j = l;
					max = mD[k][l];
				}
			}
		}
		
		mScore = mD[i][j];
		
		int k = mSeqA.length;
		int l = mSeqB.length;
		
		while (k > i) {
			mAlignmentSeqB += "-";
			mAlignmentSeqA += mSeqA[k - 1];
			k--;
		}
		while (l > j) {
			mAlignmentSeqA += "-";
			mAlignmentSeqB += mSeqB[l - 1];
			l--;
		}
		
		while (mD[i][j] != 0) {			
			if (mD[i][j] == mD[i-1][j-1] + weight(i, j)) {				
				mAlignmentSeqA += mSeqA[i-1];
				mAlignmentSeqB += mSeqB[j-1];
				i--;
				j--;				
				continue;
			} else if (mD[i][j] == mD[i][j-1] - 1) {
				mAlignmentSeqA += "-";
				mAlignmentSeqB += mSeqB[j-1];
				j--;
				continue;
			} else {
				mAlignmentSeqA += mSeqA[i-1];
				mAlignmentSeqB += "-";
				i--;
				continue;
			}
		}
		
		while (i > 0) {
			mAlignmentSeqB += "-";
			mAlignmentSeqA += mSeqA[i - 1];
			i--;
		}
		while (j > 0) {
			mAlignmentSeqA += "-";
			mAlignmentSeqB += mSeqB[j - 1];
			j--;
		}
		
		mAlignmentSeqA = new StringBuffer(mAlignmentSeqA).reverse().toString();
		mAlignmentSeqB = new StringBuffer(mAlignmentSeqB).reverse().toString();
	}
	
	private int weight(int i, int j) {
		if (mSeqA[i - 1] == mSeqB[j - 1]) {
			return 2;
		} else {
			return -1;
		}
	}
	
	void printMatrix() {
		System.out.print("D =       ");
		for (int i = 0; i < mSeqB.length; i++) {
			System.out.print(String.format("%4c ", mSeqB[i]));
		}
		System.out.println();
		for (int i = 0; i < mSeqA.length + 1; i++) {
			if (i > 0) {
				System.out.print(String.format("%4c ", mSeqA[i-1]));
			} else {
				System.out.print("     ");
			}
			for (int j = 0; j < mSeqB.length + 1; j++) {
				System.out.print(String.format("%4d ", mD[i][j]));
			}
			System.out.println();
		}
		System.out.println();
	}
	
	void printScoreAndAlignments() {
		System.out.println("Score: " + mScore);
		System.out.println("Sequence A: " + mAlignmentSeqA);
		System.out.println("Sequence B: " + mAlignmentSeqB);
		System.out.println();
	}
	
	public static void main(String [] args) {		
		char[] seqB = { 'A', 'C', 'G', 'A' };
		char[] seqA = { 'T', 'C', 'C', 'G' };
		
		SmithWaterman sw = new SmithWaterman();
		sw.init(seqA, seqB);		
		sw.process();
		sw.backtrack();
		
		sw.printMatrix();
		sw.printScoreAndAlignments();
	}
}
