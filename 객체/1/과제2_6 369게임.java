import java.util.Scanner;

public class ����2_6 {
	public static void main(String[] args) {
		System.out.print("1~99 ������ �������Է��Ͻÿ�>>");
		Scanner scanner=new Scanner(System.in);
		
		int a=scanner.nextInt();
		int cnt=0;
		
		if(a>=10 && a/10%3==0) cnt++;
		if(a%10!=0 && a%10%3==0) cnt++;
		
		if(cnt==0) System.out.println(a);
		else if(cnt==1) System.out.println("�ڼ�¦");
		else System.out.println("�ڼ�¦¦");
			
		scanner.close();
	}
}
