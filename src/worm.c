#include <common.h>
#include <worm.h>
#include <worm_private.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>


/*
*Global variables
*/
Worm2EinsConn WH_einsConn;
uint16_t WH_myId;
WormSetup WH_mySetup;

DestinationWorms WH_myDstWorms = {0};
/*
*===============
*/

uint8_t WH_init(void)
{
	WH_myDstWorms.numberOfWorms = 0;
	WH_myDstWorms.worms = malloc(sizeof(DestinationWorm));

	WH_myId = atoi(getenv("WORM_ID"));

	WH_einsConn.Port = atoi(getenv("EINSTEIN_PORT"));
	WH_einsConn.IP = inet_addr(getenv("EINSTEIN_IP"));

	WH_einsConn.socket = tcp_connect_to(getenv("EINSTEIN_IP"), WH_einsConn.Port);

	if (WH_einsConn.socket == -1) {
		return 1;
	}

	// Fill hello message with worm id
	size_t hellomsgSize = sizeof(enum ctrlMsgType) + sizeof(uint16_t);
	uint8_t hellomsg[hellomsgSize];
	enum ctrlMsgType *msgType = (enum ctrlMsgType *)hellomsg;
	*msgType = HELLOEINSTEIN;
	* ((uint16_t *)(hellomsg + sizeof(enum ctrlMsgType))) = htons(WH_myId);

	// Send hello message
	if (tcp_message_send(WH_einsConn.socket, hellomsg, hellomsgSize) != 0) {
		return 1;
	}

	// Receive WormSetup
	uint8_t *wormSetupMsg = (uint8_t *) &WH_mySetup;

	if (tcp_message_recv(WH_einsConn.socket, wormSetupMsg, sizeof(WormSetup)) != 0) {
		return 1;
	}

	// Receive connectionDescription
	WH_mySetup.connectionDescription = malloc(WH_mySetup.connectionDescriptionLength);

	if (tcp_message_recv(WH_einsConn.socket, WH_mySetup.connectionDescription, WH_mySetup.connectionDescriptionLength) != 0) {
		return 1;
	}

	// TODO: Lanzar hilo de recepción de conexiones

	return 0;
}

uint8_t WH_getWormData(WormSetup *ws, const uint16_t wormId)
{
	enum ctrlMsgType ctrlMsg = QUERYID;

	if (tcp_message_send(WH_einsConn.socket, (uint8_t *) &ctrlMsg, sizeof(enum ctrlMsgType)) != 0) {
		return 1;
	}

	if (tcp_message_send(WH_einsConn.socket, (void *) &wormId, sizeof(uint16_t)) != 0) {
		return 1;
	}

	if (tcp_message_recv(WH_einsConn.socket, (uint8_t *) &ctrlMsg, sizeof(enum ctrlMsgType)) != 0) {
		return 1;
	}

	if (ctrlMsg != CTRL_OK) {
		return 1;
	}

	if (tcp_message_recv(WH_einsConn.socket, (uint8_t *) ws, sizeof(WormSetup)) != 0) {
		return 1;
	}

	return 0;
}

/************************************************************
	Dynamic Routing Library
*************************************************************/
#define _DYM_ROUTE_DEBUG_
/***************************************/
extern uint8_t _binary_obj_structures_h_start;
extern uint8_t _binary_obj_structures_h_end;

const char *_WH_DymRoute_CC_includes = "\n"
									   "#include<stdint.h>\n"
									   "#include<stdio.h>\n";
const char *_WH_DymRoute_CC_FuncStart = "\n\n"
										"uint8_t WH_DymRoute_precompiled_route (const void *const data, const MessageInfo *const mi, DestinationWorms *const cns)\n{\n"
										"int ret = 0;\n"
										"DestinationWorm *dw;\n";
const char *_WH_DymRoute_CC_FuncEnd = "\n"
									  "return ret;"
									  "}\n";
const char *_WH_DymRoute_CC_send =    "ret += WH_DymRoute_send(data, mi, dw);\n";
const char *_WH_DymRoute_CC_setDw =   "dw = cns->worms+%d;\n";

//RR special
const char *_WH_DymRoute_CC_RRswitch   =    "static uint16_t rr%d=0;\n"
		"switch(rr%d){\n";
const char *_WH_DymRoute_CC_RRcase     =    "case %d :\n";
const char *_WH_DymRoute_CC_RRbreak    =    "break;\n";
const char *_WH_DymRoute_CC_RRend      =    "default:\n"
		"return -1;"
		"}\n"
		"rr%d=(rr%d+1) %% %d;\n";

void *_WH_DymRoute_libHandle;
/***************************************/

/* Name WH_DymRoute_precompiled_route
 * Enrute a message
 * Return the number of msgs sent
 */
uint8_t (*WH_DymRoute_precompiled_route)(const void *const data, const MessageInfo *const mi, DestinationWorms *const cns) = 0;

/* Name WH_send
 * TODO
 * Params:
 * Return 0 if OK, something else if error.
 */
