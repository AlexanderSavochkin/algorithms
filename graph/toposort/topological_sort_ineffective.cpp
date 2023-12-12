#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

using std::vector;
using std::make_tuple;
using std::tuple;
using std::tie;

using std::unordered_map;
using std::unordered_set;

tuple<bool, vector<int>> kan_topo_sort_ineffective(
    unordered_map<int, unordered_set<int>>& incoming,
    unordered_map<int, unordered_set<int>>& outgoing)
{
    vector<int> result;

    while (incoming.size() != 0) {
        auto it = incoming.begin();
        bool found = false;
        for (;it != incoming.end(); ++it) {
            if (it->second.size() == 0) {
                found = true;
                break;
            }
        }

        if (!found) {
            return make_tuple(false, vector<int>);
        }

        //Clean incomings for all outgoings
        auto outgoing_for_verex = outgoing.at(it->first);
        for (auto oe : outgoing_for_verex)
        {
            incoming[oe].erase(it->first);
        }
        outgoing.erase(it->first);

        result.push_back(it->first);
    }

    return make_tuple(false, result);
}


int main()
{
    vector<tuple<int, int>> edges = {
        make_tuple(1,2),
        make_tuple(2,3),
        make_tuple(1,4),
        make_tuple(1,5),
        make_tuple(2,5),
        make_tuple(3,5),
        make_tuple(4,5)
    };

    unordered_map<int, unordered_set<int>>& incoming;
    unordered_map<int, unordered_set<int>>& outgoing;

    for (auto e: edges) {
        tie(from, to) = e;
        incoming[to].insert(from);
        outgoings[from].insert(to);
    }

    bool success;
    vector<int> sort_result;
    tie(success, sort_result) = kan_topo_sort_ineffective(
        incoming,
        outgoings);

    if (success) {
        cout << "Success:" << endl;
        for (auto v: result) {
            cout << v << ' ';
        }
        cout << endl;
    } else {
        cout << "Failed!" << endl;
    }
}