#include <map>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include "trie.h"

void Trie::add_word(const char* str) {
  const char c = str[m_depth];
  if(c == '\0') //end of word
    m_is_word = true;
  if(c < 'a' || c > 'z' || !in_adj_mat(str)) // not interested
    return;
  if(m_child_map[c-'a'] == NULL)
    m_child_map[c-'a'] = boost::shared_ptr<Trie>(new Trie(m_depth+1, m_adj_matrix));
  m_child_map[c-'a']->add_word(str);
}

void Trie::add_words(const char **strs, int len) {
  for (int i = 0; i < len; i++)
    add_word(strs[i]);
}

bool Trie::is_end_of_word() const {
  return m_is_word;
}

Trie *Trie::get_sub_trie(const char c) {
  if(m_child_map[c-'a'] != NULL)
    return m_child_map[c-'a'].get();
  else
    return NULL;
}

