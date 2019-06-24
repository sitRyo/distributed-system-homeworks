#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "controller.hpp"

int main( int argc, char* argv[] ) {
  std::string file = argv[argc-1];
  V6Sh shell(file);
  shell.start(); 
}