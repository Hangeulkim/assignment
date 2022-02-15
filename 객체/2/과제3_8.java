package 과제;

import java.util.Scanner;
import java.util.Random;

public class 과제3_8 {
	public static void main(String[] args) {
		Scanner scanner=new Scanner(System.in);
		
		System.out.print("정수 몇개?>>");
		int num=scanner.nextInt();
		Random r=new Random();
		int[] arr=new int[num];
		boolean rflag=false;
		for(int i=0;i<num;i++) {
			rflag=false;
			int tmp=r.nextInt(100)+1;
			for(int j=0;j<i;j++) {
				if(arr[j]==tmp) {
					tmp=r.nextInt(100)+1;
					i--;
					rflag=true;
					break;
				}
			}
			if(rflag) i--;
			else arr[i]=tmp;
		}
		
		for(int i=0;i<arr.length;i++) {
			if(i%10==0) System.out.println();
			System.out.print(arr[i]+" ");
		}
		
		scanner.close();
	}
}
