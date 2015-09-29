/* 
 * Unlike a binary search tree, each node of a B+-tree may have a variable number of keys and children.
 * The keys are stored in non-decreasing order. Each node either is a leaf node or
 * it has some associated children that are the root nodes of subtrees.
 * The left child node of a node's element contains all nodes (elements) with keys less than or equal to the node element's key
 * but greater than the preceding node element's key (except for duplicate internal node elements).
 * If a node becomes full, a split operation is performed during the insert operation.
 * The split operation transforms a full node with 2*T-1 elements into two nodes with T-1 and T elements
 * and moves the median key of the two nodes into its parent node.
 * The elements left of the median (middle) element of the splitted node remain in the original node.
 * The new node becomes the child node immediately to the right of the median element that was moved to the parent node.
 * 
 * Example (T = 4):
 * 1.  R = | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
 * 
 * 2.  Add key 8
 *   
 * 3.  R =         | 4 |
 *                 /   \
 *     | 1 | 2 | 3 | -> | 4 | 5 | 6 | 7 | 8 |
 *
 */

public class BPlusTree {
	class Node {
		public int mNumKeys = 0;
		public int[] mKeys = new int[2 * T - 1];
		public Object[] mObjects = new Object[2 * T - 1];
		public Node[] mChildNodes = new Node[2 * T];
		public boolean mIsLeafNode;
		public Node mNextNode;
	}
	
	private Node mRootNode;
	private static final int T = 4;
	
	public BPlusTree() {
		mRootNode = new Node();
		mRootNode.mIsLeafNode = true;		
	}
	
	public void add(int key, Object object) {
		Node rootNode = mRootNode;
		if (rootNode.mNumKeys == (2 * T - 1)) {
			Node newRootNode = new Node();
			mRootNode = newRootNode;
			newRootNode.mIsLeafNode = false;
			mRootNode.mChildNodes[0] = rootNode;
			splitChildNode(newRootNode, 0, rootNode); // Split rootNode and move its median (middle) key up into newRootNode.
			insertIntoNonFullNode(newRootNode, key, object); // Insert the key into the B-Tree with root newRootNode.
		} else {
			insertIntoNonFullNode(rootNode, key, object); // Insert the key into the B-Tree with root rootNode.
		}
	}
	
	// Split the node, node, of a B-Tree into two nodes that contain T-1 (and T) elements and move node's median key up to the parentNode.
	// This method will only be called if node is full; node is the i-th child of parentNode.
	// All internal keys (elements) will have duplicates within the leaf nodes.
	void splitChildNode(Node parentNode, int i, Node node) {
		Node newNode = new Node();
		newNode.mIsLeafNode = node.mIsLeafNode;
		newNode.mNumKeys = T;
		for (int j = 0; j < T; j++) { // Copy the last T elements of node into newNode. Keep the median key as duplicate in the first key of newNode.
			newNode.mKeys[j] = node.mKeys[j + T - 1];
			newNode.mObjects[j] = node.mObjects[j + T - 1];
		}
		if (!newNode.mIsLeafNode) {
			for (int j = 0; j < T + 1; j++) { // Copy the last T + 1 pointers of node into newNode.
				newNode.mChildNodes[j] = node.mChildNodes[j + T - 1];
			}
			for (int j = T; j <= node.mNumKeys; j++) {
				node.mChildNodes[j] = null;
			}
		} else {
			// Manage the linked list that is used e.g. for doing fast range queries.
			newNode.mNextNode = node.mNextNode;
			node.mNextNode = newNode;
		}
		for (int j = T - 1; j < node.mNumKeys; j++) {
			node.mKeys[j] = 0;
			node.mObjects[j] = null;
		}
		node.mNumKeys = T - 1;
		
		// Insert a (child) pointer to node newNode into the parentNode, moving other keys and pointers as necessary.
		for (int j = parentNode.mNumKeys; j >= i + 1; j--) {
			parentNode.mChildNodes[j + 1] = parentNode.mChildNodes[j];
		}
		parentNode.mChildNodes[i + 1] = newNode;	
		for (int j = parentNode.mNumKeys - 1; j >= i; j--) {
			parentNode.mKeys[j + 1] = parentNode.mKeys[j];
			parentNode.mObjects[j + 1] = parentNode.mObjects[j];
		}
		parentNode.mKeys[i] = newNode.mKeys[0];
		parentNode.mObjects[i] = newNode.mObjects[0];
		parentNode.mNumKeys++;
	}
	
