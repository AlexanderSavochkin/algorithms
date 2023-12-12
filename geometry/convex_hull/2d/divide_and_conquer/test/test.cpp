#include <algorithm>

#include "../../../../test/catch_amalgamated.hpp"
#include "../include/divide_and_conquer.hpp"

using std::sort;

vector<vec2d<double>> points_4_1 = { {1,1}, {2, 1.5}, {3,3}, {3, 1}  };
vector<int> idxs_4_1 = { 0, 1, 2, 3 };
auto convex_hull_4_1 = convex_hull_naive(
    points_4_1,
    idxs_4_1.begin(),
    idxs_4_1.end(),
    ComplexNumbersCrossProductFunctor()
);


TEST_CASE( "Naive convex hull algo works", "[naive]" ) {
    REQUIRE( convex_hull_4_1.size() == 3 );
    REQUIRE( convex_hull_4_1.count(0) == 1 );
    REQUIRE( convex_hull_4_1.count(1) == 0 );
    REQUIRE( convex_hull_4_1.count(2) == 1 );
    REQUIRE( convex_hull_4_1.count(3) == 1 );
}


//Test merge_convex_hulls
vector<vec2d<double>> points_8_1 = { {-3,0}, {-2.1, 1}, {-1.9,-1}, {-1, 0},
                                     {1, 0}, {1.9, 1}, {2.1, -1}, {3, 0} };
ConvexHullPointsIdx convex_hull_left = unordered_map<int, tuple<int, int>>{
    { 0, make_tuple(2, 1) },
    { 1, make_tuple(0, 3) },
    { 2, make_tuple(3, 0) },
    { 3, make_tuple(1, 2) }
};

ConvexHullPointsIdx convex_hull_right = unordered_map<int, tuple<int, int>>{
    { 4, make_tuple(6, 5) },
    { 5, make_tuple(4, 7) },
    { 6, make_tuple(7, 4) },
    { 7, make_tuple(5, 6) }
};

ConvexHullPointsIdx convex_hull_merged = merge_convex_hulls(
    points_8_1,
    convex_hull_left,
    convex_hull_right,
    ComplexNumbersCrossProductFunctor(),
    [](const vec2d<double>& v1, const vec2d<double>& v2) {
        return v1.real() < v2.real();
    }
);


TEST_CASE( "Merge convex hulls works", "[merge]" ) {
    REQUIRE(convex_hull_merged.size() == 6 );
    REQUIRE(convex_hull_merged.count(0) == 1 );
    REQUIRE(convex_hull_merged.count(1) == 1);
    REQUIRE(convex_hull_merged.count(2) == 1);
    REQUIRE(convex_hull_merged.count(3) == 0);
    REQUIRE(convex_hull_merged.count(4) == 0);
    REQUIRE(convex_hull_merged.count(5) == 1);
    REQUIRE(convex_hull_merged.count(6) == 1);
    REQUIRE(convex_hull_merged.count(7) == 1);
}


vector<vec2d<double>> points_17_1 = { 
    {0,0}, {1, -4}, {2, -1}, {4, -3}, {6, -2},
    {5, -1}, {7, 0}, {6, 2}, {3, 1}, {4, 3},
    {2, 3}, {1, 2}, {0, 3}, {-2, 2}, {-1, -1},
    {-2, 0}, {-2, -2}
};

vector<int> idxs_17_1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };

struct SideEffects {
    SideEffects() {
        sort(idxs_17_1.begin(), idxs_17_1.end(), [&](const int& i1, const int& i2) {
            return points_17_1[i1].real() < points_17_1[i2].real();
            });
	}
};

SideEffects side_effects;

auto convex_hull_17_1 = convex_hull_divide_and_coquer(
	points_17_1,
	idxs_17_1.cbegin(),
	idxs_17_1.cend(),
	ComplexNumbersCrossProductFunctor(),
    [](const vec2d<double>& v1, const vec2d<double>& v2) {
        return v1.real() < v2.real();
    }
);

TEST_CASE("End-to-end divide and conquer", "[convex_hull_divide_and_coquer]") {
    REQUIRE(convex_hull_17_1.count(1) == 1);
    REQUIRE(convex_hull_17_1.count(3) == 1);
    REQUIRE(convex_hull_17_1.count(4) == 1);
    REQUIRE(convex_hull_17_1.count(6) == 1);
    REQUIRE(convex_hull_17_1.count(7) == 1);

    REQUIRE(convex_hull_17_1.count(9) == 1);
    REQUIRE(convex_hull_17_1.count(12) == 1);
    REQUIRE(convex_hull_17_1.count(13) == 1);
    REQUIRE(convex_hull_17_1.count(16) == 1);
    REQUIRE(convex_hull_17_1.size() <= 11);

}