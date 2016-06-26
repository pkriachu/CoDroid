import java.util.Random;

class TestCoverage {
	public static void main(String[] args) {
		int i;
		Random r = new Random(System.currentTimeMillis());

		System.out.println("Coverage Test: 0x12e5 = " + 0x12e5);
		Coverage.init();
		Coverage.value5();
		Coverage.valueE();
		Coverage.value2();
		Coverage.value1();
		System.out.println(Coverage.commit());

		System.out.println("Random tests ...");
		Coverage.clear();
		for(i = 0; i < 10000; i++) {
			int v = r.nextInt(256);
			Coverage.init();
			Coverage.updateValue((v>>0) & 0x0f);
			Coverage.updateValue((v>>4) & 0x0f);
			Coverage.commit();
		}
		Coverage.dump();
		System.out.println("Done.");

		return;
	}
}

