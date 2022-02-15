import java.util.Scanner;

public class 과제2_6 {
	public static void main(String[] args) {
		System.out.print("1~99 사이의 정수를입력하시오>>");
		Scanner scanner=new Scanner(System.in);
		
		int a=scanner.nextInt();
		int cnt=0;
		
		if(a>=10 && a/10%3==0) cnt++;
		if(a%10!=0 && a%10%3==0) cnt++;
		
		if(cnt==0) System.out.println(a);
		else if(cnt==1) System.out.println("박수짝");
		else System.out.println("박수짝짝");
			
		scanner.close();
	}
}
