class Access{
public:
  long unsigned address;
  int size;
  char type;
  Access(): address(0), type(' '){};
  Access(long unsigned a, char t, int s): size(s), address(a), type(t){};
};
