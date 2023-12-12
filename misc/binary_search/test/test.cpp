#include <vector>

#include "../include/binary_search.hpp "
#include "../../../test/catch_amalgamated.hpp"


bool check_found_1_elem() {
    vector<int> data { 42 };
    int found_idx = binary_search(data, 42);
    return found_idx == 0;
}

bool check_found_2_elem() {
    vector<int> data { 13, 42 };
    int found_idx = binary_search(data, 42);
    return found_idx == 1;
}

bool check_not_found_1_elem() {
    vector<int> data { 11 };
    int found_idx = binary_search(data, 54);
    return found_idx == -1;
}

bool check_not_found_2_elem() {
    vector<int> data { 13, 42 };
    int found_index = binary_search(data, 54);
    return found_index == -1;
}

bool check_not_found(int iter) {
    const int RANGE = 1000;
    const int MIN_SIZE = 1, MAX_SIZE = 1000;
    for (int i = 0; i < iter; ++i) {    
        int size = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE);
        vector<int> data(size);
        for (int j = 0; j < size; ++j)
        {
            data[j] = rand() % RANGE;
        }

        int target = rand() % RANGE;
        int right = size - 1;
        while (data[right] == target)
            right--;
        for (int i = 0; i < right; ++i) {
            if (data[i] == target) {
                swap(data[i], data[right]);
                while (data[right] == target)
                    right--;
            }            
        }

        data.resize(right + 1);

        sort(data.begin(), data.end());
        int found_index = binary_search(data, target);
        if (found_index != -1) {
            return false;
        }
    }

    return true;
}

bool check_found(int iter) {
    const int RANGE = 1000;
    const int MIN_SIZE = 1, MAX_SIZE = 1000;
    for (int i = 0; i < iter; ++i) {    
        int size = MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE);
        vector<int> data(size);
        for (int j = 0; j < size; ++j)
        {
            data[j] = rand() % RANGE;
        }

        int target = rand() % RANGE;
        data.push_back(target);

        sort(data.begin(), data.end());
        int found_index = binary_search(data, target);
        if (found_index == -1 || data[found_index] != target)
            return false;
    }

    return true;
}

TEST_CASE( "Test 1 element data (found)", "[binary serach test]" ) {
    REQUIRE(check_found_1_elem());
}

TEST_CASE( "Test 2 element data (found)", "[binary serach test]" ) {
    REQUIRE(check_found_2_elem());
}


TEST_CASE( "Test 1 element data (not found)", "[binary serach test]" ) {
    REQUIRE(check_not_found_1_elem());
}

TEST_CASE( "Test 2 element data (not found)", "[binary serach test]" ) {
    REQUIRE(check_not_found_2_elem());
}

TEST_CASE( "Test random data (not found)", "[binary serach test]" ) {
    REQUIRE(check_not_found(10));
}

TEST_CASE( "Test random data (found)", "[binary serach test]" ) {
    REQUIRE(check_found(10));
}