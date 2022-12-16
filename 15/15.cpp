#include <cstdio>
#include <utility>
#include <vector>
#include <algorithm>

static const bool debug = true;

using namespace std;

struct SBPair {
  pair<int, int> m_sensor;
  pair<int, int> m_beacon;
  SBPair(const pair<int,int>& sensor, const pair<int,int>& beacon);
};

SBPair::SBPair(const pair<int,int>& sensor, const pair<int,int>& beacon):
  m_sensor(sensor),
  m_beacon(beacon) {
}

// bool operator<(const SBPair& sbl, const SBPair& sbh) {
//   return sbl.m_sensor.first < sbh.m_sensor.first ||
//     (sbl.m_sensor.first == sbh.m_sensor.first && sbl.m_sensor.second < sbh.m_sensor.second);
// }

int manhattan(const pair<int,int>& p1, const pair<int,int>& p2) {
  return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

class Network {
  vector<SBPair> m_network;
public:
  void read();
  int solve1(const int& yline);
};

void Network::read() {
  int sx, sy, bx, by;
  // Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d
  while(scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sx, &sy, &bx, &by) == 4) {
    //if (debug) fprintf(stderr,"sx: %d sy: %d bx: %d by: %d\n", sx, sy, bx, by);
    m_network.push_back(SBPair(make_pair(sx,sy), make_pair(bx,by)));
  }
}

int Network::solve1(const int& yline) {
  // An array of ranges where beacon cannot be present
  vector<pair<int,int> > excluded;
  for(const SBPair& sbp : m_network) {
    // Dist from: Sensor to Beacon
    int dist_s_b = manhattan(sbp.m_sensor, sbp.m_beacon);
    //if(debug) fprintf(stderr, "Dist: %d\n", dist_s_b);
    int dist_sensor_y = abs(sbp.m_sensor.second - yline);
    int spread = dist_s_b - dist_sensor_y;
    if(spread >= 0) {
      // How much space
      excluded.push_back(make_pair(sbp.m_sensor.first - spread, sbp.m_sensor.first + spread));
    }
  }
  // Now merge overlapping
  sort(excluded.begin(), excluded.end());
  //if(debug) fprintf(stderr, "excluded size(): %lu\n", excluded.size());
//  if(debug) {
//    for(const auto &p : excluded) {
//      fprintf(stderr, "s: %d e: %d, ", p.first, p.second);
//    }
//    fprintf(stderr, "\n");
//  }

  vector<pair<int,int> > excluded_merged;

  int old_size;
  do {
    excluded_merged.clear();
    for(vector<pair<int,int> >::iterator it = excluded.begin(); it != excluded.end(); it++) {
      if(it + 1 != excluded.end() && it->second >= (it+1)->first) {
	// Merge
	excluded_merged.push_back(make_pair( it->first, max(it->second, (it+1)->second)) );
	++it;
      } else {
	excluded_merged.push_back(*it);
      }
    }
//    if (debug) fprintf(stderr, "excluded.size(): %lu excluded_merged.size(): %lu \n",
//		       excluded.size(),
//		       excluded_merged.size());
    old_size = excluded.size();
    excluded.clear();
    excluded = vector<pair<int,int> > (excluded_merged.begin(), excluded_merged.end());
  } while(old_size != excluded_merged.size());

//  if(debug) {
//    for(const auto &p : excluded_merged) {
//      fprintf(stderr, "s: %d e: %d, ", p.first, p.second);
//    }
//    fprintf(stderr, "\n");
//  }

  int sum = 0;
  for(const auto &p : excluded_merged) {
    sum += p.second - p.first;
  }
  return sum;
}


int main(int argc, char* argv[]) {
  Network n;
  n.read();
  if(argc >= 2) {
    int sum = n.solve1(atoi(argv[1]));
    printf("Sum: %d\n", sum);
  }
}
