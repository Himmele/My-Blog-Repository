import java.util.LinkedList;

public class Node {
	private Object mId;
	private LinkedList<Node> mLinks = new LinkedList<Node>();
	
	public Node(Object id) {
		mId = id;
	}
	
	public Object getId() {
		return mId;
	}
	
	public Iterable<Node> getLinks() {
		return mLinks;
	}
	
	public void addLink(Node node) {
		if (!mLinks.contains(node)) {
			mLinks.add(node);			
		}
	}
	
	public void removeLink(Node node) {
		mLinks.remove(node);		
	}
	
	public int hashCode() {
		int hashCode = 17;
		hashCode = hashCode * 31 + mId.hashCode();		
		return hashCode;
	}
	
	public boolean equals(Object o) {
		if (this == o) return true;
		if (o == null || getClass() != o.getClass()) return false;
		Node node = (Node) o;		
		if (mId != node.mId) return false;
		return true;
	}
}
