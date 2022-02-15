abstract class PairMap{
	protected String KeyArray [];
	protected String valueArray[];
	abstract String get(String key);
	abstract void put(String key, String value);
	
	abstract String delete(String key);
	
	abstract int length();
}

class Dictionary extends PairMap{
	public Dictionary(int size) {
		KeyArray = new String[size];
		valueArray = new String[size];
	}

	@Override
	String get(String key) {
		// TODO Auto-generated method stub
		for(int i=0;i<10;i++) {
			if(key.equals(KeyArray[i])) return valueArray[i];
		}
		return null;
	}

	@Override
	void put(String key, String value) {
		// TODO Auto-generated method stub
		for(int i=0;i<10;i++) {
			if(KeyArray[i]==null) {
				KeyArray[i] = key;
				valueArray[i] = value;
				break;
			}
			else if(key.equals(KeyArray[i])) {
				 valueArray[i] = value;
				 break;
			}
		}
	}

	@Override
	String delete(String key) {
		// TODO Auto-generated method stub
		for(int i=0;i<10;i++) {
			if(key.equals(KeyArray[i])) {
				KeyArray[i] = null;
				valueArray[i] = null;
				break;
			}
		}
		return null;
	}

	@Override
	int length() {
		// TODO Auto-generated method stub
		int cnt=0;
		for(int i=0;i<10;i++) {
			if(KeyArray[i]!=null) {
				cnt++;
			}
		}
		return cnt;
	}
	
}

public class 과제10 {
	public static void main(String[] args) {
		Dictionary dic = new Dictionary(10);
		dic.put("황기태","자바");
		dic.put("이재문", "파이선");
		dic.put("이재문","C++");
		System.out.println("이재문의 값은 "+dic.get("이재문"));
		System.out.println("황기태의 값은 "+dic.get("황기태"));
		dic.delete("황기태");
		System.out.println("황기태의 값은 "+dic.get("황기태"));
	}
}
