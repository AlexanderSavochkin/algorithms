#include <cstdlib>
#include <vector>


#include "../../../test/catch_amalgamated.hpp"
#include "../include/quicksort.hpp"

using std::rand;
using std::vector;

/*
Generate num_iter random arrays of size between min_size and max_size and check quick sort on them
*/
bool check_quick_sort(int num_iter, int min_size, int max_size)
{
    for (int i = 0; i < num_iter; ++i) 
    {
        int size = min_size + rand() % (max_size - min_size);
        vector<int> data(size);
        for (int j = 0; j < size; ++j)
        {
            data[j] = rand() % 10;
        }

        my_quick_sort(data);

        for (int j = 0; j < data.size() - 1; ++j) {
            if (data[j + 1] < data[j])
                return false;
        }
    }

    return true;
}


/*
Generate num_iter random arrays of size between min_size and max_size and check quick select on them
*/
bool check_quick_select(int num_iter, int min_size, int max_size)
{
    for (int i = 0; i < num_iter; ++i) 
    {
        int size = min_size + rand() % (max_size - min_size);
        vector<int> data(size);
        for (int j = 0; j < size; ++j)
        {
            data[j] = rand() % 10;
        }

        int k = min_size + rand() % (data.size() - min_size) - 1;

        my_quick_select(data, k);

        for (int j = 0; j < k; ++j) {
            for (int l = k; l < data.size(); ++l) {
               if (data[j] > data[l])
                   return false;
           }
        }
    }

    return true;
}


TEST_CASE( "Test quick sort", "[qsort]" ) {
    REQUIRE(check_quick_sort(5, 5, 100));
}

TEST_CASE( "Test quick select", "[qselect]" ) {
    REQUIRE(check_quick_select(5, 5, 100));
}