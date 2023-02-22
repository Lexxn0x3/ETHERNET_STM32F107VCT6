#include "connectionMan.h"
#include "lwip.h"
#include "lwip/tcp.h"
#include <string.h>
static struct tcp_pcb *pcb;

static err_t
close_con(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_close(pcb);
}

void TCPClient_Send(char * msg) {
  int bufspace = 0;

  bufspace = tcp_sndbuf(pcb);
  if (bufspace) {
    tcp_write(pcb, msg,strlen(msg), 0);
    tcp_output(pcb);
    //tcp_sent(pcb, close_con);
  }

}

void Connection_Init(void)
{
  ip_addr_t server;
  err_t ret_val;
  int bufspace = 0;
  char *string = "Mattis\r\n\r\n";

  IP4_ADDR(&server, 87,162,57,165);

  pcb = tcp_new();
  tcp_bind(pcb, IP_ADDR_ANY, 61110);

  ret_val = tcp_connect(pcb, &server, 12345, NULL);

  if (ret_val == ERR_OK) {
     bufspace = tcp_sndbuf(pcb);
     if (bufspace) {
       do {
          tcp_write(pcb, string,strlen(string), 0);
        } while (ret_val == ERR_MEM);
       tcp_output(pcb);
       //tcp_sent(pcb, close_con);
     }
  }

  TCPClient_Send("Apfelkuchen\r\n\r\n");

}