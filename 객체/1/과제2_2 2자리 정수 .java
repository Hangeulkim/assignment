import java.util.Scanner;

public class ����2_2 {
	public static void main(String[] args) {
		System.out.print("2�ڸ��� ���� �Է�(10~99)>>");
		Scanner scanner=new Scanner(System.in);
		
		int a=scanner.nextInt();
		if(a%10==a/10) System.out.println("Yes! 10�� �ڸ��� 1�� �ڸ��� �����ϴ�.");
		else System.out.println("No ! 10�� �ڸ��� 1�� �ڸ��� �ٸ��ϴ�.");

		scanner.close();
	}
}
