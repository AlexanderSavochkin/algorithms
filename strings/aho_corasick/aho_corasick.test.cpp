/*
Autor: Alexander Savochkin, 2014
WTFPL v.2 License
*/
 
#define BOOST_TEST_MODULE SRCLFTest
#include <boost/test/unit_test.hpp>
 
#include "AhoCorasickTrie.h"

#include <vector>

using std::vector;

bool testHashMapBasedAhoCorasickFind() 
{
    AhoCorasikSearcher<MapBasedLoadedTrie<int>, int> searcher;
    MapBasedLoadedTrie<int> &mapTrie = searcher.getMutableTrie();

    mapTrie.addString(L"HER", 1);
    mapTrie.addString(L"SHE", 2);
    mapTrie.addString(L"HE", 3);
    mapTrie.addString(L"IS", 4);
    mapTrie.addString(L"HERS", 5);
    mapTrie.addString(L"I", 6);
    mapTrie.addString(L"HIS", 7);
    mapTrie.addString(L"HIM", 8);
    mapTrie.addString(L"ME", 9);
    mapTrie.addString(L"AM", 10);
    mapTrie.addString(L"HI", 11);

    searcher.init();

    wstring text = L"HISHER HIMERAME";

/*
0 1 HI
1 1 I
0 2 HIS
1 2 IS
2 4 SHE
3 4 HE
3 5 HER
6 7 HI
7 7 I
6 8 HIM
8 9 ME
10 11 AM
11 12 ME
*/

    vector<Find> expected_result = {
        Find( 0, 1, 10), Find( 1, 1, 5), Find( 0, 2, 6), Find( 1, 2, 3), Find( 2, 4, 1), Find( 3, 4, 2), Find( 3, 5, 0), Find( 6, 7, 10),
        Find( 7, 7, 5), Find( 6, 8, 7), Find( 8, 9, 8), Find( 10, 11, 9), Find( 11, 12, 8)
    };

    vector<Find> result;
    searcher.search( text.cbegin(), text.cend(), back_inserter(result) );

    return expected_result == result;
}

bool testHashMapBasedAhoCorasickFind1() 
{
    AhoCorasikSearcher<MapBasedLoadedTrie<int>, int> searcher;
    MapBasedLoadedTrie<int> &mapTrie = searcher.getMutableTrie();

    mapTrie.addString(L"2", 1);
    mapTrie.addString(L"22", 2);
    mapTrie.addString(L"232", 3);
    mapTrie.addString(L"2222", 4);
    mapTrie.addString(L"22322", 5);

    searcher.init();

    wstring text = L"2222223222222222322222222222322222222232222222222232222222223222222222223222222222322222";

    vector<Find> result;
    searcher.search( text.cbegin(), text.cend(), back_inserter(result) );

    bool all_found = true;

    for (int i = 85; i <= 87; ++i)
    {
           bool found = false;
           for (int j = 0; j < result.size(); ++j)
           {
              if (result[j].from == i && result[j].to == i)
              {
                  found = true;
                  break;
              }
           }
		   all_found = all_found && found;
    }

    return all_found;
}

bool testHashMapBasedAhoCorasickFind3()
{
	AhoCorasikSearcher<MapBasedLoadedTrie<int>, int> searcher;
	MapBasedLoadedTrie<int> &mapTrie = searcher.getMutableTrie();

	mapTrie.addString(L"b", 1);
	mapTrie.addString(L"a", 2);
	mapTrie.addString(L"aba", 3);

	searcher.init();

	wstring text = L"aba";

	vector<Find> result;
	searcher.search(text.cbegin(), text.cend(), back_inserter(result));

	bool count_ok = result.size() == 4;

	return count_ok;
}

BOOST_AUTO_TEST_CASE( test1 ) 
{
    BOOST_CHECK( testHashMapBasedAhoCorasickFind() ); 
}

BOOST_AUTO_TEST_CASE( test2 ) 
{
    BOOST_CHECK( testHashMapBasedAhoCorasickFind1() ); 
}

BOOST_AUTO_TEST_CASE(test3)
{
	BOOST_CHECK(testHashMapBasedAhoCorasickFind3());
}
