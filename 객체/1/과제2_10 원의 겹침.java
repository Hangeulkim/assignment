import java.util.Scanner;

public class ����2_10 {
	public static boolean intri(int x1,int y1, int r1,int x2,int y2,int r2) {
		if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)>(r1+r2)*(r1+r2)) return false;
		else return true;
	}
	
	public static void main(String[] args) {
		System.out.print("ù��° ���� �߽ɰ� ������ �Է�>>");
		Scanner scanner=new Scanner(System.in);
		int x1=scanner.nextInt();
		int y1=scanner.nextInt();
		int r1=scanner.nextInt();

		
		System.out.print("�ι�° ���� �߽ɰ� ������ �Է�>>");
		int x2=scanner.nextInt();
		int y2=scanner.nextInt();
		int r2=scanner.nextInt();
		
		
		if(intri(x1,y1,r1,x2,y2,r2)) System.out.println("�� ���� ���� ��ģ��.");
		else System.out.println("�� ���� ���� �Ȱ�ģ��.");
			
		scanner.close();
	}
}
