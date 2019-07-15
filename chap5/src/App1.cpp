#include <iostream>
#include "header/MiddleWare.hpp"
#include <thread>
#include <chrono>

using std::cout;
using std::endl;

// process A
int main() {
  MiddleWare middleWare(1);
  middleWare.AddNewClient(1, "ClientA", "127.0.0.1", 8888);
  
  middleWare.AddNewClient(2, "ClientB", "127.0.0.1", 8889);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  Task task = {1};
  middleWare.sendData("ClientA", task);
  middleWare.sendData("ClientB", task);
}