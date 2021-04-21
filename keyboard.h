#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include <STC89C5xRC.H>
#include "defs.h"

#define KB_DATA P1

u8 kb_scan();
u8 kb_get_event(); // press and release
#endif