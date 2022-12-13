#include <cstdio>
#include <vector>
#include <string>
#include <format>
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

bool debug = true;

class Node {
protected:
  shared_ptr<Node> m_parent;
  string m_name;
  Node(shared_ptr<Node> parent, string name): m_parent(parent), m_name(name) {
  }
public:
  virtual const string& get_name() const {
    return m_name;
  }
  virtual ~Node() = default;
  virtual int size() const = 0;
  shared_ptr<Node> get_parent() const {
    return m_parent;
  }
};

class File;

class Directory : public Node {
private:
  map<string, shared_ptr<Node> > m_directories;
  map<string, shared_ptr<Node> > m_files;
public:
  Directory(shared_ptr<Directory> parent, string name):
    Node(parent, name),
    m_directories (map<string, shared_ptr<Node> > ()) {
    if (debug) cerr << "Directory parent: " << parent << " name: " << name << endl;
  }
  shared_ptr<Directory> add_dir(const string &name, shared_ptr<Directory> parent) {
    auto it = m_directories.find(name);
    if(it == m_directories.end()) {
      m_directories[name] = make_shared<Directory>(parent, name);
      it = m_directories.find(name);
    }
    // return the directory
    return dynamic_pointer_cast<Directory>(it->second);
  }
  void add_file(shared_ptr<Directory> dir, const string &name, int size);
  int size() const {
    int dirsize = 0;
    for(auto const& f : m_files) {
      dirsize += f.second->size();
    }
    for(auto const& d : m_directories) {
      dirsize += d.second->size();
    }
    cerr << "DIR: " << this->get_name()  << " has size " << dirsize << endl;
    return dirsize;
  }
  shared_ptr<Node> get_parent() const {
    return Node::get_parent();
  }
};

class File : public Node {
private:
  int m_size;
public:
  File(shared_ptr<Directory> parent, string name, int size): Node(parent, name),
    m_size(size) {
    if (debug) cerr << "File parent: " << parent << " name: " << name << " size: " << size  << endl;
  }
  int size() const {
    return m_size;
  }
};

void Directory::add_file(shared_ptr<Directory> dir, const string &name, int size ) {
  shared_ptr<File> pp = make_shared<File>(dir, name, size);
  m_files[name] = pp;
}


class FS {
private:
  shared_ptr<Directory> m_root;
public:
  FS(): m_root(make_shared<Directory>(Directory(NULL, "/"))) {
  }
  void read_input() {
    string line;
    shared_ptr<Directory> cur = m_root;
    while(getline(cin, line)) {
      if(line[0] == '$') {
	// command
	string cmd = line.substr(2);
	if(cmd.substr(0,2) == "cd") {
	  string dir = cmd.substr(3);
	  if(dir == "/") {
	    cur = m_root;
	  } else if(dir == "..") {
	    cur = dynamic_pointer_cast<Directory> (cur->get_parent());
	  } else {
	    // Go to other dir than root
	    cur = cur->add_dir(dir, cur);
	  }
	} else if(cmd.substr(0, 2) == "ls") {
	  // Do nothing???
	}
      } else if(line.substr(0, 3) == "dir") {
	string dir = line.substr(4);
	cur->add_dir(dir, cur);
	cerr << "DIR added:" << dir << endl;
      } else {
	cerr << "Is this a file??? " << line << endl;
	stringstream ss(line);
	int filesize;
	string filename;
	ss >> filesize >> filename;
	cur->add_file(cur, filename, filesize);
      }
      //cout << line << endl;
    }
  }
  int root_size() const {
    return m_root-> size();
  }
};

int main(int argc, char * argv[]) {
  unique_ptr<FS> fs = make_unique<FS> ();
  fs->read_input();
  int root_size = fs->root_size();
  cerr << "Root has size: " << root_size  << endl;  
}
