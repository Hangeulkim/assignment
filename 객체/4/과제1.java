
class TV{
	private int size;
	public TV(int size) {this.size = size;}
	protected int getSize() {return size;}
}

class ColorTV extends TV{
	private int colors;
	public ColorTV(int size, int colors) {
		super(size);
		this.colors = colors;
	}
	protected int getColors() {return colors;}
	public void printProperty() {
		System.out.println(this.getSize()+"인치 "+this.getColors()+"컬러");
	}
}

public class 과제1 {
	public static void main(String [] args) {
		ColorTV myTV = new ColorTV(32, 1024);
		myTV.printProperty();
	}
}
