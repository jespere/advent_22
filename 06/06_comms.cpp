#include <cstdio>
#include <vector>
#include <set>
#include <string>

using namespace std;

class Comms {
  //private:
public:
  Comms() {}
  int find_first(FILE *fp, int num) {
    string lf;
    for (int offset = 0;char c = getc(fp); offset++) {
      if(!isalpha(c)) {
	break;
      } else {
	if(lf.size()< num) {
	  lf.push_back(c);
	} else {
	  lf[offset % num] = c;
	}
      }
      set<char> sss (lf.cbegin(), lf.cend());
      if(sss.size() == num)
	return offset;
    }
    return 0;
  }
};


int main(int argc, char *argv[]) {
  Comms c;
  if(FILE *fp = fopen(argv[1], "r")) {
    int pos = c.find_first(fp, 4);
    printf("Offset: %d\n", pos + 1);
    rewind(fp);
    pos = c.find_first(fp, 14);
    printf("Offset: %d\n", pos + 1);
  }
}
