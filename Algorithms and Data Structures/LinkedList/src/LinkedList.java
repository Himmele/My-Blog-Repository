public class LinkedList<T> {
	public void put(T item) {    
	    Node node = new Node(item);
	    Node curNode = mHeadNode;
	    if (curNode == null) {
	    	node.nextNode = curNode;
	        mHeadNode = node;	            
	    } else {
            Node prevNode = null;
            while (curNode != null) {
                prevNode = curNode;
                curNode = curNode.nextNode;
            }
            node.nextNode = prevNode.nextNode;
            prevNode.nextNode = node;            
	    }	    
	}

	public T take() {
        Node node = getHeadNode();
        if (node != null) {
            T item = node.item;                    
            return item;
        } else {
            return null;
        }		
    }
    
    class Node
    {
    	public T item;
        public Node nextNode;
        public Node(T t) { item = t; }
    };

    Node getHeadNode() {
    	Node node = mHeadNode;
        if (node != null) {
        	mHeadNode = node.nextNode;
            return node;
        }
        return null;
    }

    private Node mHeadNode;
}
