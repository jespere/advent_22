#include <cstdio>
#include <vector>
#include <utility>

using namespace std;

typedef pair<int,int> cleanrange_t;
typedef pair<cleanrange_t, cleanrange_t> two_cleanrange_t;


auto debug = false;

auto read_ranges() {
  vector<two_cleanrange_t> res;
  int x1,x2,y1,y2;
  while(scanf("%d-%d,%d-%d\n", &x1,&x2,&y1,&y2) == 4) {
    res.push_back ( make_pair(make_pair(x1,x2), make_pair(y1,y2)) );
  }

  return res;
}

bool cleanrange_contained(const cleanrange_t &r1, const cleanrange_t &r2) {
  return
    // Is r1 within r2
    (r1.first >= r2.first && r1.second <= r2.second) ||
    //Is r2 within r2
    (r2.first >= r1.first && r2.second <= r1.second)
    ;
}

int containing(const vector<two_cleanrange_t> &rs) {
  int num_cantains = 0;
  for(const auto &r : rs) {
    if(debug)
      fprintf(stderr,"%d-%d,%d-%d\n",r.first.first, r.first.second, r.second.first, r.second.second);
    if(cleanrange_contained(make_pair(r.first.first, r.first.second), make_pair(r.second.first, r.second.second)))
      num_cantains++;
  }
  return num_cantains;
}

void smallest_first(vector<two_cleanrange_t> &rs) {
  for(int i=0; i < rs.size(); i++) {
    if(rs[i].first.first > rs[i].second.first) {
      rs[i] = make_pair(make_pair(rs[i].second.first, rs[i].second.second),
			make_pair(rs[i].first.first,  rs[i].first.second));
    }
  }
}

int num_some_overlap(vector<two_cleanrange_t> &rs) {
  int n_some_overlap = 0;
  for(const auto &e : rs) {
    if(e.first.second >= e.second.first) {
      n_some_overlap++;
    }
  }
  return n_some_overlap;
}

int main() {
  auto pairs = read_ranges();
  smallest_first(pairs);
  int contains = containing(pairs);
  printf("Containing: %d\n", contains);
  int s_overlap = num_some_overlap(pairs);
  printf("Some overlap: %d\n\n", s_overlap);
}
