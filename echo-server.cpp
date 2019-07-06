#include "Socket.h"
#include <process.h>
#include <string>

unsigned __stdcall Answer(void* a) {
  Socket* s = (Socket*) a;

  while (1) {
    std::string r = s->ReceiveLine();
    if (r.empty()) break;
    s->SendLine(r);
  }

  delete s;

  return 0;
}

int main(int argc, char* argv[]) {
  SocketServer in(2000,5);

  while (1) {
    Socket* s=in.Accept();

    unsigned ret;
    _beginthreadex(0,0,Answer,(void*) s,0,&ret);
  }
 
  return 0;
}
