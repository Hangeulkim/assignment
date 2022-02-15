package 과제;

import java.util.Random;
import java.util.Scanner;


public class 과제3_16 {
	public static void main(String[] args) {
		Scanner scanner=new Scanner(System.in);
		System.out.println("컴퓨터와 가위 바위 보 게임을 합니다.");
		String str[]= {"가위","바위","보"};
		Random r=new Random();
		
		int i=0;
		int hflag=0;
		while(true) {
			System.out.print("가위 바위 보!>>");
			String tmp=scanner.next();
			if(tmp.equals("그만")) break;
			
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
			System.out.print("사용자 = "+str[i]+" , 컴퓨터 = "+str[n]+" , ");
			switch(hflag) {
				case 0:
					System.out.println("사용자가 이겼습니다.");
					break;
				case 1:
					System.out.println("비겼습니다.");
					break;
				case 2:
					System.out.println("사용자가 졌습니다.");
					break;
			}
		}
		
		
		
		scanner.close();
	}
}
