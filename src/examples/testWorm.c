/* Copyright (c) 2015-2018 Rafael Leira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <common.h>
#include <worm.h>
#include <worm_private.h>

#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

extern Worm2EinsConn WH_einsConn;
extern uint16_t WH_myId;
extern HoleSetup WH_mySetup;

#define TESTLIST "1"

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	int st = WH_init();
	assert(st == 0);

	// assert(WH_myId == 1);

	assert(WH_einsConn.Port == 5000);
	// assert(WH_einsConn.IP == inet_addr("127.0.0.1"));

	// assert(WH_mySetup.id == 1);
	assert(WH_mySetup.listenPort == 10000 + WH_myId);
	assert(WH_mySetup.core == -1);
	assert(WH_mySetup.IP == inet_addr("127.0.0.1"));
	assert(WH_mySetup.connectionDescriptionLength == strlen(TESTLIST));
	assert(!memcmp(WH_mySetup.connectionDescription, TESTLIST, strlen(TESTLIST)));
	fprintf(stderr, "Éxito setup\n");

	HoleSetup otherWorm;

	for (int i = 0; i < 1; i++) {
		int id = (i % 3) + 1;
		WH_getWormData(&otherWorm, id);
		assert(otherWorm.id == id);
		assert(otherWorm.listenPort == 10000 + id);
		assert(otherWorm.core == -1);
		assert(otherWorm.IP == inet_addr("127.0.0.1"));
	}

	fprintf(stderr, "Éxito getWormData\n");
}
