package backtype.storm;

import es.hpcn.wormhole.Worm;
import es.hpcn.wormhole.Einstein;

import backtype.storm.generated.StormTopology;

import java.util.Map;
import java.io.PrintWriter;

public class StormSubmitter
{
	public StormSubmitter()
	{

	}

	private static boolean  isEinsteinNode	= false;
	private static String   mainClass 		= null;
	private static String[] mainClassArgs 	= null;

	public static void setEinsteinNode(String mainclassPrm, String[] mainClassArgsPrm)
	{
		isEinsteinNode = true;
		mainClass = mainclassPrm;
		mainClassArgs = mainClassArgsPrm;
	}

	public static void submitTopologyWithProgressBar(String name, Map stormConf, StormTopology topology) throws Exception
	{
		if (isEinsteinNode) {   //Einstein things
			String configFileName = "/tmp/stormEinstein.conf";
			String listenIp = "150.244.58.114";
			int listenPort = 5000;
			boolean autoDeployWorms = true;

			PrintWriter writer = new PrintWriter(configFileName, "UTF-8");

			for (int i = 1; i <= topology.getWHnum(); i++) {
				writer.println(topology.getWHconfig(i));
				writer.println("	" + topology.getWHdesc(i));
			}

			writer.println("");
			writer.close();

			String[]  params = new String[mainClassArgs.length + 1];
			params[0] = mainClass;

			for (int i = 0; i < mainClassArgs.length; i++) {
				params[i + 1] = mainClassArgs[i];
			}

			Einstein eins = new Einstein(configFileName, listenIp, listenPort, autoDeployWorms, params);

		} else { //Worm things
			return;

		}
	}
}