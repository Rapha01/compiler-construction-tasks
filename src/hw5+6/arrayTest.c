uint64_t x[3][3][3];
uint64_t c;

uint64_t main() {
  uint64_t y[2][2];

  y[0][0] = 42;
  y[1][0] = 43;
  y[0][1] = 44;

  return y[1][0];
}

//uint64_t (uint64_t* z[2][2][3]) {
  //z[1][1][2] = 52;
  //return z[1][1][2];
//}
