#include <stdio.h>
#include <common.h>
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>

#define NUM_SMALL_MESSAGES 1000000000
#define NUM_BIG_MESSAGES 100
#define SIZE_BUFFER 1024*1024*50

int main(int argc, char **argv) {
	void *buffer = malloc(SIZE_BUFFER);
	int listen_socket = tcp_listen_on_port(5000);
	assert(listen_socket != -1);
	
	AsyncSocket sock;
	tcp_accept_async(listen_socket, &sock, 1024*1024);

	struct timeval start, end;	
	
	
	fprintf(stderr, "Comenzando pruebas de recibir valores pequeños\n");
	
	uint8_t value;
	gettimeofday(&start, 0);
	for (int i = 0; i < NUM_SMALL_MESSAGES; i++) {
		tcp_message_recv_async(&sock, (void *)&value, sizeof(uint8_t));
	}
	gettimeofday(&end, 0);
	
	fprintf(stderr, "Terminadas pruebas. %f gbps\n",
	((double)NUM_SMALL_MESSAGES*8/1000) / (((double)end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)));
	
	
	
	fprintf(stderr, "Comenzando pruebas de recibir valores grandes\n");
	

	gettimeofday(&start, 0);
	for (int i = 0; i < NUM_BIG_MESSAGES; i++) {
		tcp_message_recv_async(&sock, (void *)buffer, SIZE_BUFFER);
	}
	gettimeofday(&end, 0);
	
	fprintf(stderr, "Terminadas pruebas. %f gbps\n",
	((double)NUM_BIG_MESSAGES*SIZE_BUFFER*8/1000) / (((double)end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec)));

}