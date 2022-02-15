
interface Shape{
	final double PI = 3.14;
	void draw();
	double getArea();
	default public void redraw() {
		System.out.print("--- 다시 그립니다. ");
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
		System.out.println("반지름이 "+line+"인 원입니다.");
	}

	@Override
	public double getArea() {
		// TODO Auto-generated method stub
		return PI*line*line;
	}
	
}

public class 과제13 {
	public static void main(String[] args) {
		Shape donut = new Circle(10);
		donut.redraw();
		System.out.println("면적은 "+donut.getArea());
	}
}
