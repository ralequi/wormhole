#pragma once

#include <common.h>
#include <zeus/zeus.hpp>

#include <poll.h>
#include <signal.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <vector>

using namespace std;

namespace zeus
{
class ShellCommand
{
	friend class ZeusShell;

   public:
	// variables
	static shared_ptr<Zeus> zeus;
	string cmd;
	function<int(string)> exec;

	string shortHelp;
	string longHelp;
	string hints;

	// set<ShellCommand> subCommands;

	// methods
	ShellCommand(string cmd, function<int(string)> exec, string shortHelp, string longHelp);

	ShellCommand(string cmd, function<int(string)> exec, string shortHelp, string longHelp, string hits);

	~ShellCommand();

	bool operator<(const ShellCommand &rhs) const;

	int execute(string cmd);

	// static methods
	static set<ShellCommand> getCommandList();

	// Commands for methods
	static int cmdHelp(string cmd);
	static int cmdHalt(string cmd);
	static int cmdQuit(string cmd);
	static int cmdList(string cmd);
	static int cmdPing(string cmd);
	static int cmdKill(string cmd);
	static int cmdChRoute(string cmd);
	static int cmdVersion(string cmd);
	static int cmdStatistics(string cmd);

   private:
	ShellCommand(string cmd);
	static int forHole(string cmd, function<int(shared_ptr<Hole>, string)> fn);
	static string normalize(string str);
	static string normalize(string str, int length);
};
}