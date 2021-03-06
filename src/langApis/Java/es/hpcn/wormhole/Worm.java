package es.hpcn.wormhole;

import static java.nio.charset.StandardCharsets.UTF_8;

public class Worm
{
	static
	{
		System.loadLibrary("javaworm");
	}

	private native int init();

	private byte [] barray;
	public Worm() throws Exception
	{
		if (init() != 0) {
			throw new Exception("Failed to initialize JavaWorm library");
		}

		barray = new byte[1048576 * 2];
	}

	public native int halt();

	public native int recv(byte[] data);
	public native int send(byte[] data);

	@Deprecated
	public native int recv(String data);

	public native String recv();
	public native int send(String data);

	public native void flushIO();

	public void dispose() //TODO check if ok
	{
		halt();
	}

	/*************/
	public native int getId();
}