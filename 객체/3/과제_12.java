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

public class 과제_12 {
	public static void main(String [] args) {
		Scanner scanner = new Scanner(System.in);
		
		System.out.println("명품 콘서트홀 예약 시스템입니다.");
		Concert S = new Concert();
		Concert A = new Concert();
		Concert B = new Concert();
		boolean end_flag=false;
		while(true) {
			System.out.print("예약:1, 조회:2, 취소:3, 끝내기:4>>");
			int menu = scanner.nextInt();
			int chair = 0;
			int where = 0;
			String name = "";
			switch(menu) {
			case 1:
				System.out.print("좌석구분 S(1), A(2), B(3)>>");
				chair = scanner.nextInt();
				switch(chair) {
				case 1:
					System.out.print("S>> ");
					S.show();
					System.out.print("이름>>");
					name = scanner.next();
					System.out.print("번호>>");
					where = scanner.nextInt();
					if(where < 1 || where > 10) System.out.println("없는 좌석 입니다.");
					else if(!S.res(name, where)) System.out.println("이미 예약되어 있는 자리 입니다.");
					else System.out.println("예약되었습니다.");
					break;
					
				case 2:
					System.out.print("A>> ");
					A.show();
					System.out.print("이름>>");
					name = scanner.next();
					System.out.print("번호>>");
					where = scanner.nextInt();
					if(where < 1 || where > 10) System.out.println("없는 좌석 입니다.");
					else if(!A.res(name, where)) System.out.println("이미 예약되어 있는 자리 입니다.");
					else System.out.println("예약되었습니다.");
					break;
					
				case 3:
					System.out.print("B>> ");
					B.show();
					System.out.print("이름>>");
					name = scanner.next();
					System.out.print("번호>>");
					where = scanner.nextInt();
					if(where < 1 || where > 10) System.out.println("없는 좌석 입니다.");
					else if(!B.res(name, where)) System.out.println("이미 예약되어 있는 자리 입니다.");
					else System.out.println("예약되었습니다.");
					
					break;
					
				default:
					System.out.println("없는 좌석 입니다.");
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
				System.out.println("<<<조회를 완료하였습니다.>>>");
				break;
			case 3:
				System.out.print("좌석 S:1, A:2, B:3>>");
				chair = scanner.nextInt();
				switch(chair) {
				case 1:
					System.out.print("S>> ");
					S.show();
					System.out.print("이름>>");
					name = scanner.next();
					if(!S.cancel_res(name)) System.out.println("없는 사람이거나 좌석이 비어있습니다.");
					else System.out.println("취소되었습니다.");
					break;
					
				case 2:
					System.out.print("A>> ");
					A.show();
					System.out.print("이름>>");
					name = scanner.next();
					if(!A.cancel_res(name)) System.out.println("없는 사람이거나 좌석이 비어있습니다.");
					else System.out.println("취소되었습니다.");
					break;
					
				case 3:
					System.out.print("B>> ");
					B.show();
					System.out.print("이름>>");
					name = scanner.next();
					if(!B.cancel_res(name)) System.out.println("없는 사람이거나 좌석이 비어있습니다.");
					else System.out.println("취소되었습니다.");
					break;
					
					
				default:
					System.out.println("없는 좌석 입니다.");
					break;
				}
				break;
			case 4:
				end_flag=true;
				break;
			default:
				System.out.println("다른 메뉴를 입력해주십시오.\n\n");
				break;
			}
			if(end_flag) break;
		}
		scanner.close();
	}
}
