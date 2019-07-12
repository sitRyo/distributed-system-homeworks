#include "header/Logger.hpp"
#include "header/Task.hpp"

using std::cout;
using std::endl;

Logger::Logger() noexcept {
  registerMessage();
}

void Logger::registerMessage() noexcept {
  _logMessageTable[util::REQ_FLAG] = std::string("REQ: ");
  _logMessageTable[util::ACK_FLAG] = std::string("ACK: ");
  _logMessageTable[util::INIT_FLAG] = std::string("INIT: ");
  _logMessageTable[util::TERMINATE_FLAG] = std::string("TERMINATE: ");
  _logMessageTable[util::FUNCTION_EXECTION_FLAG] = std::string("Exection: ");
}

void Logger::showMessage(const short flag, const double time) const noexcept {
  for (auto&& itr : _logMessageTable) {
    if (flag | itr.first) cout << itr.second << " " << time << endl;
  }
}

// specified Task class
void Logger::showData(char *buf) const noexcept {
  Task task = *reinterpret_cast<Task*>(&buf[10]);
  printf(" %d\n", task.priority);
}