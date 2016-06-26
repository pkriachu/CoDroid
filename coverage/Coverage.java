import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.Random;
import java.util.Hashtable;
import java.util.Set;
import java.lang.Long;

class Coverage {
	private static int	mask = 0;
	private static long	value = 0;
	private static Lock	mutex = null;
	private static Hashtable<Long,Long> table = null;
	private static Long	key = null;
	private static Long	one = null;
	private static Random	random = null;

	public static void realinit() {
		if(mutex == null) {
			mutex = new ReentrantLock();
			table = new Hashtable<Long, Long>();
			key = new Long(0);
			one = new Long(1);
			random = new Random(System.currentTimeMillis());
			if(mutex == null || table == null || key == null || one == null || random == null) {
				System.out.println("initialization failed.");
			}
		}
	}

	public static void init() {
		realinit();
		mutex.lock();
		mask = 0;
		value = 0;
	}

	public static long commit() {
		long ret;
		Long i = null;
		key = value;
		ret = value;
		if((i = table.get(key)) != null) {
			table.put(key, i + 1);
		} else {
			table.put(key, one);
		}
		mutex.unlock();
		return ret;
	}

	public static long commit(long v) {
		Long i = null;
		mutex.lock();
		key = v;
		if((i = table.get(key)) != null) {
			table.put(key, i + 1);
		} else {
			table.put(key, one);
		}
		mutex.unlock();
		return v;
	}

	public static void updateValue(long v) {
		long x = v << mask;
		value |= x;
		mask += 4;
	}

	public static void clear() {
		table.clear();
	}

	public static void dump() {
		Set<Long> keys = table.keySet();
		long count = 0;
		for(Long i: keys) {
			System.out.println("" + i + ": " + table.get(i));
			count += table.get(i);
		}
		System.out.println("Count: " + count);
		return;
	}

	public static void value0() { updateValue(0x00); }
	public static void value1() { updateValue(0x01); }
	public static void value2() { updateValue(0x02); }
	public static void value3() { updateValue(0x03); }
	public static void value4() { updateValue(0x04); }
	public static void value5() { updateValue(0x05); }
	public static void value6() { updateValue(0x06); }
	public static void value7() { updateValue(0x07); }
	public static void value8() { updateValue(0x08); }
	public static void value9() { updateValue(0x09); }
	public static void valueA() { updateValue(0x0a); }
	public static void valueB() { updateValue(0x0b); }
	public static void valueC() { updateValue(0x0c); }
	public static void valueD() { updateValue(0x0d); }
	public static void valueE() { updateValue(0x0e); }
	public static void valueF() { updateValue(0x0f); }

	public static void valueI0() { init(); updateValue(0x00); }
	public static void valueI1() { init(); updateValue(0x01); }
	public static void valueI2() { init(); updateValue(0x02); }
	public static void valueI3() { init(); updateValue(0x03); }
	public static void valueI4() { init(); updateValue(0x04); }
	public static void valueI5() { init(); updateValue(0x05); }
	public static void valueI6() { init(); updateValue(0x06); }
	public static void valueI7() { init(); updateValue(0x07); }
	public static void valueI8() { init(); updateValue(0x08); }
	public static void valueI9() { init(); updateValue(0x09); }
	public static void valueIA() { init(); updateValue(0x0a); }
	public static void valueIB() { init(); updateValue(0x0b); }
	public static void valueIC() { init(); updateValue(0x0c); }
	public static void valueID() { init(); updateValue(0x0d); }
	public static void valueIE() { init(); updateValue(0x0e); }
	public static void valueIF() { init(); updateValue(0x0f); }

	public static void value0C() { updateValue(0x00); commit(); }
	public static void value1C() { updateValue(0x01); commit(); }
	public static void value2C() { updateValue(0x02); commit(); }
	public static void value3C() { updateValue(0x03); commit(); }
	public static void value4C() { updateValue(0x04); commit(); }
	public static void value5C() { updateValue(0x05); commit(); }
	public static void value6C() { updateValue(0x06); commit(); }
	public static void value7C() { updateValue(0x07); commit(); }
	public static void value8C() { updateValue(0x08); commit(); }
	public static void value9C() { updateValue(0x09); commit(); }
	public static void valueAC() { updateValue(0x0a); commit(); }
	public static void valueBC() { updateValue(0x0b); commit(); }
	public static void valueCC() { updateValue(0x0c); commit(); }
	public static void valueDC() { updateValue(0x0d); commit(); }
	public static void valueEC() { updateValue(0x0e); commit(); }
	public static void valueFC() { updateValue(0x0f); commit(); }

	public static void valueI0C() { commit(0x00); }
	public static void valueI1C() { commit(0x01); }
	public static void valueI2C() { commit(0x02); }
	public static void valueI3C() { commit(0x03); }
	public static void valueI4C() { commit(0x04); }
	public static void valueI5C() { commit(0x05); }
	public static void valueI6C() { commit(0x06); }
	public static void valueI7C() { commit(0x07); }
	public static void valueI8C() { commit(0x08); }
	public static void valueI9C() { commit(0x09); }
	public static void valueIAC() { commit(0x0a); }
	public static void valueIBC() { commit(0x0b); }
	public static void valueICC() { commit(0x0c); }
	public static void valueIDC() { commit(0x0d); }
	public static void valueIEC() { commit(0x0e); }
	public static void valueIFC() { commit(0x0f); }
}

