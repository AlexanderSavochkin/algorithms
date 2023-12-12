#pragma once

#include <vector>
#include <stack>
#include <complex>
#include <algorithm>
#include <limits>
#include <unordered_map>
#include <tuple>

#include <iostream>


using std::vector;
using std::stack;
using std::sort;
using std::numeric_limits;
using std::unordered_map;
using std::tuple;
using std::make_tuple;
using std::get;
using std::make_tuple;

template<typename T>
using vec2d = std::complex<T>;

class ComplexNumbersCrossProductFunctor 
{
public:
    template<typename T>
    T operator()(const vec2d<T>& v1, const vec2d<T>& v2) const
    {
        return v1.real() * v2.imag() - v1.imag() * v2.real();
    }
};

using ConvexHullPointsIdx = unordered_map<int, tuple<int, int>>;


//Check each pair of points and if all the other points are on the same side  
//of the line, then the pair of points is part of the convex hull.
//Time complexity: O(n^3)
//Space complexity: O(n)
//n - number of points
//Returns a map from point tuple of pair of following point indices in clockwise (first element of the tuple)
// and counter-clockwise order (2nd element of the tuple)
template<typename TCrossProductFunctor>
ConvexHullPointsIdx convex_hull_naive(
    const vector<vec2d<double>>& pts,
    vector<int>::const_iterator begin,
    vector<int>::const_iterator end,
    TCrossProductFunctor cross_prod_func)
{
    //Map from point index to the index of the following point  
    //in the convex hull according to counter-clockwise order
    unordered_map<int, int> following_point_index;
    for (auto i = begin; i != end; ++i) {
        for (auto j = i + 1; j != end; ++j) {
            bool is_on_left = true;
            bool is_on_right = true;

            for (auto k = begin; k != end; ++k) {
                if (k == i || k == j) {
                    continue;
                }

                auto v1 = pts[*j] - pts[*i];
                auto v2 = pts[*k] - pts[*i];

                auto cross_prod = cross_prod_func(v1, v2);

                if (cross_prod > 0) {
                    is_on_right = false;
                }
                else if (cross_prod < 0) {
                    is_on_left = false;
                }
            }

            if (is_on_left || is_on_right) {
                if (is_on_right) {
                    following_point_index[*i] = *j;
                }
                else {
                    following_point_index[*j] = *i;
                }
            }
        }
    }

    unordered_map<int, tuple<int, int>> convex_hull;
    int curr_point = following_point_index.begin()->first;
    while (convex_hull.count(following_point_index[curr_point]) == 0) {
        convex_hull[following_point_index[curr_point]] = make_tuple(
            curr_point,
            following_point_index[following_point_index[curr_point]]);

        curr_point = following_point_index[curr_point];
    }

    return convex_hull;
}


