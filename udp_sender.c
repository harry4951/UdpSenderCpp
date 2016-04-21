/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "GenerateRandomId.h"

///////////////////////////////////
// Constants definition
///////////////////////////////////

#define NUM_SAMPLES 1024
#define BUFFER_SIZE	32
#define N_smallpacket 4

void error(const char *);
int RandomId();

typedef struct 
{
  int packet_id;
  int packet_id_num;
  float buffer[BUFFER_SIZE];
  
}netpacket;
int main(int argc, char *argv[])
{
     int sock;
     unsigned int length;
     struct sockaddr_in server;
     struct hostent *hp;

     // Parse commmand line arguments
     if (argc != 3) { printf("Usage: server port\n");
                      exit(1);
     }

     // Prepare Socket Connection
     sock= socket(AF_INET, SOCK_DGRAM, 0);
     if (sock < 0) error("socket");

     server.sin_family = AF_INET;
     hp = gethostbyname(argv[1]);
     if (hp==0) error("Unknown host");

     bcopy((char *)hp->h_addr, 
          (char *)&server.sin_addr,
           hp->h_length);
     server.sin_port = htons(atoi(argv[2]));
     length=sizeof(struct sockaddr_in);


    // Prepare input wavedata with a linear ramp
    float wavedata[NUM_SAMPLES];
    //int flag_number = 0;
    int value = 0;
	static const float DELTA = 0.1f;

    for(int i=0; i<NUM_SAMPLES; i++) {
          wavedata[i] = DELTA * value;
          value++;
      }

    // Loop over signal in chunks of BUFFER_SIZE and send each buffer to socket
	//Every time send 32 floating number
	
	int N_BUFFERS = NUM_SAMPLES / BUFFER_SIZE/N_smallpacket;
  netpacket netpacket_test;
  for(int big_buf_idx=0; big_buf_idx < N_BUFFERS; big_buf_idx++)
  {
    netpacket_test.packet_id = big_buf_idx;
    //printf("netpacket_test.packet_id %d \n", netpacket_test.packet_id);
    for(int buf_idx=0; buf_idx<N_smallpacket; buf_idx++)
    {

      netpacket_test.packet_id_num = RandomId();
      //netpacket_test.packet_id_num = buf_idx;
      //printf("netpacket_test.packet_id_num %d \n", netpacket_test.packet_id_num);
      memcpy(netpacket_test.buffer, &wavedata[(big_buf_idx * N_smallpacket + netpacket_test.packet_id_num) * BUFFER_SIZE], BUFFER_SIZE * sizeof(float));
      //test timeout function at receiver
      //if(big_buf_idx!=0 && big_buf_idx!=1)
      //{
        if(buf_idx==3)
        {
          usleep(100*1000);  //50ms
        }
      //}
      //for(int i=0; i< BUFFER_SIZE; i++)
      //{
      //  printf("%5.2f\n",netpacket_test.buffer[i]);
      //}
      int sent_bytes = sendto(sock, (const void*) &netpacket_test,
                  sizeof(netpacket_test),0,(const struct sockaddr *)&server,length);
  		if (sent_bytes < 0)
  		{
  		   	error("Sendto");
  		}

  		usleep(1);

    }
  }

}
 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

