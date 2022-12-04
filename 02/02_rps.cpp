#include <cstdio>
#include <vector>
#include <string>
#include <map>

using namespace std;

class RPS {
public:
  RPS(): m_score_table(map<string, int>()),
	 m_score_table_2(map<string, int>()) {
    //                 Score for result + Score for RPS
    m_score_table["A X"] = 3 + 1; // Rock rock
    m_score_table["A Y"] = 6 + 2; // Rock paper
    m_score_table["A Z"] = 0 + 3; // Rock scissor

    m_score_table["B X"] = 0 + 1; // Paper Rock
    m_score_table["B Y"] = 3 + 2; // Paper Paper
    m_score_table["B Z"] = 6 + 3; // Paper Scissor

    m_score_table["C X"] = 6 + 1; // Scissor Rock
    m_score_table["C Y"] = 0 + 2; // Scissor Paper
    m_score_table["C Z"] = 3 + 3; // Scissor Scissor

    //                       Score for RPS + Score for result
    // Agains rock
    m_score_table_2["A X"] = 3 + 0; // Scissor +   Lose
    m_score_table_2["A Y"] = 1 + 3; // Rock    +   Draw
    m_score_table_2["A Z"] = 2 + 6; // Paper   +   Win

    // Against Paper
    m_score_table_2["B X"] = 1 + 0; //  Rock    +   Lose
    m_score_table_2["B Y"] = 2 + 3; //  Paper   +   Draw
    m_score_table_2["B Z"] = 3 + 6; //  Scissor +   Win

    // Against Scissor
    m_score_table_2["C X"] = 2 + 0; // Paper   +   Lose
    m_score_table_2["C Y"] = 3 + 3; // Scissor +   Draw
    m_score_table_2["C Z"] = 1 + 6; // Rock    +   Win

  }
  void read_input() {
    char line[1024];
    while(fgets(line, sizeof line, stdin)) {
      int len = strlen(line);
      if(line[len -1] == '\n') {
	line[len -1] = 0;
      }
      m_plays.push_back(line);
    }
  }
  int sum_scores() const {
    int scores = 0;
    for(const string &p : m_plays) {
      const auto it = m_score_table.find(p);
      if(it != m_score_table.end()) {
	scores += it->second;
      } else {
	fprintf(stderr, "Could not find score for: %s\n", p.c_str());
      }
    }
    return scores;
  }
  int sum_scores_2() const {
    int scores = 0;
    for(const string &p : m_plays) {
      const auto it = m_score_table_2.find(p);
      if(it != m_score_table_2.end()) {
	scores += it->second;
      } else {
	fprintf(stderr, "Could not find score for: %s\n", p.c_str());
      }
    }
    return scores;
  }
private:
  map<string, int> m_score_table;
  map<string, int> m_score_table_2;
  vector<string> m_plays;
};

int main() {
  RPS rps;
  rps.read_input();
  int score = rps.sum_scores();
  printf("Score: %d \n", score);
  int score_2 = rps.sum_scores_2();
  printf("Score 2: %d \n", score_2);
}
