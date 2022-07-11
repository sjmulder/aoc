#include <iostream>

int main() {
  /* d is pt.2 depth, a is pt.1 depth AND aim */
  std::string cmd;
  int x, h=0,d=0,a=0;

  while (std::cin >> cmd >> x)
    switch (cmd[0]) {
    case 'd': a+=x; break;
    case 'u': a-=x; break;
    case 'f': h+=x; d+=a*x; break;
    default: std::cerr << "bad input\n"; return 1;
    }

  std::cout << "02: " << h*a << " " << h*d << "\n";
}
