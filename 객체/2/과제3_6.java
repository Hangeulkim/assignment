package ����;

import java.util.Scanner;

public class ����3_6 {
	public static void main(String[] args) {
		Scanner scanner=new Scanner(System.in);
		
		System.out.print("�ݾ��� �Է��Ͻÿ�>>");
		
		int [] unit= {50000,10000,1000,500,100,50,10,1};
		int how_much=scanner.nextInt();
		for(int i=0;i<unit.length;i++) {
			if(how_much/unit[i]==0) continue;
			System.out.println(unit[i]+"�� ¥�� : "+how_much/unit[i]+"��");
			how_much=how_much-unit[i]*(how_much/unit[i]);
		}
		
		scanner.close();
	}
}
