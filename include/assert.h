#ifndef ASSERT_H
#define ASSERT_H

#include <stdint.h>

#define assert(cond) if (!(cond)) { kpanic("Assertion failed: " #cond); }

#endif