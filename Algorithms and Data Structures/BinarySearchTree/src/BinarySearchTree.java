public class BinarySearchTree {
	class Node {
		public int mKey;
		public Object mObject;
		public Node mParentNode;
		public Node mLeftNode;
		public Node mRightNode;
	}
	
	private Node mRootNode;
	
	// Recursive add method.
	public void add(Node node, Node parentNode, int key, Object object) {
		if (node == null) {
			Node newNode = new Node();
			newNode.mKey = key;
			newNode.mObject = object;
			newNode.mParentNode = parentNode;
			if (parentNode != null) {
				if (key < parentNode.mKey) {
					parentNode.mLeftNode = newNode;
				} else {
					parentNode.mRightNode = newNode;
				}
			} else {
				mRootNode = newNode;
			}
			return;
		}
		
		if (key < node.mKey) {
			add(node.mLeftNode, node, key, object);
		} else {
			add(node.mRightNode, node, key, object);
		}
	}
	
	public void add(int key, Object object) {
		add(mRootNode, null, key, object);
	}
	
	// Iterative add method.
	public void add2(Node node, int key, Object object) {
		Node prevNode = null;		
		while (node != null) {
			prevNode = node;
			if (key < node.mKey) {
				node = node.mLeftNode;
			} else {
				node = node.mRightNode;
			}
		}
		Node newNode = new Node();
		newNode.mKey = key;
		newNode.mObject = object;
		newNode.mParentNode = prevNode;
		if (prevNode == null) {
			mRootNode = newNode;
		} else {
			if (key < prevNode.mKey) {
				prevNode.mLeftNode = newNode;
			} else {
				prevNode.mRightNode = newNode;
			}
		}
	}
	
	public void add2(int key, Object object) {
		add2(mRootNode, key, object);
	}	
	
	// Recursive search method.
	public Object search(Node node, int key) {
		if(node == null) {
			return null;
		}
		if (node.mKey == key) {
			return node.mObject;
		}
		
		if (key < node.mKey) {
			return search(node.mLeftNode, key);
		} else {
			return search(node.mRightNode, key);
		}
	}
	
	public Object search(int key) {
		return search(mRootNode, key);
	}
	
	// Iterative search method.
	public Object search2(Node node, int key) {
		if(node == null) {
			return null;
		}
		
		while (node != null && node.mKey != key) {
			if (key < node.mKey) {
				node = node.mLeftNode;
			} else {
				node = node.mRightNode;
			}
		}
		if (node != null) {
			return node.mObject;
		} else {
			return null;
		}
	}
	
	public Object search2(int key) {
		return search2(mRootNode, key);
	}
	
	public Node min(Node node) {
		while (node.mLeftNode != null) {
			node = node.mLeftNode;
		}
		return node;
	}
	
	public Node max(Node node) {
		while (node.mRightNode != null) {
			node = node.mRightNode;
		}
		return node;
	}
	
	public Node successor(Node node) {
		if (node.mRightNode != null) {
			return min(node.mRightNode);
		}
		Node parentNode = node.mParentNode;
		while (parentNode != null && parentNode.mRightNode.equals(node)) {
			node = parentNode;
			parentNode = node.mParentNode;
		}
		return parentNode;
	}
	
	// Inorder walk over the tree.
	String printBinarySearchTree(Node node) {
		String string = "";
		if (node != null) {
			string += printBinarySearchTree(node.mLeftNode);
			string += node.mObject + ", ";
			string += printBinarySearchTree(node.mRightNode);
		}
		return string;
	}
	
	public String toString() {
		return printBinarySearchTree(mRootNode);
	}
	
	public static void main(String[] args) {
		BinarySearchTree bst = new BinarySearchTree();
		bst.add(2, "2");
		bst.add2(1, "1");
		bst.add(7, "7");
		bst.add2(9, "9");
		bst.add(5, "5");
		bst.add2(3, "3");
		bst.add(6, "6");
		
		System.out.println(bst.search(9));
		System.out.println(bst.search2(3));
		System.out.println(bst.toString());
	}
}
