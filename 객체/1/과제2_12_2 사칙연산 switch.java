import java.util.Scanner;

public class ����2_12_2 {
	public static void main(String[] args) {
		System.out.print("����>>");
		Scanner scanner=new Scanner(System.in);
		
		double num1=scanner.nextDouble();
		String calc=scanner.next();
		double num2=scanner.nextDouble();
		
		switch(calc) {
			case "+":
				System.out.println(num1+"+"+num2+"�� ��� ����� "+(num1+num2));
				break;
			case "-":
				System.out.println(num1+"-"+num2+"�� ��� ����� "+(num1-num2));
				break;
			case "*":
				System.out.println(num1+"*"+num2+"�� ��� ����� "+(num1*num2));
				break;
			case "/":
				if(num2!=0) System.out.println(num1+"/"+num2+"�� ��� ����� "+(num1/num2));
				else System.out.println("0���� ���� �� �����ϴ�.");
				break;
		}
		
		scanner.close();
	}
}