uint8_t WH_send(const void *const data, const MessageInfo *const mi)
{
	return !WH_DymRoute_route(data, mi, &WH_myDstWorms);
}
/* Name WH_DymRoute_route
* Enrute a message
* Return the number of msgs sent
*/
uint8_t WH_DymRoute_route(const void *const data, const MessageInfo *const mi, DestinationWorms *wms)
{
	if (WH_DymRoute_precompiled_route == 0) {
		return 0;

	} else {
		return WH_DymRoute_precompiled_route(data, mi, &WH_myDstWorms);
	}
}

/* Name WH_DymRoute_init
 * Starts the Dynamic Routing Library, and setups connection configuration.
 * Also makes connections
 * Return 0 if OK, something else if error.
 */
uint8_t WH_DymRoute_init(const uint8_t *const routeDescription, DestinationWorms *wms)
{
	pid_t myPid = getpid();
	char *tmpString = malloc(1024);
	char *errorString;

	if (!tmpString) {
		return -1;
	}

	sprintf(tmpString, "/tmp/%d.c", myPid);
	FILE *f = fopen(tmpString, "w+");

	if (!f) {
		free(tmpString);
		return -2;
	}

	/*Write headers to the file*/
	fwrite(_WH_DymRoute_CC_includes, strlen(_WH_DymRoute_CC_includes), 1, f);
	fwrite(&_binary_obj_structures_h_start, &_binary_obj_structures_h_end - &_binary_obj_structures_h_start, 1, f);
	fwrite(_WH_DymRoute_CC_FuncStart, strlen(_WH_DymRoute_CC_FuncStart), 1, f);

	int ret = WH_DymRoute_route_create(f, routeDescription, wms);
	fwrite(_WH_DymRoute_CC_FuncEnd, strlen(_WH_DymRoute_CC_FuncEnd), 1, f);
	fclose(f);

	/*Compile the .c*/
	if (!ret) {
		sprintf(tmpString, "gcc /tmp/%d.c -o /tmp/%d.so -shared -fPIC ", myPid, myPid);
		ret = system(tmpString);
	}

	/*Link the .SO*/
	if (!ret) {
		sprintf(tmpString, "/tmp/%d.so", myPid);
		_WH_DymRoute_libHandle = dlopen(tmpString, RTLD_NOW);

		if (!_WH_DymRoute_libHandle) {
			free(tmpString);
			return 5;
		}

		WH_DymRoute_precompiled_route = dlsym(_WH_DymRoute_libHandle, "WH_DymRoute_precompiled_route");

		if ((errorString = dlerror()) != NULL)  {
			fputs(errorString, stderr);
			ret = 6;
		}
	}

	free(tmpString);
	return ret;
}

/* Name WH_DymRoute_send
 * Sends a message to the network
 * Return 0 if OK, something else if error.
  */
uint8_t WH_DymRoute_send(const void *const data, const MessageInfo *const mi, const DestinationWorm *const dw)
{
	//TODO search info
	//fprintf(stderr, "Sending %d bytes to %d\n", mi->size, cn->ip);
	return -1;
}

/* Name WH_DymRoute_route_create
 * Starts the Dynamic Routing Library, and setups connection configuration.
 * Return 0 if OK, something else if error.
 */
uint8_t WH_DymRoute_route_create(FILE *f, const uint8_t *const routeDescription, DestinationWorms *wms)
{
	uint8_t ret = 0;
	uint16_t parentesys = 0;
	uint16_t i = 0;
	uint16_t nextNode = 0;

	while (routeDescription[i] != '\0') {
		if (routeDescription[i] == ')') {
			parentesys--;

		} else if (routeDescription[i] == '(') {
			parentesys++;

			if (parentesys == 1) {
				fprintf(stderr, "PROCESSING: |%s|\n", routeDescription + i);
				ret += WH_DymRoute_route_createFunc(f, routeDescription + i, wms);
			}

		} else if (parentesys == 0 && (routeDescription[i] >= '0' && routeDescription[i] <= '9')) {
			nextNode = atoi((char *)(routeDescription + i));

			while (routeDescription[i] >= '0' && routeDescription[i] <= '9') {
				i++;
			}

			DestinationWorm *worm = WH_addWorm(wms, nextNode);

			if (worm) {
				fprintf(f, _WH_DymRoute_CC_setDw, WH_findWormIndex(wms, nextNode));
				fprintf(f, _WH_DymRoute_CC_send);

#ifdef _DYM_ROUTE_DEBUG_
				fprintf(stderr, "ROUTEDEBUG: Sending to %d\n", nextNode);
#endif

			} else {
				return 77;    //some random error code
			}

			i--;
		}

		i++;
	}


	return ret;
}

/* Name WH_DymRoute_route_createFunc
 * Searchs for a Function, and calls the correct function.
 * Return 0 if OK, something else if error.
 */
