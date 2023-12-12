/*
Autor: Alexander Savochkin, 2014
WTFPL v.2 License
*/

#pragma once

#include <vector>
#include <string>
#include <iterator>
#include <unordered_map>
#include <iterator>
#include <queue>

using std::unordered_map;
using std::vector;
using std::wstring;
using std::iterator;
using std::back_insert_iterator;
using std::queue;
using std::priority_queue;
using std::move;

template<typename TPayLoad>
class MapBasedLoadedTrie
{
public: 

    class TrieNodeMap;
    typedef TrieNodeMap NodeType;

    class TrieNodeMap
    {
        unordered_map<wchar_t, int> child_nodes;
        int match_index;
        int suffix_link;
        int term_link;

    public:
        TrieNodeMap() : match_index(-1), suffix_link(0), term_link(-1) {}
        bool hasChild(wchar_t c) const { return child_nodes.count(c); };
        int getChild(wchar_t c) const { return child_nodes.at(c); };
        int getSuffixLink() const { return suffix_link; }
        int getMatchIndex() const { return match_index; }        
        int getTermLink() const { return term_link; }                

        typedef unordered_map<wchar_t, int>::const_iterator ChildIterator;
        ChildIterator beginIterator() const { return child_nodes.begin(); };
        ChildIterator endIterator() const { return child_nodes.end(); };

    private:
        typedef unordered_map<wchar_t, int>::iterator MutableChildIterator;
        MutableChildIterator beginMutableIterator() const { return child_nodes.begin(); };
        MutableChildIterator endMutableIterator() const { return child_nodes.end(); };

        template<typename TTrie, typename TNode>
        friend class AhoCorasikSearcher;

        friend class MapBasedLoadedTrie;
    };

private:

    vector<NodeType> trie;
    vector<wstring> dictionary;
    vector<vector<TPayLoad>> payloadStorage;

public:

    MapBasedLoadedTrie() : trie(1) {};

    const wstring& getString(int index) const { return dictionary[index]; }
    const vector<TPayLoad>& getPayLoads(int index) const { return payloadStorage[index]; }

    const NodeType& getNode(int node_index) const { return trie[node_index]; }


    void addString(const wstring& s, const TPayLoad& payload)
    {
        //Insert nodes into trie
        int current_node = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            if (!trie[current_node].hasChild( s[i] ))
            {
                trie[current_node].child_nodes[ s[i] ] = trie.size();
                current_node = trie.size();
                trie.push_back( NodeType() );
            }
            else
            {
               current_node = trie[current_node].child_nodes[ s[i] ];
            }
        }

        if ( trie[current_node].match_index == -1)
        {
            trie[current_node].match_index = dictionary.size();
            dictionary.push_back(s);
            payloadStorage.push_back( vector<TPayLoad>() );
        }

        payloadStorage[ trie[current_node].match_index ].push_back( payload );
    }

private:
    NodeType& getMutableNode(int node_index) { return trie[node_index]; }

    template<typename TTrie, typename TNode>
    friend class AhoCorasikSearcher;

};

/*
class TrieNodeVector
{
    map<wchar_t, int> child_nodes;
    int match_index;
    int suffix_link;
    int term_link;

    TrieNode() : match_index(-1), suffix_link(0), term_link(-1) {}
};
*/

struct Find
{
    int from;
    int to;
    int word_index;

    Find( int _from, int _to, int _word_index ) : from(_from), to(_to), word_index(_word_index) {}

    bool operator == (const Find& other) const {
        return (from == other.from) && (to == other.to) && (word_index == other.word_index);
    }

};

template <typename TLoadedTrie, typename TPayLoad>
class AhoCorasikSearcher
{
    TLoadedTrie trie;
public:

    AhoCorasikSearcher() {};

    const TLoadedTrie& getTrie() { return trie; }
    TLoadedTrie& getMutableTrie() { return trie; }

    void init()
    {
        trie.getMutableNode(0).suffix_link = 0;
        trie.getMutableNode(0).term_link = -1;
        queue<int> q;
        //q.push(0);
        for (auto it = trie.getNode(0).beginIterator(); it != trie.getNode(0).endIterator(); ++it )
            q.push( it->second );
        while( !q.empty() )
        {
            int current_node_index = q.front();
            q.pop();
            for (auto it = trie.getNode(current_node_index).beginIterator(); it != trie.getNode(current_node_index).endIterator(); ++it )
            {
                int current_suffix_index = trie.getNode(current_node_index).suffix_link;
                while ( 1 )
                {
                    if ( trie.getNode(current_suffix_index).child_nodes.count(it->first) )
                    {
                        int this_child_index = trie.getNode(current_node_index).child_nodes.at(it->first);
                        int suffix_child_index = trie.getNode(current_suffix_index).child_nodes.at(it->first);
                        trie.getMutableNode(this_child_index).suffix_link = suffix_child_index;

                        int terminal_candidate = suffix_child_index;
                        while (trie.getNode(terminal_candidate).match_index == -1 && terminal_candidate != 0)
                        {
                            terminal_candidate = trie.getNode(terminal_candidate).suffix_link;
                        }
                        trie.getMutableNode(this_child_index).term_link = terminal_candidate;
                        break;
                    }
                    if (current_suffix_index == 0)
                        break;
                    current_suffix_index = trie.getNode(current_suffix_index).suffix_link;
                }
                q.push(it->second);
            }
        }
    }

    void search( wstring::const_iterator iter_start, wstring::const_iterator iter_end, back_insert_iterator<vector<Find>> result) const
    {
        int current_state = 0;
        int text_position = 0;
        for ( auto iter = iter_start; iter != iter_end; ++iter )
        {
            wchar_t current_character = *iter;
            while ( !trie.getNode(current_state).hasChild( current_character ) && current_state != 0 )
                current_state = trie.getNode(current_state).getSuffixLink();

            if( trie.getNode(current_state).hasChild( current_character )  )
            {
                current_state = trie.getNode(current_state).getChild( current_character );
                if (trie.getNode(current_state).getMatchIndex() != -1)
                {
                    int match_string_index = trie.getNode(current_state).getMatchIndex();
                    *result = Find( text_position - trie.getString(match_string_index).size() + 1, text_position, match_string_index);
                }

                int run_up_current_state = current_state;
                while (trie.getNode(run_up_current_state).getTermLink() != -1)
                {
                    run_up_current_state = trie.getNode(run_up_current_state).getTermLink();
                    int match_string_index = trie.getNode(run_up_current_state).getMatchIndex();
                    if (match_string_index == -1)
                        break;
                    *result = Find( text_position - trie.getString(match_string_index).size() + 1, text_position, match_string_index);
                }
                ++text_position;
            }

            // else current_state = 0

        }
    }
};