#include "../include/math_parser.hpp"

#include <utility>
#include <istream>
#include <limits>

using std::move;
using std::istream;
using std::numeric_limits;

Tokenizer::Tokenizer(istream& input_stream, unordered_set<char>& delimiters) : input_stream(input_stream), delimiters(delimiters)
{
}

Tokenizer::Tokenizer(istream& input_stream, unordered_set<char> && delimiters) noexcept : input_stream(input_stream)
{
	this->delimiters = std::move(delimiters);
}

string Tokenizer::read_next_token()
{
	string token;
	char c;
	while (input_stream.get(c))
	{
		if (delimiters.find(c) != delimiters.end())
		{
			if (!token.empty())
			{
				token_buffer.push_back(token);
				return token;
			}
		}
		else
		{
			token += c;
		}
	}

	if (!token.empty())
	{
		token_buffer.push_back(token);
	}

	return token;
}


const string& Tokenizer::get_token(unsigned offset)
{
	string token;
	while (token_buffer.size() <= offset) {
		token = read_next_token();
		if (token.empty()) {
			return empty_string;
		}

	}
	return token_buffer[offset];
}


bool Tokenizer::advance(unsigned offset)
{
	for (int i = 0; i < offset; ++i) {
		if (!token_buffer.empty())
		{
			token_buffer.pop_front();
		}

		if (token_buffer.empty()) {
			const auto& token = read_next_token();
			if (token.empty()) {
				return false;
			}

			token_buffer.push_back(token);
		}
	}

	return true;
}



double FunctionEvaluation::eval(const unordered_map<string, double>& vars) const
{
	double x = argument.eval(vars);
	switch (function)
	{
	case Function::Sin:
		return sin(x);
	case Function::Cos:
		return cos(x);
	case Function::Tan:
		return tan(x);
	case Function::Cot:
		return numeric_limits<double>::quiet_NaN(); //Not implemented
	case Function::Asin:
		return asin(x);
	case Function::Acos:
		return acos(x);
	case Function::Atan:
		return atan(x);
	case Function::Acot:
		return numeric_limits<double>::quiet_NaN(); //Not implemented
	case Function::Sqrt:
		return sqrt(x);
	case Function::Log:
		return log(x);
	case Function::Exp:
		return exp(x);
	default:
	    return numeric_limits<double>::quiet_NaN(); //Unknown function
	}
}