
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
		System.out.println(this.getSize()+"��ġ "+this.getColors()+"�÷�");
	}
}

public class ����1 {
	public static void main(String [] args) {
		ColorTV myTV = new ColorTV(32, 1024);
		myTV.printProperty();
	}
}
