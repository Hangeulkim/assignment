
interface Shape{
	final double PI = 3.14;
	void draw();
	double getArea();
	default public void redraw() {
		System.out.print("--- �ٽ� �׸��ϴ�. ");
		draw();
	}
}

class Circle implements Shape{
	private int line;
	Circle(int line){
		this.line = line;
	}
	
	@Override
	public void draw() {
		// TODO Auto-generated method stub
		System.out.println("�������� "+line+"�� ���Դϴ�.");
	}

	@Override
	public double getArea() {
		// TODO Auto-generated method stub
		return PI*line*line;
	}
	
}

public class ����13 {
	public static void main(String[] args) {
		Shape donut = new Circle(10);
		donut.redraw();
		System.out.println("������ "+donut.getArea());
	}
}
