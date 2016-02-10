#include <einstein.hpp>
#include <common.h>

#include <cassert>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

class EinsteinTester: public Einstein
{

 public:
	EinsteinTester();
	EinsteinTester(string configFilePath, string einsteinIp, bool autoDeployWorms);
	~EinsteinTester();
};

EinsteinTester::EinsteinTester() : EinsteinTester("../src/examples/test.conf", "150.244.58.77", true)
{

}

EinsteinTester::EinsteinTester(string configFilePath, string einsteinIp, bool autoDeployWorms) : Einstein(configFilePath, einsteinIp, 5000, autoDeployWorms)
{


	assert(ec.listenIp == inet_addr("150.244.58.77"));
	assert(ec.listenPort == 5000);

	assert(ec.connections.at(1)->ws.id == 1);
	assert(ec.connections.at(1)->ws.listenPort == 10001);
	assert(ec.connections.at(1)->ws.core == -1);
	assert(ec.connections.at(1)->ws.IP == inet_addr("150.244.58.77"));
	assert(ec.connections.at(1)->ws.connectionDescriptionLength == 29);
	assert(!memcmp(ec.connections.at(1)->ws.connectionDescription, "1", 29));

}
EinsteinTester::~EinsteinTester()
{

}



int main(int argc, char **argv)
{
	if (argc != 3) {
		try {
			EinsteinTester einsTester;
			std::cout << "Éxito\n";

		} catch (exception &e) {
			std::cerr << "Excepción: " << e.what() << '\n';
		}

	} else {
		try {
			EinsteinTester einsTester(argv[1], argv[2], false);
			std::cout << "Éxito\n";

		} catch (exception &e) {
			std::cerr << "Excepción: " << e.what() << '\n';
		}
	}
}
