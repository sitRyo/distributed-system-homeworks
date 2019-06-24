#include <iostream>
#include <string>
#include "controller.hpp"

V6Sh::V6Sh( std::string& file_name ) : file_name(file_name) {
  init();
}

void V6Sh::init() {
  analyzer.open_storage_file(file_name);
  analyzer.seek_bootblock();
  analyzer.seek_superblock();
}

void V6Sh::start() {
  int inode_block_number = 1;
  while (1) {
    std::string input;
    std::string command;
    INode inode;
    analyzer.get_inodeblock(inode_block_number, &inode);
    std::vector<name_t> names;
    if (analyzer.isDirectory(inode.i_mode)) {
      names = get_child_directries(inode);
    }

    std::cout << ">> ";
    std::getline(std::cin, input);
    command = get_command(input);
    if (command == "ls") {
      std::string opt = "";
      if (input.size() >= 5) {
        opt = input.substr(3, 5);
      }

      for (auto&& itr : names) { 
        if (opt == "-l") {
          std::cout << analyzer.check_permission(itr.ino);
        }
        std::cout << itr.name << "\n";
      }
    } else if (command == "cd") {
      std::string next_dir = input.substr(3);
      next_dir += '\0';
      for (auto&& itr : names) {
        if (itr.name == next_dir) {
          analyzer.get_inodeblock(itr.ino, &inode);
          if (analyzer.isDirectory(inode.i_mode)) {
            inode_block_number = itr.ino;
          } else {
            std::cout << next_dir << " is not Directory.\n";
          }
          break;
        }
      }
    }
  }
}

std::vector<name_t> V6Sh::get_child_directries( INode& inode ) {
  std::vector<name_t> names = analyzer.seek_storage(inode);
  return names;
}

std::string V6Sh::get_command( std::string& input ) {
  // とりあえず妥協
  return input.substr(0, 2);
}
