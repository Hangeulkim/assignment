import java.util.Scanner;

abstract class Shape{
	private Shape next;
	public Shape() {
		next = null;
	}
	public void setNext(Shape obj) {
		next = obj;
	}
	public Shape getNext() {
		return next;
	}
	public abstract void draw();
}

class Line extends Shape{

	@Override
	public void draw() {
		// TODO Auto-generated method stub
		System.out.println("Line");
	}


}

class Rect extends Shape{

	@Override
	public void draw() {
		// TODO Auto-generated method stub
		System.out.println("Rect");
	}
	
}

class Circle extends Shape{

	@Override
	public void draw() {
		// TODO Auto-generated method stub
		System.out.println("Circle");
		
	}
	
}

public class ����12 {
	public static void main(String [] args) {
		System.out.println("�׷��� ������ beauty�� �����մϴ�.");
		Scanner sc = new Scanner(System.in);
		Line head = new Line();
		while(true) {
			System.out.print("����(1), ����(2), ��� ����(3), ����(4)>>");
			int menu = sc.nextInt();
			if(menu == 1) {
				System.out.print("Line(1), Rect(2), Circle(3)>>");
				int chnum = sc.nextInt();
				Shape p = (Shape)head;
				while(p.getNext()!=null) {
					p = p.getNext();
				}
				switch(chnum) {
				case 1:
					p.setNext(new Line());
					break;
				case 2:
					p.setNext(new Rect());
					break;
				case 3:
					p.setNext(new Circle());
					break;
				}
			}
			else if(menu == 2) {
				System.out.print("������ ������ ��ġ>>");
				int delnum = sc.nextInt();
				Shape p = (Shape)head;
				int cnt = 0;
				while(p.getNext()!=null) {
					cnt++;
					if(delnum==cnt) {
						p.setNext(p.getNext().getNext());
						break;
					}
					p = p.getNext();
				}
				if(delnum>cnt) System.out.println("������ �� �����ϴ�.");
			}
			else if(menu == 3) {
				Shape p = (Shape)head;
				while(p.getNext()!=null) {
					p.getNext().draw();
					p = p.getNext();
				}
			}
			else if(menu == 4) break;
		}
		System.out.println("beauty�� �����մϴ�.");
		sc.close();
	}
}
