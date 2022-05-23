uint64_t x;
uint64_t y;


uint64_t main() {
  x = 10;
  y = 0;

  for (x = 1;x < 12;x = x + 1) {
    y = y + 1;
  }

  return y;
}
