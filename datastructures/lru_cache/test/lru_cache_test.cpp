#include "../../../test/catch_amalgamated.hpp"
#include "../include/lru_cache.hpp"


TEST_CASE("lru_cache")
{
	lru_cache<int, int> cache(2);
	cache.put(1, 1);
	cache.put(2, 2);
	REQUIRE(cache.get(1).value() == 1);
	cache.put(3, 3);
	REQUIRE(cache.get(2).has_value() == false);
	cache.put(4, 4);
	REQUIRE(cache.get(1).has_value() == false);
	REQUIRE(cache.get(3).value() == 3);
	REQUIRE(cache.get(4).value() == 4);
	REQUIRE(cache.size() == 2);
}