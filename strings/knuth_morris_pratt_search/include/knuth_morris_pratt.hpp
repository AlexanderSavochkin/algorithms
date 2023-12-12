#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

vector<int> kmp_table(const string& s) 
{
    vector<int> table(s.size());
    table[0] = 0;
    int k = 0;
    for (int i = 1; i < s.size(); ++i)
    {
        k = table[i - 1];
        while (k > 0 && s[i] != s[k])
        {
			k = table[k - 1];
		}
        if (s[i] == s[k])
        {
            k += 1;
        }
        table[i] = k;
    }

    return table;
}


vector<int> kmp_search(const string& text, const string& pattern)
{
	vector<int> result;
	vector<int> prefix = kmp_table(pattern);
	int k = 0;
    for (int i = 0; i < text.size(); ++i)
    {
        while (k > 0 && pattern[k] != text[i])
        {
			k = prefix[k - 1];
		}
        if (pattern[k] == text[i])
        {
			k += 1;
		}
        if (k == pattern.size())
        {
			result.push_back(i - pattern.size() + 1);
			k = prefix[k - 1];
		}
	}
	return result;
}
