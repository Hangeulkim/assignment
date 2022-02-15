package 과제;

import java.util.Scanner;

public class 과제3_4 {
	public static void main(String[] args) {
		Scanner scanner=new Scanner(System.in);
		
		System.out.print("소문자 알파벳 하나를 입력하시오>>");
		char c=scanner.next().charAt(0);
		for(char i=c;i>='a';i--) {
			for(char j='a';j<=i;j++) {
				System.out.print(j);
			}
			System.out.println();
		}
		scanner.close();
	}
}
