import java.util.Scanner;

class Concert{
	private boolean[] cant_res = new boolean[10];
	private String[] name = new String[10];
	
	public boolean res(String name, int where) {
		if(cant_res[where-1]) return false;
		this.name[where-1] = name;
		this.cant_res[where-1] = true;
		return true;
	}
	
	public boolean cancel_res(String name) {
		for(int i = 0 ; i < cant_res.length ; i++) {
			if(name.equals(this.name[i])) {
				cant_res[i] = false;
				this.name[i] = "";
				return true;
			}
		}
		return false;
	}
	
	public void show() {
		for(int i = 0 ; i < cant_res.length ; i++) {
			if(cant_res[i]) System.out.print(name[i]+" ");
			else System.out.print("--- ");
		}
		System.out.println();
	}
	
}

public class ����_12 {
	public static void main(String [] args) {
		Scanner scanner = new Scanner(System.in);
		
		System.out.println("��ǰ �ܼ�ƮȦ ���� �ý����Դϴ�.");
		Concert S = new Concert();
		Concert A = new Concert();
		Concert B = new Concert();
		boolean end_flag=false;
		while(true) {
			System.out.print("����:1, ��ȸ:2, ���:3, ������:4>>");
			int menu = scanner.nextInt();
			int chair = 0;
			int where = 0;
			String name = "";
			switch(menu) {
			case 1:
				System.out.print("�¼����� S(1), A(2), B(3)>>");
				chair = scanner.nextInt();
				switch(chair) {
				case 1:
					System.out.print("S>> ");
					S.show();
					System.out.print("�̸�>>");
					name = scanner.next();
					System.out.print("��ȣ>>");
					where = scanner.nextInt();
					if(where < 1 || where > 10) System.out.println("���� �¼� �Դϴ�.");
					else if(!S.res(name, where)) System.out.println("�̹� ����Ǿ� �ִ� �ڸ� �Դϴ�.");
					else System.out.println("����Ǿ����ϴ�.");
					break;
					
				case 2:
					System.out.print("A>> ");
					A.show();
					System.out.print("�̸�>>");
					name = scanner.next();
					System.out.print("��ȣ>>");
					where = scanner.nextInt();
					if(where < 1 || where > 10) System.out.println("���� �¼� �Դϴ�.");
					else if(!A.res(name, where)) System.out.println("�̹� ����Ǿ� �ִ� �ڸ� �Դϴ�.");
					else System.out.println("����Ǿ����ϴ�.");
					break;
					
				case 3:
					System.out.print("B>> ");
					B.show();
					System.out.print("�̸�>>");
					name = scanner.next();
					System.out.print("��ȣ>>");
					where = scanner.nextInt();
					if(where < 1 || where > 10) System.out.println("���� �¼� �Դϴ�.");
					else if(!B.res(name, where)) System.out.println("�̹� ����Ǿ� �ִ� �ڸ� �Դϴ�.");
					else System.out.println("����Ǿ����ϴ�.");
					
					break;
					
				default:
					System.out.println("���� �¼� �Դϴ�.");
					break;
				}
				break;
			case 2:
				System.out.print("S>> ");
				S.show();
				System.out.print("A>> ");
				A.show();
				System.out.print("B>> ");
				B.show();
				System.out.println("<<<��ȸ�� �Ϸ��Ͽ����ϴ�.>>>");
				break;
			case 3:
				System.out.print("�¼� S:1, A:2, B:3>>");
				chair = scanner.nextInt();
				switch(chair) {
				case 1:
					System.out.print("S>> ");
					S.show();
					System.out.print("�̸�>>");
					name = scanner.next();
					if(!S.cancel_res(name)) System.out.println("���� ����̰ų� �¼��� ����ֽ��ϴ�.");
					else System.out.println("��ҵǾ����ϴ�.");
					break;
					
				case 2:
					System.out.print("A>> ");
					A.show();
					System.out.print("�̸�>>");
					name = scanner.next();
					if(!A.cancel_res(name)) System.out.println("���� ����̰ų� �¼��� ����ֽ��ϴ�.");
					else System.out.println("��ҵǾ����ϴ�.");
					break;
					
				case 3:
					System.out.print("B>> ");
					B.show();
					System.out.print("�̸�>>");
					name = scanner.next();
					if(!B.cancel_res(name)) System.out.println("���� ����̰ų� �¼��� ����ֽ��ϴ�.");
					else System.out.println("��ҵǾ����ϴ�.");
					break;
					
					
				default:
					System.out.println("���� �¼� �Դϴ�.");
					break;
				}
				break;
			case 4:
				end_flag=true;
				break;
			default:
				System.out.println("�ٸ� �޴��� �Է����ֽʽÿ�.\n\n");
				break;
			}
			if(end_flag) break;
		}
		scanner.close();
	}
}
