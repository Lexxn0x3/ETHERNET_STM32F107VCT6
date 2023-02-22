#include "connectionMan.h"
#include "lwip.h"
#include "lwip/tcp.h"
#include <string.h>

#define LEDTEXT "Starting LED "

static err_t
close_con(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_close(pcb);
}
void Connection_Init(void)
{
  struct tcp_pcb *pcb;
  ip_addr_t server;
  err_t ret_val;
  int bufspace = 0;
  struct pbuf *pb;
  char *string = "What is going on my man!!!!\r\n";


  pb = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_REF);
  pb->payload = string;
  pb->len = pb->tot_len = strlen(string);


  IP4_ADDR(&server, 192,168,178,196);

  pcb = tcp_new();
  tcp_bind(pcb, IP_ADDR_ANY, 61110);

  ret_val = tcp_connect(pcb, &server, 23, NULL);

  if (ret_val == ERR_OK) {
     bufspace = tcp_sndbuf(pcb);
     if (bufspace) {
       do {
          tcp_write(pcb, pb->payload,pb->len, 0);
        } while (ret_val == ERR_MEM);
       tcp_output(pcb);
       tcp_sent(pcb, close_con);
     }
  }
  pbuf_free(pb);

}