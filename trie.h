#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

class Trie {
  public:
    Trie(const boost::shared_array<bool> adj_matrix) : m_depth(0), m_adj_matrix(adj_matrix), m_is_word(false) {}
    
    void add_word(const char *str);
    void add_words(const char **str, int len);

    bool is_end_of_word() const;

    Trie *get_sub_trie(const char c);

  private:
    Trie(const int depth, const boost::shared_array<bool> adj_matrix) : m_depth(depth), m_adj_matrix(adj_matrix), m_is_word(false) {}
    
    const boost::shared_array<bool> m_adj_matrix;
    boost::shared_ptr<Trie> m_child_map[26];
    int m_depth;
    bool m_is_word;

    inline bool in_adj_mat(const char* str) {
      return m_depth == 0 ? true : m_adj_matrix[(str[m_depth-1]-'a')*26 + str[m_depth]-'a'];
    }
};

#endif /* TRIE_H_INCLUDED */
