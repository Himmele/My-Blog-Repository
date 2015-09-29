public class LinkedListExample1 {
	static class Message {
		Message(int id) {
			mId = id;
		}
		public String toString() {
			return String.valueOf(mId);
		}
		private int mId;		
		public Message mNextMessage;
	}
	
	static class MessageQueue {
		public void enqueueMessage(Message message) {		
	        Message curMessage = mHeadMessage;
	        if (curMessage == null) {
	        	message.mNextMessage = curMessage;
	            mHeadMessage = message;
	        } else {
                Message prevMessage = null;
                while (curMessage != null) {
                	prevMessage = curMessage;
                    curMessage = curMessage.mNextMessage;
                }
                message.mNextMessage = prevMessage.mNextMessage;
                prevMessage.mNextMessage = message;                
	        }        
		}
		
		public Message dequeueMessage() {
			Message message = mHeadMessage;
			if (message != null) {
				mHeadMessage = message.mNextMessage;
			}
            return message;
		}
		
		private Message mHeadMessage;
	}
	
	public static void main(String[] args) {
		LinkedList<Integer> ll = new LinkedList<Integer>();
		ll.put(4);
		ll.put(3);
		ll.put(2);
		ll.put(1);
		System.out.println(ll.take());
		System.out.println(ll.take());
		System.out.println(ll.take());
		System.out.println(ll.take());
		System.out.println(ll.take());
		
		MessageQueue mq = new MessageQueue();
		mq.enqueueMessage(new Message(1));
		mq.enqueueMessage(new Message(2));
		mq.enqueueMessage(new Message(3));
		mq.enqueueMessage(new Message(4));
		System.out.println(mq.dequeueMessage());
		System.out.println(mq.dequeueMessage());
		System.out.println(mq.dequeueMessage());
		System.out.println(mq.dequeueMessage());
		System.out.println(mq.dequeueMessage());
	}
}
