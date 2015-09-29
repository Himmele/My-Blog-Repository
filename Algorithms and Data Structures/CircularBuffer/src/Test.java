public class Test {	
	public static void main(String[] args) {
		final CircularBuffer<Integer> circularBuffer = new CircularBuffer<Integer>(4);
		
		Thread thread1 = new Thread(new Runnable() {
			@Override
			public void run() {
				for (int i = 0; i < 10; i++) {
					circularBuffer.offer(i);
					try {
						Thread.sleep(1000);
					} catch (InterruptedException e) {						
					}
				}				
			}
		});
		thread1.start();
		
		Thread thread2 = new Thread(new Runnable() {
			@Override
			public void run() {
				for (int i = 0; i < 10; i++) {
					int element = circularBuffer.peek();
					System.out.println(element);
					element = circularBuffer.poll();
					System.out.println(element);
				}
			}
		});
		thread2.start();
	}
}
