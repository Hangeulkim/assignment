package 과제;

import java.util.Scanner;

public class 과제3_14 {
	public static void main(String[] args) {
		String course [] = {"Java","C++","HTML5","컴퓨터구조","안드로이드"};
		int score [] = {95,88,76,62,55};
		
		Scanner scanner=new Scanner(System.in);
		int i=0;
		while(true) {
			System.out.print("과목 이름>>");
			String a=scanner.next();
			if(a.equals("그만")) break;
			boolean fflag=false;
			for(i=0;i<course.length;i++) {
				if(course[i].equals(a)) {
					fflag=true;
					break;
				}
			}
			if(fflag) {
				System.out.println(course[i]+"의 점수는 "+score[i]);
			}
			else System.out.println("없는 과목입니다.");
		}
		scanner.close();
	}
}
