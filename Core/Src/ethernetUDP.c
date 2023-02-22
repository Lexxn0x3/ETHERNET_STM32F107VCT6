#include "ethernetUDP.h"
#include "lwip.h"
#include "usart.h"
#include "gpio.h"
#include "lwip/udp.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>

struct udp_pcb *upcb;
int counter = 0;
struct pbuf buffer;

void udpClient_send(char * msg)
{
  struct pbuf *txBuf;
  char data[100];
  int len = 0;

	if (msg != NULL)
	{
		len = sprintf(data, "PIN: %s", msg);
	}
	else
	{
		len = sprintf(data, "sending UDP client message %d", counter);
	}

  /* allocate pbuf from pool*/
  txBuf = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

  if (txBuf != NULL)
  {
    /* copy data to pbuf */
    pbuf_take(txBuf, data, len);

    /* send udp data */
    udp_send(upcb, txBuf);

    /* free pbuf */
    pbuf_free(txBuf);

	counter++;
  }
}

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	/* Copy the data from the pbuf */
	//strncpy (&buffer, (char *)p->payload, p->len);

	/*increment message count */
	counter++;

	udpClient_send("working");

	/* Free receive pbuf */
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
	IP_ADDR4(&DestIPaddr, 192, 168, 178, 23);
	err= udp_connect(upcb, &DestIPaddr, 12345);

	if (err == ERR_OK)
	{
		/* 2. Send message to server */
		udpClient_send(NULL);

		/* 3. Set a receive callback for the upcb */
		udp_recv(upcb, udp_receive_callback, NULL);
	}
}