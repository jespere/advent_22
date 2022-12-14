#include <iostream>
#include <sstream>
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
	m_start = make_pair(i,j);
      } else if(m_graph[i][j] == 'E') {
	m_end = make_pair(i,j);
      }
    }
  }
  if(debug) cerr << "Start: " << m_start.first << " " << m_start.second <<
	      " End: " << m_end.first << " " << m_end.second << endl;
}

V Graph::bfs(const V &start, const V &end) {
  list<V> queue;

  int tmp_depth = 1;
  
  queue.push_back(V(start.m_x, start.m_y, 0));
  m_visited[start.m_x][start.m_y] = true;
  while(!queue.empty()) {
    V cur = queue.front();
    // Found it!
    tmp_depth++;
    if(cur == end) {
      cout << "tmp_depth: " << tmp_depth << endl;
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

int main() {
  string line;
  Graph g;

  while(getline(cin, line)) {
    //if(debug) cerr << "Line: " << line << endl;
    g.add_line(line);
  }
  g.prepare();
  V found = g.bfs(V(g.m_start.first, g.m_start.second, 0), V(g.m_end.first, g.m_end.second, 100000));
  cout << "Found depth: "  << found.m_depth << endl;
  return 0;
}
