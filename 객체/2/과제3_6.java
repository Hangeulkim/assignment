package 과제;

import java.util.Scanner;

public class 과제3_6 {
	public static void main(String[] args) {
		Scanner scanner=new Scanner(System.in);
		
		System.out.print("금액을 입력하시오>>");
		
		int [] unit= {50000,10000,1000,500,100,50,10,1};
		int how_much=scanner.nextInt();
		for(int i=0;i<unit.length;i++) {
			if(how_much/unit[i]==0) continue;
			System.out.println(unit[i]+"원 짜리 : "+how_much/unit[i]+"개");
			how_much=how_much-unit[i]*(how_much/unit[i]);
		}
		
		scanner.close();
	}
}
