#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <string>


int debug = true;

using namespace std;

class RucksackReorg {
private:
  int score(const char &c) const {
    if(c >= 'a' && c <= 'z') {
      return c - 'a' + 1;
    } else if(c >= 'A' && c <= 'Z') {
      return c - 'A' + 27;
    } else {
      // Error
      fprintf(stderr, "%c is unsupported for score", c);
      return 0;
    }
  }
  vector<string> m_rucksacks;
  int rucksack_priority(const string &r) const {
    set<char> first_half;
    for(int i = 0; i < r.size() / 2; i++) {
      first_half.insert(r[i]);
    }
    for(int j=r.size()/2; j < r.size(); j++) {
      set<char>::const_iterator cit = first_half.find(r[j]);
      if(cit != first_half.end()) {
	return score(*cit);
      }
    }
    fprintf(stderr, "No priority for %s\n", r.c_str());
    return 0;
  }
public:
  RucksackReorg(): m_rucksacks(vector<string>()) {
  }

  void read_input() {
    char line[1024];
    while(fgets(line, sizeof line, stdin)) {
      int len = strlen(line);
      if(len && line[len -1] == '\n') {
	line[len - 1] = 0;
	int newlen = strlen(line);
	if(newlen % 2 != 0)
	  fprintf(stderr, "Uneven line length: %d line: %s\n", len, line);
	m_rucksacks.push_back(line);
      } else {
	fprintf(stderr, "Malformed line: \"%s\"", line);
      }
    }
  }

  int tot_rucksacks_priority()  const {
    int tot_pri = 0;
    for(const auto &r : m_rucksacks) {
      tot_pri += rucksack_priority(r);
    }
    return tot_pri;
  }

  // Make private
  int three_group_priority(const string &r1, const string &r2, const string &r3) const {
    set<char> r1_s(r1.begin(), r1.end());
    set<char> r1_r2_inter;

    for(const char &c : r2) {
      set<char>::const_iterator r1_it = r1_s.find(c);
      if(r1_it != r1_s.end()) {
	r1_r2_inter.insert(c);
      }
    }
    for(const char &c : r3) {
      set<char>::const_iterator r1_r2_inter_it = r1_r2_inter.find(c);
      if(r1_r2_inter_it != r1_r2_inter.end())
	return score(c);
    }

    fprintf(stderr, "No score found for: \n%s\n%s\n%s\n", r1.c_str(), r2.c_str(), r3.c_str());
    return 0;
  }
    
  int tot_group_priority() const {
    int tot_group_priority = 0;
    int cur_group = 0;
    while(m_rucksacks.size() - cur_group >= 3) {
      tot_group_priority += three_group_priority(m_rucksacks[cur_group],
						 m_rucksacks[cur_group+1],
						 m_rucksacks[cur_group+2]);
      cur_group += 3;
    }
    if(m_rucksacks.size() - cur_group != 0) {
      fprintf(stderr, "Uneven groups of three!");
    }
    return tot_group_priority;
  }
};

int main() {
  RucksackReorg rr;
  rr.read_input();
  int tot_rr_priority = rr.tot_rucksacks_priority();
  printf("Tot priority: %d ", tot_rr_priority);
  int tgp = rr.tot_group_priority();
  printf("Tot group priority: %d\n", tgp);
}
