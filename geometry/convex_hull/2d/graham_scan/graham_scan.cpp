#include <vector>
#include <stack>
#include <complex>
#include <algorithm>
#include <limits>

#include <iostream>


using std::vector;
using std::stack;
using std::sort;
using std::numeric_limits;


template<typename T>
using vec2d = std::complex<T>;


template<
    typename Tv,
    typename TCompTv,
    typename TCrossProdTv>
vector<int> convex_hull_graham_scan(const std::vector<Tv>& pts, TCompTv x_comparer, TCrossProdTv cross_prod_comparer)
{
    vector<int> indices(pts.size());
    for (auto i = 0; i < pts.size(); ++i)
    {
        indices[i] = i;
    }

    sort(indices.begin(), indices.end(), x_comparer);

    // lower hull
    stack<int> lower_hull;
    for (auto i = 0; i < indices.size(); ++i)
    {
        while (lower_hull.size() >= 2)
        {
            auto i1 = lower_hull.top();
            lower_hull.pop();
            auto i2 = lower_hull.top();
            if (cross_prod_comparer(pts[i2], pts[i1], pts[indices[i]]))
            {
                lower_hull.push(i1);
                break;
            }
        }
        lower_hull.push(indices[i]);
    }

    // upper hull
    stack<int> upper_hull;
    //Copilot introduced interesting bug here: it put size_t i as a loop variable,
    //but size_t is never negative so 
    //the loop went beyond zero (jumped to max value of size_t) and caused 
    //accessing array out of bounds.
    for (size_t i = pts.size() - 1; i != numeric_limits<size_t>::max(); --i)
    {
        while (upper_hull.size() >= 2)
        {
            auto i1 = upper_hull.top();
            upper_hull.pop();
            auto i2 = upper_hull.top();
            if (cross_prod_comparer(pts[i2], pts[i1], pts[indices[i]]))
            {
                upper_hull.push(i1);
                break;
            }
        }
        upper_hull.push(indices[i]);
    }

    // remove the last element of each stack
    lower_hull.pop();
    upper_hull.pop();

    // merge the two stacks
    while (!upper_hull.empty())
    {
        lower_hull.push(upper_hull.top());
        upper_hull.pop();
    }

    // copy the stack to a vector
    vector<int> hull;
    while (!lower_hull.empty())
    {
        hull.push_back(lower_hull.top());
        lower_hull.pop();
    }

    return hull;
}


int main() 
{
    std::vector<vec2d<double>> pts = { {2, 1.5}, {3, 3}, {1, 1}, {3, 1} };
    auto res = convex_hull_graham_scan(
        pts,
        [&pts](const int a, const int b) -> bool
        {
            return pts[a].real() < pts[b].real();
        },
        [](const vec2d<double>& a, const vec2d<double>& b, const vec2d<double>& c) -> bool
        {
            return (b - a).imag() * (c - a).real() - (b - a).real() * (c - a).imag() > 0;
        });

    // print the result
    for (auto i : res)
    {
        std::cout << pts[i].real() << ", " << pts[i].imag() << std::endl;
    }

    return 0;
}
