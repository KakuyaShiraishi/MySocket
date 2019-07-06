#include "Socket.h"
#include <process.h>
#include <string>
#include <list>

typedef std::list<Socket*> socket_list;

socket_list g_connections;

unsigned __stdcall Connection(void* a) {
  Socket* s = (Socket*) a;

  g_connections.push_back(s);

  s->SendLine("Welcome to the Message Distributor");

  while (1) {
    std::string r = s->ReceiveLine();
    if (r.empty()) break;

    for (socket_list::iterator os =g_connections.begin();
                               os!=g_connections.end(); 
                               os++) {
      if (*os != s) (*os)->SendLine(r);
    }
  }

  g_connections.remove(s);

  delete s;

  return 0;
}

int main() {
  SocketServer in(2000,5);
  
  while (1) {
    Socket* s=in.Accept();
  
    unsigned ret;
    _beginthreadex(0,0,Connection,(void*) s,0,&ret);
  }
  
  return 0;
}
