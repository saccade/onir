#pragma once

inline int absv(int n) {
  if (n >= 0) return n;
  return -n;
}

static inline int mod(int modulus, int value) {
  int ret = value % modulus;
  if (ret < 0) { ret += modulus; }
  return ret;
}

inline char hex_digit(int value) {
  if ((value < 0) or (value > 16)) return '-';
  if (value < 10) return '0' + value;
  return 'A' + value - 10;
}
