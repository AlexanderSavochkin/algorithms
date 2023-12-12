#include <cstdlib>
#include <vector>


#include "../../../../test/catch_amalgamated.hpp"
#include "../include/segmented_tree.hpp"

using std::rand;
using std::vector;


bool check(int num_iter, int min_size, int max_size) 
{
    for (int i = 0; i < num_iter; ++i) 
    {
        int size = min_size + rand() % (max_size - min_size);
        vector<int> data(size);
        for (int j = 0; j < size; ++j)
        {
            data[j] = rand() % 10;
        }

        SegmentTree tree(data);
        int from = rand() % size;
        int to = rand() % size;
        if (from > to)
        {
            std::swap(from, to);
        }

        // Caclulate sum using segmented tree and brute force
        int s_tree = tree.sumRange(from, to);
        int s_brute_froce = 0;
        for (int i = from; i <= to; ++i) {
            s_brute_froce += data[i];
        }

        if (s_tree != s_brute_froce) {
            return false;
        }
    }

    return true;
}

TEST_CASE( "Test segmented tree", "[bit-test]" ) {
    REQUIRE(check(5, 2, 100));
}
