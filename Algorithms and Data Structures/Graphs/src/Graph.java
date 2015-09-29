import java.util.Comparator;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.ArrayDeque;
import java.util.ArrayList;

public class Graph {	
	public static class NodeAttributes {
		public static final int WHITE = 0;
		public static final int GRAY = 1;
		public static final int BLACK = 2;
		
		public int color;
		public int distance;
		public Node predecessor;
		public int startTime;
		public int finishTime;
	}
	
	private ArrayList<Node> mNodes = new ArrayList<Node>();
	private int mTime = 0;
	LinkedList<Node> mTopologicalOrdering = new LinkedList<Node>();
	
	public void addNode(Node node) {
		mNodes.add(node);
	}
	
	public Iterable<Node> getNodes() {
		return mNodes;
	}
	
	public void printPath(Node fromNode, Node toNode) {
		Dictionary<Node, NodeAttributes> nodeAttributes = breadthFirstSearch(fromNode);		
		if (fromNode.equals(toNode)) {
			System.out.print(fromNode.getId().toString() + " -> ");
		} else {
			if (nodeAttributes.get(toNode).predecessor == null) {				
				System.out.println("No path from " + fromNode.getId() + " to " + toNode.getId());
			} else {
				printPath(fromNode, nodeAttributes.get(toNode).predecessor);				
				System.out.print(toNode.getId() + " -> ");
			}
		}		
	}
	
	
	// O(V + E) or O(Nodes + Links)
	public Dictionary<Node, NodeAttributes> breadthFirstSearch(Node s) {
		Hashtable<Node, NodeAttributes> nodeAttributes = new Hashtable<Node, NodeAttributes>();
		
		for (Node u : mNodes) {			
			NodeAttributes attributes = new NodeAttributes();
			attributes.color = NodeAttributes.WHITE;
			attributes.distance = Integer.MAX_VALUE;
			attributes.predecessor = null;
			nodeAttributes.put(u, attributes);
		}
		NodeAttributes sAttributes = nodeAttributes.get(s);
		sAttributes.color = NodeAttributes.GRAY;
		sAttributes.distance = 0;
		sAttributes.predecessor = null;
		Queue<Node> queue = new ArrayDeque<Node>();
		queue.add(s);
		while (!queue.isEmpty()) {
			Node u = queue.poll();
			NodeAttributes uAttributes = nodeAttributes.get(u);
			for (Node v : u.getLinks()) {
				NodeAttributes vAttributes = nodeAttributes.get(v);
				if (vAttributes.color == NodeAttributes.WHITE) {
					vAttributes.color = NodeAttributes.GRAY;
					vAttributes.distance = uAttributes.distance + 1;
					vAttributes.predecessor = u;
					queue.add(v);
				}
			}
			uAttributes.color = NodeAttributes.BLACK;
		}
		
		return nodeAttributes;
	}
	
	
	// O(V + E) or O(Nodes + Links)
	public Dictionary<Node, NodeAttributes> depthFirstSearch() {
		Hashtable<Node, NodeAttributes> nodeAttributes = new Hashtable<Node, NodeAttributes>();
		
		for (Node u : mNodes) {			
			NodeAttributes attributes = new NodeAttributes();
			attributes.color = NodeAttributes.WHITE;
			attributes.predecessor = null;
			nodeAttributes.put(u, attributes);
		}
		mTime = 0;
		for (Node u : mNodes) {
			if (nodeAttributes.get(u).color == NodeAttributes.WHITE) {
				dfsVisit(u, nodeAttributes);
			}
		}
		return nodeAttributes;
	}
	
	private void dfsVisit(Node u, Dictionary<Node, NodeAttributes> nodeAttributes) {
		NodeAttributes uAttributes = nodeAttributes.get(u);
		uAttributes.color = NodeAttributes.GRAY;	
		mTime++;
		uAttributes.startTime = mTime;
		
		for (Node v : u.getLinks()) {
			NodeAttributes vAttributes = nodeAttributes.get(v);
			if (vAttributes.color == NodeAttributes.WHITE) {
				vAttributes.predecessor = u;
				dfsVisit(v, nodeAttributes);
			}
		}
		
		uAttributes.color = NodeAttributes.BLACK;
		mTime++;
		uAttributes.finishTime = mTime;
		mTopologicalOrdering.addFirst(u);
	}
	
	
	public LinkedList<Node> topologicalSort() {
		depthFirstSearch();		
		return mTopologicalOrdering;
	}
	
	
	private Hashtable<Pair<Node, Node>, Integer> mLinkWeights = new Hashtable<Pair<Node, Node>, Integer>();
	
