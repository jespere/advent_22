#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#include <list>

using namespace std;

static bool debug = true;

struct V {
  int m_x;
  int m_y;
  int m_depth;

  V(int x, int y, int depth);
  bool operator==(const V& rhs) const;
};

bool V::operator==(const V& rhs) const {
  return 
    this->m_x == rhs.m_x &&
    this->m_y == rhs.m_y;
}

V::V(int x, int y, int depth): m_x(x), m_y(y), m_depth(depth) {
}

class Graph {
public:
  vector<vector<char> > m_graph;
  vector<vector<bool> > m_visited;
  pair<int, int> m_start;
  pair<int, int> m_end;
  V bfs(const V &start, const V &end);
  bool visited(const V &v) const;
  bool in_graph(const V &v) const;
  bool can_climb(const V &from, const V &to) const;
public:
  Graph();
  void add_line(const string&);
  void prepare();
  void reset_visited();
};

bool Graph::can_climb(const V &from, const V &to) const {
  char from_char = m_graph[from.m_x][from.m_y] == 'S' ? 'a' : m_graph[from.m_x][from.m_y];
  char to_char = m_graph[to.m_x][to.m_y] == 'E' ? 'z' : m_graph[to.m_x][to.m_y];
  return to_char - from_char <= 1;
}

bool Graph::visited(const V &v) const {
  return m_visited[v.m_x][v.m_y];
}
bool Graph::in_graph(const V &v) const {
  return
    v.m_x >= 0 && v.m_x < m_graph.size() &&
    v.m_y >= 0 && v.m_y < m_graph[v.m_x].size();
}


Graph::Graph():
  m_graph(vector<vector<char> >()),
  m_visited(vector<vector<bool> >()) {
}

void Graph::add_line(const string &line) {
  vector<char> tmp (line.begin(), line.end());
  m_graph.push_back(tmp);
  m_visited.push_back(vector<bool> (line.size(), false));
}

void Graph::prepare() {
  for(int i=0; i < m_graph.size(); ++i) {
    for(int j=0; j < m_graph[i].size(); ++j) {
      if(m_graph[i][j] == 'S') {
	m_graph[i][j] = 'a';
	m_start = make_pair(i,j);
      } else if(m_graph[i][j] == 'E') {
	m_end = make_pair(i,j);
      }
    }
  }
//  if(debug) cerr << "Start: " << m_start.first << " " << m_start.second <<
//	      " End: " << m_end.first << " " << m_end.second << endl;
}

void Graph::reset_visited() {
  for(int x=0; x < m_visited.size(); x++) {
    for(int y=0; y < m_visited.size(); y++) {
      m_visited[x][y] = false;
    }
  }
}

V Graph::bfs(const V &start, const V &end) {
  list<V> queue;

  
  queue.push_back(V(start.m_x, start.m_y, 0));
  m_visited[start.m_x][start.m_y] = true;
  while(!queue.empty()) {
    V cur = queue.front();
    // Found it!
    if(cur == end) {
      return cur;
    }
    queue.pop_front();

    V candidate(0,0,0);
    // Left
    candidate = V(cur.m_x - 1, cur.m_y, cur.m_depth + 1);
    if(in_graph(candidate) && can_climb(cur, candidate)  && !visited(candidate)) {
      m_visited[candidate.m_x][candidate.m_y] = true;
      queue.push_back(candidate);
    }
    // Up
    candidate = V(cur.m_x, cur.m_y - 1, cur.m_depth + 1);
    if(in_graph(candidate) && can_climb(cur, candidate)  && !visited(candidate)) {
      m_visited[candidate.m_x][candidate.m_y] = true;
      queue.push_back(candidate);
    }
    // Right
    candidate = V(cur.m_x + 1, cur.m_y, cur.m_depth + 1);
    if(in_graph(candidate) && can_climb(cur, candidate)  && !visited(candidate)) {
      m_visited[candidate.m_x][candidate.m_y] = true;
      queue.push_back(candidate);
    }
    // Down
    candidate = V(cur.m_x, cur.m_y + 1, cur.m_depth + 1);
    if(in_graph(candidate) && can_climb(cur, candidate)  && !visited(candidate)) {
      m_visited[candidate.m_x][candidate.m_y] = true;
      queue.push_back(candidate);
    }
  }
  // Not found. Error
  return V(-1,-1,-1);
}

int main(int argc, char *argv[]) {
  string line;
  Graph g;
  
  ifstream myfile1 ("12_input.in");
  while(getline(myfile1, line)) {
    //if(debug) cerr << "Line: " << line << endl;
    g.add_line(line);
  }

  int min_path = INT_MAX;
  
  for(int x = 0; x < g.m_graph.size(); x++) {
    for(int y = 0; y < g.m_graph[x].size(); y++) {
      if(g.m_graph[x][y] == 'a') {
	ifstream myfile2 ("12_input.in");
	Graph g2;
	while(getline(myfile2, line)) {
	  //if(debug) cerr << "Line: " << line << endl;
	  g2.add_line(line);
	}
	g2.prepare();
	g2.reset_visited();
	V found = g2.bfs(V(x, y, 0), V(g2.m_end.first, g2.m_end.second, 100000));
	//cout << "Found depth: "  << found.m_depth << endl;
	if(found.m_depth != -1)
	  min_path = found.m_depth < min_path ? found.m_depth : min_path;
      }
    }
  }
  cout << "Min path: " << min_path << endl;
  return 0;
}
