#include <cstdlib>
#include <vector>
#include <istream>

#include "../../../test/catch_amalgamated.hpp"
#include "../include/math_parser.hpp"

using std::rand;
using std::vector;
using std::istringstream;

bool test_tokenizer1()
{
    string test_str = " skjdu   odfse deee";
    istringstream istrstr(test_str);
    Tokenizer tokenizer(istrstr, unordered_set{' '});

    const string& token1 = tokenizer.get_token();
    if (token1 != "skjdu")
        return false;

    const string& token2 = tokenizer.get_token(1);
    if (token2 != "odfse")
        return false;

    tokenizer.advance();

    const string& token3 = tokenizer.get_token(1);
    if (token3 != "deee")
        return false;

    if (!tokenizer.advance()) //Can move one token further
        return false;

    if (tokenizer.advance()) //Must reach end of the token strem, so advance must return false
        return false;

    return true;
}

bool test_tokenizer2()
{
    string test_str = "skjdu   odfse deee    ";
    istringstream istrstr(test_str);
    Tokenizer tokenizer(istrstr, unordered_set{ ' ' });

    const string& token1 = tokenizer.get_token();
    if (token1 != "skjdu")
        return false;

    const string& token2 = tokenizer.get_token(1);
    if (token2 != "odfse")
        return false;

    tokenizer.advance();

    const string& token3 = tokenizer.get_token(1);
    if (token3 != "deee")
        return false;

    if (!tokenizer.advance()) //Can move one token further
        return false;

    if (tokenizer.advance()) //Must reach end of the token strem, so advance must return false
        return false;

    return true;
}

TEST_CASE( "Test tokenizer", "[parse-test]" ) {
    REQUIRE(test_tokenizer1());
    REQUIRE(test_tokenizer2());
}