	public class Pair<A, B> {
	    private A first;
	    private B second;

	    public Pair(A first, B second) {
	        super();
	        this.first = first;
	        this.second = second;
	    }

	    public int hashCode() {
	        int hashFirst = first != null ? first.hashCode() : 0;
	        int hashSecond = second != null ? second.hashCode() : 0;

	        return (hashFirst + hashSecond) * hashSecond + hashFirst;
	    }

	    public boolean equals(Object other) {
	        if (other instanceof Pair) {
	                Pair otherPair = (Pair) other;
	                return 
	                ((  this.first == otherPair.first ||
	                        ( this.first != null && otherPair.first != null &&
	                          this.first.equals(otherPair.first))) &&
	                 (      this.second == otherPair.second ||
	                        ( this.second != null && otherPair.second != null &&
	                          this.second.equals(otherPair.second))) );
	        }

	        return false;
	    }

	    public String toString()
	    { 
	           return "(" + first + ", " + second + ")"; 
	    }

	    public A getFirst() {
	        return first;
	    }

	    public void setFirst(A first) {
	        this.first = first;
	    }

	    public B getSecond() {
	        return second;
	    }

	    public void setSecond(B second) {
	        this.second = second;
	    }
	}
	
	public void addLinkWeight(Node fromNode, Node toNode, int weight) {
		mLinkWeights.put(new Pair<Node, Node>(fromNode, toNode), weight);
	}
	
	class NodeComparator implements Comparator<Node>
	{
		private Dictionary<Node, NodeAttributes> mNodeAttributes;
		
		public NodeComparator(Dictionary<Node, NodeAttributes> nodeAttributes) {
			mNodeAttributes = nodeAttributes;
		}
		
	    @Override
	    public int compare(Node x, Node y) {
	    	NodeAttributes xAttributes = mNodeAttributes.get(x);
	    	NodeAttributes yAttributes = mNodeAttributes.get(y);
	    	
	        if (xAttributes.distance < yAttributes.distance) {
	            return -1;
	        }
	        if (xAttributes.distance > yAttributes.distance) {
	            return 1;
	        }
	        return 0;
	    }
	}

	public Dictionary<Node, NodeAttributes> dijkstraSingleSourceShortestPath(Node s) {
		Hashtable<Node, NodeAttributes> nodeAttributes = new Hashtable<Node, NodeAttributes>();
		
		for (Node u : mNodes) {			
			NodeAttributes attributes = new NodeAttributes();
			if (u != s) {
				attributes.distance = Integer.MAX_VALUE;
			} else {
				attributes.distance = 0;
			}
			attributes.predecessor = null;
			nodeAttributes.put(u, attributes);
		}
		
		Comparator<Node> comparator = new NodeComparator(nodeAttributes);
		PriorityQueue<Node> queue = new PriorityQueue<Node>(10, comparator); // Min-Priority-Queue
		for (Node node : mNodes) {
			queue.add(node);
		}		
		while (!queue.isEmpty()) {
			Node u = queue.poll();
			NodeAttributes uAttributes = nodeAttributes.get(u);
			if (uAttributes.distance == Integer.MAX_VALUE) {
				break;
			}			
			for (Node v : u.getLinks()) {
				NodeAttributes vAttributes = nodeAttributes.get(v);
				int smallestDistance = uAttributes.distance + distanceBetween(u, v);
			    if (smallestDistance < vAttributes.distance) {
			    	vAttributes.distance = smallestDistance ;
			    	vAttributes.predecessor = u;
			    	// FIXME: use decreaseKey instead of remove + offer.
			    	// Or just add only the starting node at the beginning and offer node v here.
			    	queue.remove(v);
			        queue.offer(v);
			    }				
			}
		}
		
		return nodeAttributes;
	}
	
	int distanceBetween(Node fromNode, Node toNode) {
		Integer weight = mLinkWeights.get(new Pair<Node, Node>(fromNode, toNode));
		return (weight != null) ? weight : Integer.MAX_VALUE;
	}
}
