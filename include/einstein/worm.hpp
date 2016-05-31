#ifndef __EINSTEIN_WORM_H__
#define __EINSTEIN_WORM_H__

#include <common.h>

#include <poll.h>
#include <signal.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <memory>
#include <stdexcept>

using namespace std;

namespace einstein
{
struct Worm {
	WormSetup ws;
	int socket;
	string host;
	string programName;
	bool halting;
	bool deployed;

	Worm(uint16_t id, uint16_t listenPort, int16_t core, string ip, string connectionDescription, string host, string programName);
	~Worm();
};
}

#endif
