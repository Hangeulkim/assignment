
class Point{
	private int x,y;
	public Point(int x, int y) {this.x = x; this.y = y;}
	public int getX() {return x;}
	public int getY() {return y;}
	protected void move(int x, int y) {this.x = x; this.y = y;}
}

class ColorPoint extends Point{
	private String color;
	public ColorPoint(int x, int y, String color) {
		super(x,y);
		this.color = color;
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

public class 과제5 {
	public static void main(String [] args) {
		ColorPoint cp = new ColorPoint(5,5,"YELLOW");
		cp.SetXY(10, 20);
		cp.SetColor("RED");
		String str = cp.toString();
		System.out.println(str+"입니다.");
	}
}
