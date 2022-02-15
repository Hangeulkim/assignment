import java.util.Scanner;

class Phone{
	private String name, tel;
	Phone(String name, String tel){
		this.name=name;
		this.tel=tel;
	}
	
	public String get_name() {
		return name;
	}
	
	public void show() {
		System.out.println(name+"의 번호는 "+tel+" 입니다.");
	}
}

public class PhoneBook {
	public static void main(String [] args) {
		Scanner scanner = new Scanner(System.in);
		
		System.out.print("인원수>>");
		int num = scanner.nextInt();
		Phone[] pb = new Phone[num];
		for(int i = 0; i < pb.length; i++) {
			System.out.print("이름과 전화번호(이름과 번호는 빈 칸없이 입력)>>");
			String name = scanner.next();
			String tel = scanner.next();
			pb[i] = new Phone(name, tel);
		}
		System.out.println("저장되었습니다...");
		while(true) {
			System.out.print("검색할 이름>>");
			String search_name = scanner.next();
			if(search_name.equals("그만")) break;
			int i = 0;
			for(i = 0 ; i < pb.length ; i++) {
				if(pb[i].get_name().equals(search_name)) {
					pb[i].show();
					break;
				}
			}
			if(i == pb.length) System.out.println(search_name+" 이 없습니다.");
		}
		scanner.close();
	}
}
