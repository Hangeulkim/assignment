package ����;

import java.util.Scanner;

public class ����3_14 {
	public static void main(String[] args) {
		String course [] = {"Java","C++","HTML5","��ǻ�ͱ���","�ȵ���̵�"};
		int score [] = {95,88,76,62,55};
		
		Scanner scanner=new Scanner(System.in);
		int i=0;
		while(true) {
			System.out.print("���� �̸�>>");
			String a=scanner.next();
			if(a.equals("�׸�")) break;
			boolean fflag=false;
			for(i=0;i<course.length;i++) {
				if(course[i].equals(a)) {
					fflag=true;
					break;
				}
			}
			if(fflag) {
				System.out.println(course[i]+"�� ������ "+score[i]);
			}
			else System.out.println("���� �����Դϴ�.");
		}
		scanner.close();
	}
}
