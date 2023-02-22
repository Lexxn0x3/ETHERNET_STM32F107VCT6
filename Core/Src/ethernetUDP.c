#include "ethernetUDP.h"
#include "lwip.h"
#include "usart.h"
#include "gpio.h"
#include "lwip/udp.h"
#include "stdbool.h"
#include <stdio.h>
#include <string.h>

void sendUDP()
{
	struct pbuf *txBuf;

	/* Get the IP of the Client */
	//char *remoteIP = ipaddr_ntoa(addr);

	char buf[100];

	int len = sprintf (buf,"Hello From UDP SERVER oooooooooooooooooo:XC\n");

	/* allocate pbuf from RAM*/
	txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);

	/* copy the data into the buffer  */
	pbuf_take(txBuf, buf, len);

	/* Connect to the remote client */
	ip4_addr_t ipaddr;
	uint8_t IP_ADDRESS[4];
	IP_ADDRESS[0] = 192;
	IP_ADDRESS[1] = 168;
	IP_ADDRESS[2] = 178;
	IP_ADDRESS[3] = 23;

	struct udp_pcb upcb;

	IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
	udp_connect(&upcb, &ipaddr, 555);

	/* Send a Reply to the Client */
	udp_send(&upcb, txBuf);

	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(&upcb);

	/* Free the p_tx buffer */
	pbuf_free(txBuf);

	/* Free the p buffer */
	//pbuf_free(p);
}

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	struct pbuf *txBuf;

	/* Get the IP of the Client */
	//char *remoteIP = ipaddr_ntoa(addr);

	char buf[100];

	char msg[] = "Hello From UDP SERVER\n";

	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15))
	{
		char msgNew[] = "Ping 15 Pressed";
		strcpy(msg, msgNew);
	}

	int len = sprintf (buf,msg);

	/* allocate pbuf from RAM*/
	txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);

	/* copy the data into the buffer  */
	pbuf_take(txBuf, buf, len);

	/* Connect to the remote client */
	udp_connect(upcb, addr, port);

	/* Send a Reply to the Client */
	udp_send(upcb, txBuf);

	/* free the UDP connection, so we can accept new clients */
	udp_disconnect(upcb);

	/* Free the p_tx buffer */
	pbuf_free(txBuf);

	/* Free the p buffer */
	pbuf_free(p);
}

void udpServer_init(void)
{
	// UDP Control Block structure
   struct udp_pcb *upcb;
   err_t err;

   /* 1. Create a new UDP control block  */
   upcb = udp_new();

   /* 2. Bind the upcb to the local port */
   ip_addr_t myIPADDR;
   IP_ADDR4(&myIPADDR, 192, 168, 178, 224);

   err = udp_bind(upcb, &myIPADDR, 7);  // 7 is the server UDP port


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