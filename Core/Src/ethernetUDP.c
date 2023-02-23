#include "ethernetUDP.h"
#include "lwip.h"
#include "usart.h"
#include "gpio.h"
#include "lwip/udp.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct udp_pcb *upcb;
struct pbuf buffer;

void udpClient_send(uint32_t msg)
{
  struct pbuf *txBuf;
  char data[4];
  int len = 0;

  /* allocate pbuf from pool*/
  txBuf = pbuf_alloc(PBUF_TRANSPORT, 4, PBUF_RAM);


  if (txBuf != NULL)
  {
    /* copy data to pbuf */
    pbuf_take(txBuf, &msg, 4);

    /* send udp data */
    udp_send(upcb, txBuf);

    /* free pbuf */
    pbuf_free(txBuf);
  }
}

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	uint32_t val = 0;
	memcpy(&val, p->payload, sizeof(uint32_t));

	LL_GPIO_WriteOutputPort(GPIOD, val);

	pbuf_free(p);
}

void udpClient_connect(void)
{
	err_t err;

	/* 1. Create a new UDP control block  */
	upcb = udp_new();

	/* Bind the block to module's IP and port */
	//ip_addr_t myIPaddr;
	//IP_ADDR4(&myIPaddr, 192, 168, 178, 224);
	udp_bind(upcb, IP_ADDR_ANY, 8);


	/* configure destination IP address and port */
	ip_addr_t DestIPaddr;
	IP_ADDR4(&DestIPaddr, 192,168,178,23);
	err= udp_connect(upcb, &DestIPaddr, 12346);

	if (err == ERR_OK)
	{
		/* 2. Send message to server */
		udpClient_send(NULL);

		/* 3. Set a receive callback for the upcb */
		udp_recv(upcb, udp_receive_callback, NULL);
	}
}

void udpServer_init(void)
{
	// UDP Control Block structure
   struct udp_pcb *upcb;
   err_t err;

   /* 1. Create a new UDP control block  */
   upcb = udp_new();

   /* 2. Bind the upcb to the local port */
   //ip_addr_t myIPADDR;
   //IP_ADDR4(&myIPADDR, 192, 168, 178, 224);

   err = udp_bind(upcb, IP_ADDR_ANY, 12346);  // 7 is the server UDP port


   /* 3. Set a receive callback for the upcb */
   if(err == ERR_OK)
   {
	   udp_recv(upcb, udp_receive_callback, NULL);
   }
   else
   {
	   udp_remove(upcb);
   }
}