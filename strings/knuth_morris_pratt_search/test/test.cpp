#include <vector>
#include <string>

#include "../include/knuth_morris_pratt.hpp"
#include "../../../test/catch_amalgamated.hpp"


bool check_prefix_build_1() 
{
    vector<int> expected = {0, 0, 0, 0, 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6};
    vector<int> actual = kmp_table("abcdabcabcdabcdab");

    if (expected.size() != actual.size()) {
        return false;
    }

    for (int i = 0; i < expected.size(); ++i) {
        if (expected[i] != actual[i]) {
            return false;
        }
    }

    return true;
}

TEST_CASE( "Test prefix function building", "[KMP search]" ) {
    REQUIRE(check_prefix_build_1());
}

bool check_search_1()
{
    vector<int> expected = {0, 7, 11};
	vector<int> actual = kmp_search("abcdabcabcdabcdab", "abcd");
    if (expected.size() != actual.size()) {
		return false;
	}
    for (int i = 0; i < expected.size(); ++i) {
        if (expected[i] != actual[i]) {
			return false;
		}
	}

	return true;
}

TEST_CASE("Test search function", "[KMP search]") {
    REQUIRE(check_search_1());
}
