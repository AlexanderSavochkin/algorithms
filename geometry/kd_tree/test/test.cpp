#include <cstdlib>
#include <vector>
#include <array>


#include "../../../test/catch_amalgamated.hpp"
#include "../include/kd_tree.hpp"

using std::rand;
using std::vector;
using std::array;

using kdtree::KdTree;
using kdtree::Sample;


double distance(
    const array<double, 3>& a,
    const array<double, 3>& b
) {
    double s = 0.0;
    for (int i = 0; i < 3; ++i) {
        double d = (a[i] - b[i]);
        s += d * d; 
    }

    return s;
}

bool check1()
{
    KdTree<double, array<double, 2>, int, 2> kd_tree;
    vector<Sample<array<double, 2>, int>> data;

    data.push_back(Sample<array<double, 2>, int>{ { 0.0, 0.0}, 0});
    data.push_back(Sample<array<double, 2>, int>{ { -1.0, -0.5 }, 1});
    data.push_back(Sample<array<double, 2>, int>{ { 1.0, 0.5 }, 2});
    data.push_back(Sample<array<double, 2>, int>{ { -1.5, 1.0 }, 3});
    data.push_back(Sample<array<double, 2>, int>{ { 1.5, -1.0 }, 4});

    kd_tree.build(data);

    array<double, 2> target{ -2.0, 0.5 };
    vector<Sample<array<double, 2>, int>> result;

    kd_tree.find_k_nearest(target, 1, result);

    if (result.size() != 1) {
		return false;
	}

    if (result[0].value != 3) {
        return false;
    }

    result.resize(0);
    kd_tree.find_k_nearest(target, 2, result);

    if (result.size() != 2) {
        return false;
    }

    if (result[0].value != 3) {
        return false;
    }

    if (result[1].value != 1) {
        return false;
    }

    result.resize(0);
    array<double, 2> target2{ 0.25, 0.5 };
    kd_tree.find_k_nearest(target2, 1, result);

    if (result.size() != 1) {
        return false;
    }

    if (result[0].value != 0) {
        return false;
    }


    return true;
}

bool check_randomized(
        int num_iter,
        int num_sub_iter,
        int min_size,
        int max_size) 
{
    for (int i = 0; i < num_iter; ++i) 
    {

        KdTree<double, array<double, 3>, int, 3> kd_tree;
        int size = min_size + rand() % (max_size - min_size);

        vector<Sample<array<double, 3>, int>> data(size);
        for (int j = 0; j < size; ++j)
        {
            array<double, 3> loc = {
                1.0e-4 * (rand() % 20000),
                1.0e-4 * (rand() % 20000),
                1.0e-4 * (rand() % 20000)
            };

            data[j] = Sample<array<double, 3>, int>(loc, j);
        }

        kd_tree.build(data);

        for (int k = 0; k < num_sub_iter; ++k)
        {
            vector<Sample<array<double, 3>, int>> result;
            int kk = 1 + rand() % (size / 2);

            array<double, 3> target_location = {
                1.0e-4 * (rand() % 20000),
                1.0e-4 * (rand() % 20000),
                1.0e-4 * (rand() % 20000)
            };

            kd_tree.find_k_nearest(target_location, kk, result);
 
            //Compare with results obtained via sorting
            sort(
                data.begin(),
                data.end(),
                [&target_location] (const Sample<array<double, 3>, int>& a, const Sample<array<double, 3>, int>& b) {
                    return distance(target_location, a.location) < distance(target_location, b.location);
                });

            if (result.size() != kk)
                return false;

            unordered_set<int> found_via_kd_tree;
            for (auto r: result) {
                found_via_kd_tree.insert(r.value);
            }

            for (int l = 0; l < kk; ++l) {
                if (found_via_kd_tree.count(data[l].value) == 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

TEST_CASE( "Test KD tree", "[kd_tree-test]" ) {
    REQUIRE(check1());
    REQUIRE(check_randomized(5, 3, 10, 100));
}
