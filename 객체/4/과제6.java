
class Point{
	private int x,y;
	public Point(int x, int y) {this.x = x; this.y = y;}
	public int getX() {return x;}
	public int getY() {return y;}
	protected void move(int x, int y) {this.x = x; this.y = y;}
}

class ColorPoint extends Point{
	private String color;
	public ColorPoint() {
		super(0,0);
		this.color = "BLACK";
	}
	public ColorPoint(int x, int y, String color) {
		super(x,y);
		this.color = color;
	}
	public ColorPoint(int x, int y) {
		super(x,y);
		this.color = "BLACK";
	}
	public void SetXY(int x, int y) {
		this.move(x,y);
	}
	public void SetColor(String color) {
		this.color = color;
	}
	public String toString() {
		return this.color+"색의 ("+this.getX()+","+this.getY()+")의 점";
	}
}

public class 과제6 {
	public static void main(String [] args) {
		ColorPoint zeroPoint = new ColorPoint();
		System.out.println(zeroPoint.toString()+"입니다.");
		
		ColorPoint cp = new ColorPoint(10, 10);
		cp.SetXY(5,5);
		cp.SetColor("RED");
		System.out.println(cp.toString()+"입니다.");
	}
}
