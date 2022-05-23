uint64_t x;
uint64_t* y;

struct MY_STRUCT_ONE {
  uint64_t testfield1;
  struct MY_STRUCT_ONE * testfield2;

  struct MY_STRUCT_ONE * testfield3;
  uint64_t testfield4;
};


struct MY_STRUCT_ONE * a;
struct MY_STRUCT_ONE * b;


uint64_t main() {

  a = malloc(80);
  b = malloc(80);
  a->testfield2 = b;
  a->testfield2->testfield4 = 42;
  x = 55;

  return a->testfield2->testfield4;
}
