
#include <libmyhome_private.h>

myHomePass_t myHomePass(const char *password_s, const char* nonce_s) {
  myHomePass_t m_1 = 0xFFFFFFFFL;
  myHomePass_t m_8 = 0xFFFFFFF8L;
  myHomePass_t m_16 = 0xFFFFFFF0L;
  myHomePass_t m_128 = 0xFFFFFF80L;
  myHomePass_t m_16777216 = 0xFF000000L;
  int flag = 1;
  myHomePass_t num1 = 0L;
  myHomePass_t num2 = 0L;
  myHomePass_t password_l = atoll(password_s);
  char c = 0;
  for (int idx = 0; (c = nonce_s[idx]) != 0; idx++) {
    num1 = (num1 & m_1);
    num2 = (num2 & m_1);
    if (c == '1') {
      if (flag)
        num2 = password_l;
      num1 = num2 & m_128;
      num1 = num1 >> 7;
      num2 = num2 << 25;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '2') {
      if (flag)
        num2 = password_l;
      num1 = num2 & m_16;
      num1 = num1 >> 4;
      num2 = num2 << 28;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '3') {
      if (flag)
        num2 = password_l;
      num1 = num2 & m_8;
      num1 = num1 >> 3;
      num2 = num2 << 29;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '4') {
      if (flag)
        num2 = password_l;
      num1 = num2 << 1;
      num2 = num2 >> 31;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '5') {
      if (flag)
        num2 = password_l;
      num1 = num2 << 5;
      num2 = num2 >> 27;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '6') {
      if (flag)
        num2 = password_l;
      num1 = num2 << 12;
      num2 = num2 >> 20;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '7') {
      if (flag)
        num2 = password_l;
      num1 = num2 & 0xFF00L;
      num1 = num1 + (( num2 & 0xFFL ) << 24 );
      num1 = num1 + (( num2 & 0xFF0000L ) >> 16 );
      num2 = (( num2 & m_16777216 ) >> 8);
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '8') {
      if (flag)
        num2 = password_l;
      num1 = num2 & 0xFFFFL;
      num1 = num1 << 16;
      num1 = num1 + ( num2 >> 24 );
      num2 = num2 & 0xFF0000L;
      num2 = num2 >> 8;
      num1 = num1 + num2;
      flag = 0;
    } else if (c == '9') {
      if (flag)
        num2 = password_l;
      num1 = ~num2;
      flag = 0;
    } else {
      num1 = num2;
    }
    num2 = num1;
  }
  return (num1 & m_1);
}
