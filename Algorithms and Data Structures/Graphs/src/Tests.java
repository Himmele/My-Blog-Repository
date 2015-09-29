import java.util.Dictionary;
import java.util.LinkedList;
import java.util.ListIterator;

public class Tests {
	public static void test1() {
		Graph graph = new Graph();
		
		Node r = new Node("r");
		Node s = new Node("s");
		Node t = new Node("t");
		Node u = new Node("u");
		Node v = new Node("v");
		Node w = new Node("w");
		Node x = new Node("x");
		Node y = new Node("y");
		
		graph.addNode(r);
		graph.addNode(s);		
		graph.addNode(t);
		graph.addNode(u);
		graph.addNode(v);
		graph.addNode(w);
		graph.addNode(x);
		graph.addNode(y);
		
		// The graph is represented as an adjacency list.
		r.addLink(s);
		r.addLink(v);
		
		s.addLink(r);
		s.addLink(w);
		
		t.addLink(u);
		t.addLink(x);
		t.addLink(w);
		
		u.addLink(t);
		u.addLink(x);
		u.addLink(y);
		
		v.addLink(r);
		
		w.addLink(s);
		w.addLink(t);
		w.addLink(x);
		
		x.addLink(w);
		x.addLink(t);
		x.addLink(y);
		
		y.addLink(x);
		y.addLink(u);
		
		Dictionary<Node, Graph.NodeAttributes> nodeAttributes = graph.breadthFirstSearch(s);
		for (Node node : graph.getNodes()) {
			System.out.println(node.getId().toString() + ": " + nodeAttributes.get(node).distance);
		}
		graph.printPath(v, u);
	}
	
	public static void test2() {
		Graph graph = new Graph();
		
		Node r = new Node("r");
		Node s = new Node("s");
		Node t = new Node("t");
		Node u = new Node("u");
		Node v = new Node("v");
		Node w = new Node("w");
		Node x = new Node("x");
		Node y = new Node("y");
		
		graph.addNode(r);
		graph.addNode(s);		
		graph.addNode(t);
		graph.addNode(u);
		graph.addNode(v);
		graph.addNode(w);
		graph.addNode(x);
		graph.addNode(y);
		
		// The graph is represented as an adjacency list.
		r.addLink(s);
		r.addLink(v);
		
		s.addLink(r);
		s.addLink(w);
		
		t.addLink(u);
		t.addLink(x);
		t.addLink(w);
		
		u.addLink(t);
		u.addLink(x);
		u.addLink(y);
		
		v.addLink(r);
		
		w.addLink(s);
		w.addLink(t);
		w.addLink(x);
		
		x.addLink(w);
		x.addLink(t);
		x.addLink(y);
		
		y.addLink(x);
		y.addLink(u);
		
		Dictionary<Node, Graph.NodeAttributes> nodeAttributes = graph.depthFirstSearch();
		for (Node node : graph.getNodes()) {
			System.out.println(node.getId().toString() + ": " + nodeAttributes.get(node).startTime + "/" + nodeAttributes.get(node).finishTime);
		}
		graph.printPath(v, u);
	}
	
	public static void test3() {
		Graph graph = new Graph();
		
		Node undershorts = new Node("undershorts");
		Node pants = new Node("pants");
		Node belt = new Node("belt");
		Node shirt = new Node("shirt");
		Node tie = new Node("tie");
		Node jacket = new Node("jacket");
		Node socks = new Node("socks");
		Node shoes = new Node("shoes");
		Node watch = new Node("watch");
		
		graph.addNode(shirt);
		graph.addNode(tie);
		graph.addNode(jacket);
		graph.addNode(belt);
		graph.addNode(watch);
		graph.addNode(undershorts);
		graph.addNode(pants);		
		graph.addNode(shoes);
		graph.addNode(socks);
		
		// The acyclic graph is represented as an adjacency list.
		undershorts.addLink(shoes);
		undershorts.addLink(pants);
		
		pants.addLink(shoes);
		pants.addLink(belt);
		
		belt.addLink(jacket);
		
		shirt.addLink(tie);
		shirt.addLink(belt);
		
		tie.addLink(jacket);
		
		socks.addLink(shoes);
		
		LinkedList<Node> topologicalList = graph.topologicalSort();
		for (Node node: topologicalList) {
			System.out.print(node.getId() + " -> ");
		}
	}	
	
	public static void test4() {
		Graph graph = new Graph();
				
		Node s = new Node("s");
		Node t = new Node("t");
		Node x = new Node("x");
		Node y = new Node("y");
		Node z = new Node("z");
				
		graph.addNode(s);		
		graph.addNode(t);
		graph.addNode(x);
		graph.addNode(y);
		graph.addNode(z);
		
		// The graph is represented as an adjacency list.
		s.addLink(t);
		graph.addLinkWeight(s, t, 10);
		s.addLink(y);
		graph.addLinkWeight(s, y, 5);
		
		t.addLink(y);
		graph.addLinkWeight(t, y, 2);
		t.addLink(x);
		graph.addLinkWeight(t, x, 1);
				
		x.addLink(z);
		graph.addLinkWeight(x, z, 4);
				
		y.addLink(t);
		graph.addLinkWeight(y, t, 3);
		y.addLink(x);
		graph.addLinkWeight(y, x, 9);
		y.addLink(z);
		graph.addLinkWeight(y, z, 2);
		
		z.addLink(x);
		graph.addLinkWeight(z, x, 6);
		z.addLink(s);
		graph.addLinkWeight(z, s, 7);
		
		Dictionary<Node, Graph.NodeAttributes> nodeAttributes = graph.dijkstraSingleSourceShortestPath(s);
		Node targetNode = x;
		LinkedList<Node> weightedShortestPath = new LinkedList<Node>();
		while (targetNode != null) {
			Graph.NodeAttributes na = nodeAttributes.get(targetNode);
			weightedShortestPath.addFirst(targetNode);			
			if (targetNode == s) {
				break;
			}
			targetNode = na.predecessor;
		}
		ListIterator<Node> li = weightedShortestPath.listIterator();
		while (li.hasNext()) {
			System.out.print(li.next().getId() + " -> ");			
		}
	}
	
	public static void main(String[] args) {
		System.out.println("Test 1 - Breadth-First-Search:");
		test1();
		System.out.println();
		
		System.out.println("Test 2 - Depth-First-Search:");
		test2();
		System.out.println();
		
		System.out.println("Test 3 - Topological Sort (Dependency Graph):");
		test3();
		System.out.println();	
		
		System.out.println("Test 4 - Dijkstra's Algorithm:");
		test4();
		System.out.println();	
	}
}
