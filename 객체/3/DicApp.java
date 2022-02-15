import java.util.Scanner;

class Dictionary{
	private static String [] kor = {"사랑", "아기", "돈", "미래", "희망"};
	private static String [] eng = {"love", "baby", "money", "future", "hope"};
	public static String kor2eng(String word) {
		for(int i = 0; i < kor.length ; i++) {
			if(kor[i].equals(word)) return eng[i];
		}
		return null;
	}
}

public class DicApp {
	public static void main(String [] args) {
		Scanner scanner = new Scanner(System.in);
		System.out.println("한영 단어 검색 프로그램입니다.");
		
		while(true) {
			System.out.print("한글 단어?");
			String word = scanner.next();
			if(word.equals("그만")) break;
			String eng = Dictionary.kor2eng(word);
			if(eng == null) System.out.println(word+"는 저의 사전에 없습니다.");
			else {
				System.out.println(word+"은 "+eng);
			}
		}
		scanner.close();
	}
}
