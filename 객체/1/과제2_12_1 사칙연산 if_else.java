import java.util.Scanner;

public class 과제2_12_1 {
	public static void main(String[] args) {
		System.out.print("연산>>");
		Scanner scanner=new Scanner(System.in);
		
		double num1=scanner.nextDouble();
		String calc=scanner.next();
		double num2=scanner.nextDouble();
		
		if(calc.equals("+")) System.out.println(num1+"+"+num2+"의 계산 결과는 "+(num1+num2));
		else if(calc.equals("-")) System.out.println(num1+"-"+num2+"의 계산 결과는 "+(num1-num2));
		else if(calc.equals("*")) System.out.println(num1+"*"+num2+"의 계산 결과는 "+(num1*num2));
		else if(num2==0) System.out.println("0으로 나눌 수 없습니다.");
		else System.out.println(num1+"/"+num2+"의 계산 결과는 "+(num1/num2));
		
		
		scanner.close();
	}
}
