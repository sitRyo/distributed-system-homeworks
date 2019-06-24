#ifndef _MOUNT_
#define _MOUNT_ 1

#include <cstdint>
#include "param.hpp"

using int_t = uint16_t;

struct Mount {
  int_t m_dev;
  int_t *m_bufp;
  int_t *m_inodp;
};

// static Mount mount[NMOUNT];

#endif // _MOUNT_