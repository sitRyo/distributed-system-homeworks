#include <iostream>
#include "header/MiddleWare.hpp"

using std::cout;
using std::endl;

// process B
int main() {
  MiddleWare middleWare(2);
  middleWare.AddNewClient(2, "ClientB", "127.0.0.1", 8889);
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  middleWare.AddNewClient(1, "ClientA", "127.0.0.1", 8888);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  Task task = {2};
  middleWare.sendData("ClientB", task);
  middleWare.sendData("ClientA", task);
}