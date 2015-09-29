public class Hashtable {
	private static final int HASHTABLE_SIZE = 16;
	private Object mKeys[] = new Object[HASHTABLE_SIZE];
	private Object mObjects[] = new Object[HASHTABLE_SIZE];
	
	public void add(Object key, Object object) throws Exception {
		int i = 0;
		int index;
		do {
			index = hashCode(key, i);
			if (mKeys[index] == null) {
				mKeys[index] = key;
				mObjects[index] = object;
				return;
			} else {
				i++;
			}			
		} while (i < HASHTABLE_SIZE);	
		throw new Exception("hash table overflow");
	}
	
	public void delete(Object key) {
		int i = 0;
		int index;
		do {
			index = hashCode(key, i);
			if (mKeys[index].equals(key)) {
				mKeys[index] = null;
				mObjects[index] = null;
				return;
			} else {
				i++;
			}
			index = hashCode(key, i);
		} while (i < HASHTABLE_SIZE);
	}
	
	public Object get(Object key) {
		int i = 0;
		int index;
		do {
			index = hashCode(key, i);
			if (mKeys[index].equals(key)) {
				return mObjects[index];
			}
			i++;
		} while (i < HASHTABLE_SIZE);	
		return null;
	}
	
	// Double hashing
	int hashCode(Object key, int i) {
		int k = Math.abs(key.hashCode());
		int hashCode1 = k % 701;
		int hashCode2 = 1 + (k % 700);
		int hastCode = (hashCode1 + i * hashCode2) % 701;
		return hastCode % HASHTABLE_SIZE;
	}
	
	public static void main(String[] args) {
		Hashtable ht = new Hashtable();
		Integer integer = new Integer(123);		
		String string = new String("Hashtable");
		try {
			ht.add("Test", integer);	
			ht.add(2, string);
			ht.add("Hashtable", new String("XYZ"));
		} catch (Exception e) {			
			e.printStackTrace();
		}		
		
		System.out.println(ht.get("Test"));
		System.out.println(ht.get(2));
		System.out.println(ht.get("Hashtable"));	
	}
}