	// Insert an element into a B-Tree. (The element will ultimately be inserted into a leaf node).	
	void insertIntoNonFullNode(Node node, int key, Object object) {
		int i = node.mNumKeys - 1;
		if (node.mIsLeafNode) {
			// Since node is not a full node insert the new element into its proper place within node.
			while (i >= 0 && key < node.mKeys[i]) {
				node.mKeys[i + 1] = node.mKeys[i];
				node.mObjects[i + 1] = node.mObjects[i];
				i--;
			}
			i++;
			node.mKeys[i] = key;
			node.mObjects[i] = object;
			node.mNumKeys++;
		} else {
			// Move back from the last key of node until we find the child pointer to the node
			// that is the root node of the subtree where the new element should be placed.
			while (i >= 0 && key < node.mKeys[i]) {
				i--;
			}
			i++;
			if (node.mChildNodes[i].mNumKeys == (2 * T - 1)) {
				splitChildNode(node, i, node.mChildNodes[i]);
				if (key > node.mKeys[i]) {
					i++;
				}
			}
			insertIntoNonFullNode(node.mChildNodes[i], key, object);
		}
	}	
	
	// Recursive search method.
	public Object search(Node node, int key) {		
		int i = 0;
		while (i < node.mNumKeys && key > node.mKeys[i]) {
			i++;
		}
		if (i < node.mNumKeys && key == node.mKeys[i]) {
			return node.mObjects[i];
		}
		if (node.mIsLeafNode) {
			return null;
		} else {
			return search(node.mChildNodes[i], key);
		}	
	}
	
	public Object search(int key) {
		return search(mRootNode, key);
	}
	
	// Iterative search method.
	public Object search2(Node node, int key) {
		while (node != null) {
			int i = 0;
			while (i < node.mNumKeys && key > node.mKeys[i]) {
				i++;
			}
			if (i < node.mNumKeys && key == node.mKeys[i]) {
				return node.mObjects[i];
			}
			if (node.mIsLeafNode) {
				return null;
			} else {
				node = node.mChildNodes[i];
			}
		}
		return null;
	}
	
	public Object search2(int key) {
		return search2(mRootNode, key);
	}
	
	// Inorder walk over the tree.
	public String toString() {
		String string = "";
		Node node = mRootNode;		
		while (!node.mIsLeafNode) {			
			node = node.mChildNodes[0];
		}		
		while (node != null) {
			for (int i = 0; i < node.mNumKeys; i++) {
				string += node.mObjects[i] + ", ";
			}
			node = node.mNextNode;
		}
		return string;
	}
	
	// Inorder walk over parts of the tree.
	public String toString(int fromKey, int toKey) {
		String string = "";
		Node node = getLeafNodeForKey(fromKey);
		while (node != null) {
			for (int j = 0; j < node.mNumKeys; j++) {
				string += node.mObjects[j] + ", ";
				if (node.mKeys[j] == toKey) {
					return string;
				}
			}
			node = node.mNextNode;
		}
		return string;
	}
	
	Node getLeafNodeForKey(int key) {
		Node node = mRootNode;
		while (node != null) {
			int i = 0;
			while (i < node.mNumKeys && key > node.mKeys[i]) {
				i++;
			}
			if (i < node.mNumKeys && key == node.mKeys[i]) {
				node = node.mChildNodes[i + 1];
				while (!node.mIsLeafNode) {			
					node = node.mChildNodes[0];
				}
				return node;
			}
			if (node.mIsLeafNode) {
				return null;
			} else {
				node = node.mChildNodes[i];
			}
		}
		return null;
	}
	
	public static void main(String[] args) {
		BPlusTree bPlusTree = new BPlusTree();
		int primeNumbers[] = new int[] { 2, 3, 5, 7, 11, 13, 19, 23, 37, 41, 43, 47, 53, 59, 67, 71, 61, 73, 79, 89,
				97, 101, 103, 109, 29, 31, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 17, 83, 107 };
		
		for (int i = 0; i < primeNumbers.length; i++) {
			bPlusTree.add(primeNumbers[i], String.valueOf(primeNumbers[i]));
		}		
		
		for (int i = 0; i < primeNumbers.length; i++) {
			String value = String.valueOf(primeNumbers[i]);
			Object searchResult = (Object) bPlusTree.search(primeNumbers[i]);			
			if (!value.equals(searchResult)) {
				System.out.println("Oops: Key " + primeNumbers[i] + " retrieved object " + searchResult);
			}
		}		
		
		System.out.println(bPlusTree.search(11));
		System.out.println(bPlusTree.search(17));
		System.out.println(bPlusTree.toString());
		System.out.println(bPlusTree.toString(19, 71));
	}
}
