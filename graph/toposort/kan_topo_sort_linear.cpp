#include <vector>
#include <queue>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

struct Graph {
    vector<vector<int>> adjacent;
};

bool topo_sort(Graph& graph, vector<int>& order) {
    vector<int> in_degree;
    std::queue<int> q;

    //Calculate in_degree
    in_degree.resize(graph.adjacent.size(), 0);
    for (int i = 0; i < graph.adjacent.size(); ++i) {
        for (int j = 0; j < graph.adjacent[i].size(); ++j) {
            ++in_degree[graph.adjacent[i][j]];
        }
    }

    for (int i = 0; i < in_degree.size(); ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);
        for (auto next: graph.adjacent[node]) {
            --in_degree[next];
            if (in_degree[next] == 0) {
                q.push(next);
            }
        }
    }

    if (order.size() == graph.adjacent.size()) {
        return true;
    }

    return false;
}

//Test
int main() 
{
    Graph graph;
    graph.adjacent.resize(6);
    graph.adjacent[0].push_back(1);
    graph.adjacent[0].push_back(2);
    graph.adjacent[1].push_back(3);
    graph.adjacent[2].push_back(3);
    graph.adjacent[3].push_back(4);
    graph.adjacent[4].push_back(5);

    vector<int> order;
    bool can_sort = topo_sort(graph, order);

    cout << (can_sort ? "True" : "Fasle") << endl;
    for (auto i: order) {
        cout << i << ' ';
    }
    cout << endl;
}