#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "trie.h"

using namespace std;

typedef vector<char> BoggleRow;
typedef vector<BoggleRow> BoggleBoard;

const int coords[16] = 
{
  -1, -1,
   0, -1,
   1, -1,
  -1,  0,
   1,  0,
  -1,  1,
   0,  1,
   1,  1
};

vector<string> words_longer_than(const vector<string>& words, const int len) {
  vector<string> filtered;
  for(vector<string>::const_iterator it = words.begin(); it != words.end(); ++it)
    if((*it).size() > len)
      filtered.insert(filtered.end(), *it);
  return filtered;
}

inline bool is_valid_coord(const int i, const int j, const int size) {
  return i >= 0 && j >= 0 && i < size && j < size;
}

boost::shared_array<int> offset_coord(const int i, const int j) {
  int *new_coords = new int[16];
  memset(new_coords, 0, sizeof(int)*16);
  for (int k = 0; k < 16; k+=2) {
    new_coords[k] = coords[k] + i;
    new_coords[k+1] = coords[k+1] + j;
  }
  return boost::shared_array<int>(&new_coords[0]);
}

set<string> solve_recursive(Trie *dict,
                            const boost::shared_array<char>& board,
                            bitset<10000> visited,
                            const int len,
                            const string& word, int i, int j) {
  const int board_index = i*len + j;
  const char board_char = board[board_index];
  Trie *new_dict = dict->get_sub_trie(board_char);
  const string new_word = word + board_char;
  visited.set(board_index);
  set<string> words;
  if(new_dict->is_end_of_word())
    words.insert(new_word);
  boost::shared_array<int> adjacent = offset_coord(i, j);
  for (int k = 0; k < 16; k+=2) {
    int new_i = adjacent[k];
    int new_j = adjacent[k+1];
    const int new_index = new_i*len + new_j;
    if(is_valid_coord(new_i, new_j, len) && !visited.test(new_index) && new_dict->get_sub_trie(board[new_index]) != NULL){
      set<string> w = solve_recursive(new_dict, board, visited, len, new_word, new_i, new_j);
      if(w.size() > 0) words.insert(w.begin(), w.end());
    }
  }
  return words;
}

boost::shared_ptr<Trie> build_trie(const vector<string>& dict, const boost::shared_array<bool> adj_matrix) {
  boost::shared_ptr<Trie> trie(new Trie(adj_matrix));
  for(int i = 0; i < dict.size(); i++)
    trie->add_word(dict[i].c_str());
  return trie;
}

boost::shared_array<bool> build_adj_matrix(const BoggleBoard& board) {
  const int len = board.size();
  boost::shared_array<bool> b(new bool[26*26]);
  memset(b.get(), false, sizeof(bool)*26*26);
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < len; ++j) {
      boost::shared_array<int> adjacent = offset_coord(i, j);
      for (int k = 0; k < 16; k+=2) {
        int new_i = adjacent[k];
        int new_j = adjacent[k+1];
        if(is_valid_coord(new_i, new_j, len)){
          b[(board[i][j]-'a')*26 + board[new_i][new_j]-'a'] = true;
          b[(board[new_i][new_j]-'a')*26 + board[i][j]-'a'] = true;
        }
      }
    }
  }
  return b;
}

boost::shared_array<char> build_board(const BoggleBoard& board, const int len) {
  boost::shared_array<char> b(new char[len*len]);
  for (int i = 0; i < len; ++i) {
    for (int j = 0; j < len; ++j) {
      b[i*len + j] = board[i][j];
    }
  }
  return b;
}

vector<string> get_boggle_words(const vector<string>& dictionary,
                                  const BoggleBoard& board) {
  const vector<string> filtered_words = words_longer_than(dictionary, 2);
  const boost::shared_array<bool> adj_matrix = build_adj_matrix(board);
  const boost::shared_ptr<Trie> trie = build_trie(filtered_words, adj_matrix);
  set<string> words_set;
  boost::shared_array<char> b = build_board(board, board.size());
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board.size(); ++j) {
      bitset<10000> v;
      set<string> new_words = solve_recursive(trie.get(), b, v, board.size(), string(), i, j);
      words_set.insert(new_words.begin(), new_words.end());
    }
  }
  vector<string> words;
  words.insert(words.begin(), words_set.begin(), words_set.end());
  return words;
}



void read_boggle_dictionary(const string& filename, vector<string>& result) {
  try {
    ifstream infile(filename.c_str());
    if (!infile.is_open()) {
      throw runtime_error(string("Failed to open board file '" + filename + "'.").c_str());
    }
    string line;
    while(getline(infile, line)) {
      if (!line.empty()) {
        result.push_back(line);
      }
    }
  }
  catch(const runtime_error& e) {
    const string error("Failed to open dictionary file '" + filename + "'.");
    throw runtime_error(error.c_str());
  }
}

void read_boggle_board(const string& filename, BoggleBoard& result_in) {
  ifstream f(filename.c_str());
  if (!f.is_open()) {
    throw runtime_error(string("Failed to open board file '" + filename + "'.").c_str());
  }
  vector<char> letters;
  char c;
  while (f >> c) {
    letters.push_back(c);
  }
  const unsigned int n = sqrt(letters.size());
  if ((n * n) != letters.size())
    throw runtime_error("board file is not NxN");
  BoggleBoard result;
  for (unsigned int i=0;i<n;++i) {
    BoggleRow r;
    for (unsigned int j=0;j<n;++j) {
      r.push_back(letters[(i*n)+j]);
    }
    result.push_back(r);
  }
  result_in=result;
}

void show_boggle_board(const BoggleBoard& b) {
  cout << "#######" << endl;
  for (BoggleBoard::const_iterator i = b.begin(); i != b.end(); ++i) {
    for (BoggleRow::const_iterator j = i->begin(); j != i->end(); ++j) {
      cout << *j << " ";
    }
    cout << endl;
  }
  cout << "#######" << endl << endl;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    return 1;
  }
  const string& board_file = argv[1];
  const string& dictionary_file = argv[2];
  try {
    // Read the dictionary in from the input file
    vector<string> dictionary;
    read_boggle_dictionary(dictionary_file, dictionary);
    cout << "Dictionary contains " << dictionary.size() << " words." << endl;
    // Read the board data in from the input file
    BoggleBoard board;
    read_boggle_board(board_file, board);
    // Print out the board
    show_boggle_board(board);
    // Fetch the words (you'll need to implement get_words)
    vector<string> words = get_boggle_words(dictionary, board);
    // Print out the words
    cout << "Found " << words.size() << " words:" << endl;
    for(vector<string>::const_iterator it = words.begin(); it != words.end(); it++)
      cout << *it << endl;
    return 0;
  }
  catch (const runtime_error& e) {
    // We'll get here if you specify a bad board file or a bad
    // dictionary file.
    cout << e.what() << endl;
    return 1;
  }
}
