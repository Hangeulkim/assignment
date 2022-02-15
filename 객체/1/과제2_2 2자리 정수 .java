import java.util.Scanner;

public class 과제2_2 {
	public static void main(String[] args) {
		System.out.print("2자리수 정수 입력(10~99)>>");
		Scanner scanner=new Scanner(System.in);
		
		int a=scanner.nextInt();
		if(a%10==a/10) System.out.println("Yes! 10의 자리와 1의 자리가 같습니다.");
		else System.out.println("No ! 10의 자리와 1의 자리가 다릅니다.");

		scanner.close();
	}
}
