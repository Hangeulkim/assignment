import java.util.Scanner;

public class ����2_12_1 {
	public static void main(String[] args) {
		System.out.print("����>>");
		Scanner scanner=new Scanner(System.in);
		
		double num1=scanner.nextDouble();
		String calc=scanner.next();
		double num2=scanner.nextDouble();
		
		if(calc.equals("+")) System.out.println(num1+"+"+num2+"�� ��� ����� "+(num1+num2));
		else if(calc.equals("-")) System.out.println(num1+"-"+num2+"�� ��� ����� "+(num1-num2));
		else if(calc.equals("*")) System.out.println(num1+"*"+num2+"�� ��� ����� "+(num1*num2));
		else if(num2==0) System.out.println("0���� ���� �� �����ϴ�.");
		else System.out.println(num1+"/"+num2+"�� ��� ����� "+(num1/num2));
		
		
		scanner.close();
	}
}