//Merging two convex hulls
template<typename TCrossProductFunctor, typename TCompTv>
ConvexHullPointsIdx merge_convex_hulls(
    const vector<vec2d<double>>& pts,
    const ConvexHullPointsIdx& left_hull,
    const ConvexHullPointsIdx& right_hull,
    TCrossProductFunctor cross_prod_func,
    TCompTv less_x_comparer
    )
{
    //Find the point in the left hull that is furthest to the right
    int left_hull_max_x_idx = left_hull.begin()->first;
    for (const auto& [idx, _] : left_hull) {
        if (less_x_comparer(pts[left_hull_max_x_idx],  pts[idx]) ) {
            left_hull_max_x_idx = idx;
        }
    }

    //Find the point in the right hull that is furthest to the left
    int right_hull_min_x_idx = right_hull.begin()->first;
    for (const auto& [idx, _] : right_hull) {
        if (less_x_comparer(pts[idx], pts[right_hull_min_x_idx])) {
            right_hull_min_x_idx = idx;
        }
    }

    int left_hull_upper_tangent_idx = left_hull_max_x_idx;
    int left_hull_lower_tangent_idx = left_hull_max_x_idx;
    int right_hull_upper_tangent_idx = right_hull_min_x_idx;
    int right_hull_lower_tangent_idx = right_hull_min_x_idx;

    //Find the upper tangent of the two hulls
    while (true) {
        auto v_left_right = pts[right_hull_upper_tangent_idx] - pts[left_hull_upper_tangent_idx];
        int next_left_hull_upper_tangent_idx_candidate = get<0>(left_hull.at(left_hull_upper_tangent_idx));
        int next_right_hull_upper_tangent_idx_candidate = get<1>(right_hull.at(right_hull_upper_tangent_idx));
        
        auto v_left_next_counter_clockwise = pts[next_left_hull_upper_tangent_idx_candidate] - pts[left_hull_upper_tangent_idx];
        auto v_right_next_clockwise = pts[next_right_hull_upper_tangent_idx_candidate] - pts[right_hull_upper_tangent_idx];

        //Check if we found the upper tangent
        if (cross_prod_func(v_left_right, v_left_next_counter_clockwise) < 0 &&
            cross_prod_func(v_left_right, v_right_next_clockwise) < 0) {
            break;
        }

        //Update the upper tangent
        if (cross_prod_func(v_left_right, v_left_next_counter_clockwise) > 0) {
            left_hull_upper_tangent_idx = next_left_hull_upper_tangent_idx_candidate;
        }
        else {
            right_hull_upper_tangent_idx = next_right_hull_upper_tangent_idx_candidate;
        }
    }

    //Find the lower tangent of the two hulls
    while (true)    
    {
        auto v_left_right = pts[right_hull_lower_tangent_idx] - pts[left_hull_lower_tangent_idx];
        int next_left_hull_lower_tangent_idx_candidate = get<1>(left_hull.at(left_hull_lower_tangent_idx));
        int next_right_hull_lower_tangent_idx_candidate = get<0>(right_hull.at(right_hull_lower_tangent_idx));

        auto v_left_next_clockwise = pts[next_left_hull_lower_tangent_idx_candidate] - pts[left_hull_lower_tangent_idx];
        auto v_right_next_counter_clockwise = pts[next_right_hull_lower_tangent_idx_candidate] - pts[right_hull_lower_tangent_idx];

        //Check if we found the lower tangent
        if (cross_prod_func(v_left_right, v_left_next_clockwise) > 0 &&
            cross_prod_func(v_left_right, v_right_next_counter_clockwise) > 0) {
            break;
        }

        //Update the lower tangent
        if (cross_prod_func(v_left_right, v_left_next_clockwise) < 0) {
            left_hull_lower_tangent_idx = next_left_hull_lower_tangent_idx_candidate;
        }
        else {
            right_hull_lower_tangent_idx = next_right_hull_lower_tangent_idx_candidate;
        }
    }
    
    //Merge the two hulls
    ConvexHullPointsIdx convex_hull;

    //Stich the upper tangent
    convex_hull[left_hull_upper_tangent_idx] = make_tuple(get<0>(left_hull.at(left_hull_upper_tangent_idx)), right_hull_upper_tangent_idx);
    convex_hull[right_hull_upper_tangent_idx] = make_tuple(left_hull_upper_tangent_idx, get<1>(right_hull.at(right_hull_upper_tangent_idx)));

    //Stich the lower tangent
    convex_hull[left_hull_lower_tangent_idx] = make_tuple(right_hull_lower_tangent_idx, get<1>(left_hull.at(left_hull_lower_tangent_idx)));
    convex_hull[right_hull_lower_tangent_idx] = make_tuple(get<0>(right_hull.at(right_hull_lower_tangent_idx)), left_hull_lower_tangent_idx);

    //Copy all left hull elements between the upper and lower tangent
    int left_hull_upper_tangent_idx_next = get<0>(left_hull.at(left_hull_upper_tangent_idx));
    while (left_hull_upper_tangent_idx_next != left_hull_lower_tangent_idx) {
        convex_hull[left_hull_upper_tangent_idx_next] = left_hull.at(left_hull_upper_tangent_idx_next);
        left_hull_upper_tangent_idx_next = get<0>(left_hull.at(left_hull_upper_tangent_idx_next));
    }

    //Copy all right hull elements between the lower and the upper tangents
    int right_hull_lower_tangent_idx_next = get<0>(right_hull.at(right_hull_lower_tangent_idx));
    while (right_hull_lower_tangent_idx_next != right_hull_upper_tangent_idx) {
        convex_hull[right_hull_lower_tangent_idx_next] = right_hull.at(right_hull_lower_tangent_idx_next);
        right_hull_lower_tangent_idx_next = get<0>(right_hull.at(right_hull_lower_tangent_idx_next));
    }

    return convex_hull;
}


 template<
     typename TCrossProductFunctor,
     typename TCompTv>
 ConvexHullPointsIdx convex_hull_divide_and_coquer(
     const vector<vec2d<double>>& pts,
     vector<int>::const_iterator begin,
     vector<int>::const_iterator end,
     TCrossProductFunctor cross_prod_func,
     TCompTv less_x_comparer)
 {
     ////Helps to debug
     //vector<int> debug_idx;
     //copy(begin, end, back_inserter(debug_idx));

     if (end - begin <= 6) {
         return convex_hull_naive(
            pts,
            begin,
            end,
            cross_prod_func);
     }

     int mid = (end - begin) / 2;

     auto left_hull = convex_hull_divide_and_coquer(pts, begin, begin + mid, cross_prod_func, less_x_comparer);
     auto right_hull = convex_hull_divide_and_coquer(pts, begin + mid, end, cross_prod_func, less_x_comparer);

     auto convex_hull = merge_convex_hulls(pts, left_hull, right_hull, cross_prod_func, less_x_comparer);

     return convex_hull;
 }

