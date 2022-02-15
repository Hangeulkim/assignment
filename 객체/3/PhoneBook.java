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
		System.out.println(name+"�� ��ȣ�� "+tel+" �Դϴ�.");
	}
}

public class PhoneBook {
	public static void main(String [] args) {
		Scanner scanner = new Scanner(System.in);
		
		System.out.print("�ο���>>");
		int num = scanner.nextInt();
		Phone[] pb = new Phone[num];
		for(int i = 0; i < pb.length; i++) {
			System.out.print("�̸��� ��ȭ��ȣ(�̸��� ��ȣ�� �� ĭ���� �Է�)>>");
			String name = scanner.next();
			String tel = scanner.next();
			pb[i] = new Phone(name, tel);
		}
		System.out.println("����Ǿ����ϴ�...");
		while(true) {
			System.out.print("�˻��� �̸�>>");
			String search_name = scanner.next();
			if(search_name.equals("�׸�")) break;
			int i = 0;
			for(i = 0 ; i < pb.length ; i++) {
				if(pb[i].get_name().equals(search_name)) {
					pb[i].show();
					break;
				}
			}
			if(i == pb.length) System.out.println(search_name+" �� �����ϴ�.");
		}
		scanner.close();
	}
}