uint8_t WH_DymRoute_route_createFunc(FILE *f, const uint8_t *const routeDescription, DestinationWorms *wms)
{
	uint16_t i = 1;

	if (routeDescription[0] != '(') {
#ifdef _DYM_ROUTE_DEBUG_
		fprintf(stderr, "ROUTEDEBUG: Route CreateFunc, unexpected call");
#endif
		return WH_DymRoute_route_create(f, routeDescription + i, wms);
	}

	switch (routeDescription[i]) {
	case 'd':
	case 'D':
		// DUP Function
		return WH_DymRoute_route_create(f, routeDescription + i, wms);

	case 'r':
	case 'R':
		// Round Robin Function
		return WH_DymRoute_route_createFuncRR(f, routeDescription + i, wms);

	default:
#ifdef _DYM_ROUTE_DEBUG_
		fprintf(stderr, "ROUTEDEBUG: Unexpected routing function.");
#endif
		return -1;
	}
}

/* Name WH_DymRoute_route_createFuncRR
 * Adds a "c code" for round robin.
 * Return 0 if OK, something else if error.
 * Used Constants:
	_WH_DymRoute_CC_RRswitch %rrid %rrid
	_WH_DymRoute_CC_RRcase %idcase
	_WH_DymRoute_CC_RRbreak
	_WH_DymRoute_CC_RRend %rrid %rrid %ntotal
 */
uint8_t WH_DymRoute_route_createFuncRR(FILE *f, const uint8_t *const routeDescription, DestinationWorms *wms)
{
	uint8_t ret = 0;
	uint16_t parentesys = 0;
	uint16_t i = 0;
	uint16_t nextNode = 0;

	static uint16_t rrCallId = 0;
	uint16_t myrrCallId = rrCallId++;
	uint16_t rrCaseId = 0;

	fprintf(f, _WH_DymRoute_CC_RRswitch, myrrCallId, myrrCallId);

	while (routeDescription[i] != '\0') {
		if (routeDescription[i] == ')') {
			parentesys--;

		} else if (routeDescription[i] == '(') {

			if (parentesys == 0) {
				fprintf(f, _WH_DymRoute_CC_RRcase, rrCaseId);

				ret += WH_DymRoute_route_createFunc(f, routeDescription + i, wms);

				rrCaseId++;
				fprintf(f, _WH_DymRoute_CC_RRbreak);
			}

			parentesys++;

		} else if (parentesys == 0 && (routeDescription[i] >= '0' && routeDescription[i] <= '9')) {
			nextNode = atoi((char *)(routeDescription + i));

			while (routeDescription[i] >= '0' && routeDescription[i] <= '9') {
				i++;
			}

			DestinationWorm *worm = WH_addWorm(wms, nextNode);

			if (worm) {

				fprintf(f, _WH_DymRoute_CC_RRcase, rrCaseId);
				//RR
				fprintf(f, _WH_DymRoute_CC_setDw, WH_findWormIndex(wms, nextNode));
				fprintf(f, _WH_DymRoute_CC_send);

#ifdef _DYM_ROUTE_DEBUG_
				fprintf(stderr, "ROUTEDEBUG: %d in RR list\n", nextNode);
#endif

				rrCaseId++;
				fprintf(f, _WH_DymRoute_CC_RRbreak);

			} else {
				return 77;    //some random error code
			}

			i--;
		}

		i++;
	}

	fprintf(f, _WH_DymRoute_CC_RRend, myrrCallId, myrrCallId, rrCaseId);

	return ret;
}


/* Name WH_findWorm
 * Return the worm mached (if no exists)
 */
DestinationWorm *WH_findWorm(DestinationWorms *wms, const uint16_t wormId)
{
	for (size_t i = 0; i < wms->numberOfWorms; i++) {
		if (wms->worms[i].id == wormId) {
			return wms->worms + i;
		}
	}

	return NULL;
}


/* Name WH_findWormIndex
	* Return the worm index in DestinationWorms
	*/
size_t WH_findWormIndex(DestinationWorms *wms, const uint16_t wormId)
{
	for (size_t i = 0; i < wms->numberOfWorms; i++) {
		if (wms->worms[i].id == wormId) {
			return i;
		}
	}

	return 0;
}


/* Name WH_addWorm
	* Return the created connection
	*/
DestinationWorm *WH_addWorm(DestinationWorms *wms, const uint16_t wormId)
{
	DestinationWorm *worm = NULL;
	worm = WH_findWorm(wms, wormId);

	if (worm) {
		return worm;
	}

	wms->numberOfWorms++;
	wms->worms = realloc(wms->worms, sizeof(DestinationWorm) * wms->numberOfWorms);

	worm = calloc(sizeof(DestinationWorm), 1); //TODO REVISAR
	worm->id = wormId;

	WormSetup wSetup;

	if (WH_getWormData(&wSetup, wormId)) {
#ifdef _DYM_ROUTE_DEBUG_
		fprintf(stderr, "ROUTEaddworm: Worm %d no data retrived\n", wormId);
#endif
		return NULL;
	}

	worm->conns.port = wSetup.listenPort;
	worm->conns.ip = wSetup.IP;

	//TODO: Obtener información de los tipos disponibles.


	wms->worms[wms->numberOfWorms - 1] = *worm;
	return wms->worms + (wms->numberOfWorms - 1);
}

/************************************************************/