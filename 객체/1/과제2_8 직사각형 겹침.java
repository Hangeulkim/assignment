import java.util.Scanner;

public class ����2_8 {
	public static boolean inRect(int x, int y, int rectx1, int rectx2,int recty1,int recty2) {
		if((x>=rectx1 && x<=rectx2) && (y>=recty1 && y<=recty2)) return true;
		else return false;
	}

	public static void main(String[] args) {
		System.out.print("�簢���� ù��° ��ǥ�� �Է��Ͻÿ�>>");
		Scanner scanner=new Scanner(System.in);

		int x1=scanner.nextInt();
		int y1=scanner.nextInt();

		System.out.print("�簢���� �ι�° ��ǥ�� �Է��Ͻÿ�>>");
		int x2=scanner.nextInt();
		int y2=scanner.nextInt();


		if(inRect(x1,y1,100,200,100,200)||inRect(x2,y2,100,200,100,200)) System.out.print("�Էµ� �簢���� �簢���� ��Ĩ�ϴ�.");
		else System.out.print("�Էµ� �簢���� �簢���� �Ȱ�Ĩ�ϴ�.");

		scanner.close();
	}
}
