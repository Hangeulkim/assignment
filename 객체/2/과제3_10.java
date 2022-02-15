package 과제;

import java.util.Random;

public class 과제3_10 {
	public static void main(String[] args) {
		Random r=new Random();
		int[][] arr=new int[4][4];
		
		for(int i=0;i<arr.length;i++) {
			for(int j=0;j<arr[i].length;j++) {
				arr[i][j]=0;
			}
		}
		
		for(int i=0;i<10;i++) {
			int y=r.nextInt(4);
			int x=r.nextInt(4);
			if(arr[y][x]!=0) {
				i--;
				continue;
			}
			else {
				arr[y][x]=r.nextInt(10)+1;
			}
		}
		for(int i=0;i<arr.length;i++) {
			for(int j=0;j<arr[i].length;j++) {
				System.out.print(arr[i][j]+"\t");
			}
			System.out.println();
		}
	}
}
