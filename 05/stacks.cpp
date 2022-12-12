#include <cstdio>
#include <vector>
#include <string>
#include <list>
#include <sstream>
#include <iostream>


using namespace std;

class Stacks {
public:
  Stacks(): m_lines(vector<string> ()), m_stacks(vector<list<char> >()) {
  }
  void read_input() {
    string line;

    // Read lines
    int num_parsed = 0;
    while(getline(cin, line)) {
      // Detect end
      // Max 9 stacks supported
      int stacks[9];
      num_parsed = sscanf(line.c_str(), "%d %d %d %d %d %d %d %d %d",
			      &stacks[0],
			      &stacks[1],
			      &stacks[2],
			      &stacks[3],
			      &stacks[4],
			      &stacks[5],
			      &stacks[6],
			      &stacks[7],
			      &stacks[8]);
      if(num_parsed >= 3)
	break;
      if(!line.empty() && line.back() == '\n')
	line.pop_back();
      m_lines.push_back(line);
    }
    for(int i = 0; i < num_parsed; i++) {
      
    }
  }
private:
  vector<string> m_lines;
  vector<list<char> > m_stacks;
};


int main() {
  Stacks s;
  return 0;
}
