#pragma once

#include <istream>
#include <deque>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <variant>

using std::istream;
using std::deque;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::variant;

class Tokenizer
{
	deque<string> token_buffer;
	istream& input_stream;
	unordered_set<char> delimiters;
	const string empty_string = "";

	string read_next_token();
public:
	Tokenizer(istream& input_stream, unordered_set<char>& delimiters);
	Tokenizer(istream& input_stream, unordered_set<char>&& delimiters) noexcept;

	const string& get_token(unsigned offset = 0);
	bool advance(unsigned offset = 1);
};

enum class Function {
	Sin, Cos, Tan, Cot,
	Asin, Acos, Atan, Acot,
	Sqrt, Log, Exp
};

class Expression;

class Number
{
	const double value;
public:
	Number(double value, const unordered_map<string, double>& vars) : value(value) {};
	
	double eval(const unordered_map<string, double>& vars)  const { return value; };
};

class Variable
{
	const string name;
public:
	Variable(string name, const unordered_map<string, double>& vars) : name(name) {};
	double eval(const unordered_map<string, double>& vars)  const { return vars.at(name); };
};

class FunctionEvaluation
{
	Function function;
	Expression argument;
public:
	FunctionEvaluation(Function function) : function(function) {};
	double eval(const unordered_map<string, double>& vars) const;
};


class Expression
{
	vector<AdditionTerm> additions;
public:
	double eval(const unordered_map<string, double>& vars);
};

class AdditionTerm
{
	int sign; //+1 or -1
	variant<Multiplication, Function, >
public:

};