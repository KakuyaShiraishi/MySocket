#include "Socket.h"

#include <iostream>
#include <process.h>
#include <sstream>

int         portProxy;
std::string addrServer;
int         portServer;

unsigned __stdcall RunProxyThread (void* a) {
  Socket*      s = (Socket*) a;
  SocketClient c(addrServer, portServer);

  while (1) {
    SocketSelect sel(s, &c);

    bool still_connected = true;

    if (sel.Readable(s)) {
      std::string bytes = s->ReceiveBytes();
      c.SendBytes(bytes);
      std::cout << "Server: " << bytes << std::endl;
      if (bytes.empty()) still_connected=false;
    }
    if (sel.Readable(&c)) {
      std::string bytes = c.ReceiveBytes();
      s->SendBytes(bytes);
      std::cout << "Client: " << bytes << std::endl;
      if (bytes.empty()) still_connected=false;
    }
    if (! still_connected) {
      break;
    }
  }

  delete s;

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc < 4) {
       return -1;
  }

  std::stringstream s; 
  
  s<<argv[1]; s>>portProxy; s.clear();

  addrServer=argv[2];

  s<<argv[3]; s>>portServer;

  SocketServer in(portProxy,5);

  while (1) {
    Socket* s=in.Accept();

    unsigned ret;
    _beginthreadex(0, 0, RunProxyThread,(void*) s,0,&ret);
  }
  
  return 0;
}
