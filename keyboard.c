#include "keyboard.h"

u8 kb_last_value = 0;

u8 kb_scan() 
{
  u8 ret = 0;
  KB_DATA = 0xf0;
  switch(KB_DATA) {
    case 0xe0:
      ret = 0;
      break;
    case 0xd0:
      ret = 4;
      break;
    case 0xb0:
      ret = 8;
      break;
    case 0x70:
      ret = 12;
      break;
    default:
      return 0;
  }
  KB_DATA = 0x0f;
  switch(KB_DATA) {
    case 0x07:
      return ret + 1;
    case 0x0b:
      return ret + 2;
    case 0x0d:
      return ret + 3;
    case 0x0e:
      return ret + 4;
  }
  return 0;
}

u8 kb_get_event()
{
  u8 tmp = kb_scan();
  if(kb_last_value) {
    if(tmp) {
      kb_last_value = tmp;
      return 0;
    }else {
      tmp = kb_last_value;
      kb_last_value = 0;
      return tmp;
    }
  }else {
    kb_last_value = tmp;
    return 0;
  }
  return tmp;
}