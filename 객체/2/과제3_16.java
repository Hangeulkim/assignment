package ����;

import java.util.Random;
import java.util.Scanner;


public class ����3_16 {
	public static void main(String[] args) {
		Scanner scanner=new Scanner(System.in);
		System.out.println("��ǻ�Ϳ� ���� ���� �� ������ �մϴ�.");
		String str[]= {"����","����","��"};
		Random r=new Random();
		
		int i=0;
		int hflag=0;
		while(true) {
			System.out.print("���� ���� ��!>>");
			String tmp=scanner.next();
			if(tmp.equals("�׸�")) break;
			
			int n=r.nextInt(3);
			for(i=0;i<str.length;i++) if(str[i].equals(tmp)) break;
			switch(i) {
				case 0:
					switch(n) {
						case 0:
							hflag=1;
							break;
						case 1:
							hflag=2;
							break;
						case 2:
							hflag=0;
							break;
					}
					break;
				case 1:
					switch(n) {
					case 0:
						hflag=0;
						break;
					case 1:
						hflag=1;
						break;
					case 2:
						hflag=2;
						break;
					}
					break;
				case 2:
					switch(n) {
					case 0:
						hflag=2;
						break;
					case 1:
						hflag=0;
						break;
					case 2:
						hflag=1;
						break;
					}
					break;
			}
			System.out.print("����� = "+str[i]+" , ��ǻ�� = "+str[n]+" , ");
			switch(hflag) {
				case 0:
					System.out.println("����ڰ� �̰���ϴ�.");
					break;
				case 1:
					System.out.println("�����ϴ�.");
					break;
				case 2:
					System.out.println("����ڰ� �����ϴ�.");
					break;
			}
		}
		
		
		
		scanner.close();
	}
}
