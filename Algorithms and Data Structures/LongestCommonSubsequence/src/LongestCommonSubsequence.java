public class LongestCommonSubsequence {
	char[] mSeqA;
	char[] mSeqB;
	int[][] mC;
	int[][] mB;
	private static int UP = 0;
	private static int DIAG = 1;
	private static int LEFT = 2;
	
	public LongestCommonSubsequence(char[] seqA, char[] seqB) {
		mSeqA = new char[seqA.length + 1];
		for (int i = 0; i < seqA.length; i++) {
			mSeqA[i+1] = seqA[i];
		}		
		mSeqB = new char[seqB.length + 1];
		for (int i = 0; i < seqB.length; i++) {
			mSeqB[i+1] = seqB[i];	
		}
		mC = new int[mSeqA.length][mSeqB.length];
		mB = new int[mSeqA.length][mSeqB.length];
		for (int i = 0; i < mSeqA.length; i++) {
			mC[i][0] = 0;
		}		
		for (int j = 0; j < mSeqB.length; j++) {
			mC[0][j] = 0;
		}
	}
	
	// O(m + n) -> O(mSeqA.length + mSeqB.length)
	public void lcsLength() {
		for (int i = 1; i < mSeqA.length; i++) {
			for (int j = 1; j < mSeqB.length; j++) {
				if (mSeqA[i] == mSeqB[j]) {
					mC[i][j] = mC[i-1][j-1] + 1;
					mB[i][j] = DIAG;
				} else if (mC[i-1][j] >= mC[i][j-1]) {
					mC[i][j] = mC[i-1][j];
					mB[i][j] = UP;
				} else {
					mC[i][j] = mC[i][j-1];
					mB[i][j] = LEFT;
				}
			}
		}
	}
	
	public void backtrack() {
		backtrack(mSeqA.length - 1, mSeqB.length - 1);
	}
	
	void backtrack(int i, int j) {
		if (i == 0 || j == 0) {
			return;
		}
		if (mB[i][j] == DIAG) {
			backtrack(i-1, j-1);
			System.out.print(mSeqA[i] + ", ");
		} else if (mB[i][j] == UP) {
			backtrack(i-1, j);			
		} else {
			backtrack(i, j-1);
		}
	}
	
	public static void main(String[] args) {
		char[] seqA = new char[] { 'A', 'B', 'C', 'B', 'D', 'A', 'B' };
		char[] seqB = new char[] { 'B', 'D', 'C', 'A', 'B', 'A' };
		LongestCommonSubsequence lcs = new LongestCommonSubsequence(seqA, seqB);
		lcs.lcsLength();
		lcs.backtrack();
	}
}
