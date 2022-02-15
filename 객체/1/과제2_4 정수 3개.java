import java.util.Scanner;

public class 과제2_4 {
	public static void main(String[] args) {
		System.out.print("정수 3개 입력>>");
		Scanner scanner=new Scanner(System.in);
		
		int[] a=new int[5];
		
		for(int i=1;i<=3;i++) {
			a[i]=scanner.nextInt();
		}
		
		for(int i=1;i<=2;i++) {
			for(int j=i+1;j<=3;j++) {
				if(a[i]<a[j]) {
					int tmp=a[i];
					a[i]=a[j];
					a[j]=tmp;
				}
			}
		}
		
		System.out.println(a[2]);
		
		scanner.close();
	}
}
