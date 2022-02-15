	
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
	
	class IPTV extends ColorTV{
		private String ip;
		public IPTV(String ip, int size, int colors) {
			super(size,colors);
			this.ip = ip;
		}
		protected String getIP() {return ip;}
		public void printProperty() {
			System.out.println("���� IPTV�� "+this.getIP()+" �ּ��� "+this.getSize()+"��ġ "+this.getColors()+"�÷�");
		}
	}
	public class ����2 {
		public static void main(String [] args) {
			IPTV iptv = new IPTV("192.1.1.2", 32, 2048);
			
			iptv.printProperty();
		}
	}
