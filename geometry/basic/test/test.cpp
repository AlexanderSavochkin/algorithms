#include <algorithm>

#include "../../../test/catch_amalgamated.hpp"
#include "../include/basic_geometry.hpp"

const double Tolerance = 1e-7;

using std::abs;

vec2d<double> v1 = {0.0, 0.0};
vec2d<double> v2 = {1.0, 0.0};
vec2d<double> v3 = {0.0, 1.0};

pair<vec2d<double>, double> res1 = circumscribed_circle_2d<double, vec2d<double>>(v1, v2, v3);

TEST_CASE( "circumscribed_circle_2d", "[basic]" ) {
    REQUIRE((res1.first - vec2d(0.5, 0.5)).magnitude() < Tolerance);
}

vec2d<double> w1 = { 0.0, 1.0 };
vec2d<double> w2 = { -sqrt(3.0)/2.0, -0.5};
vec2d<double> w3 = { sqrt(3.0) / 2.0, -0.5 };

pair<vec2d<double>, double> res2 = circumscribed_circle_2d<double, vec2d<double>>(w1, w2, w3);

TEST_CASE("circumscribed_circle_2d", "[basic2]") {
    REQUIRE((res2.first).magnitude() < Tolerance);
}
