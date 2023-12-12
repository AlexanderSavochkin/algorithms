#include <vector>
#include <set>
#include <limits>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <iterator>

#include <iostream>

using std::vector;
using std::set;
using std::numeric_limits;
using std::tuple;
using std::make_tuple;
using std::get;
using std::unordered_multimap;
using std::make_pair;
using std::prev;

using std::cout;
using std::endl;

template<typename T>
struct Edge {
    T weight;
    int other_vertex_index;
};

//Forward (labelind distances from start)
//Primarily follows http://e-maxx.ru/algo/dijkstra_sparse
template<typename T>
void dijkstra_fwd(
    const vector<vector<Edge<T>>>& graph,
    int start,
    vector<T>& distances,
    vector<int>& previous) {
    distances[start] = T(); //Zero of type T
    set<tuple<T, int>> q; //Use std::set<T> as "updateable" priority queue
    q.insert(make_tuple(T(), start));

    while (!q.empty()) {
        int v = get<1>(*q.begin());
        q.erase(q.begin());

        for (size_t j = 0; j < graph[v].size(); ++j) {
            int to = graph[v][j].other_vertex_index;
            T len = graph[v][j].weight;
            if (distances[v] + len < distances[to]) {
                q.erase(make_tuple(distances[to], to));
                distances[to] = distances[v] + len;
                previous[to] = v;
                q.insert(make_tuple(distances[to], to));
            }
        }
    }
}

template<typename T>
vector<vector<Edge<T>>> revert_graph(
    const vector<vector<Edge<T>>>& graph) {
    unordered_multimap<int, Edge<T>> rev_graph_map;
    for (int i = 0; i < graph.size(); ++i) {
        for (auto edge_from_i : graph[i]) {
            rev_graph_map.insert(
                make_pair(
                    edge_from_i.other_vertex_index,
                    Edge<T> { edge_from_i.weight, i}));
        }
    }

    vector<vector<Edge<T>>> rev_graph(graph.size());
    for (int i = 0; i < graph.size(); ++i) {
        for (auto rev_graph_it = rev_graph_map.find(i);
            rev_graph_it != rev_graph_map.end() && rev_graph_it->first == i;
            ++rev_graph_it) {
            rev_graph[i].push_back(rev_graph_it->second);
        }
    }

    return rev_graph;
}

//Returns the best path from backtrack
template<typename T>
vector<int> backtrack(
        const vector<vector<Edge<T>>>& rev_graph,
        int end,
        vector<T>& distances,
        vector<int>& previous) {
    vector<int> result;
    int curr = end;
    while (curr != -1) {
        result.push_back(curr);
        curr = previous[curr];
    }
    reverse(result.begin(), result.end());
    return result;
}

vector<int> append(const vector<int> vec, int val) {
    vector<int> res(vec);
    res.push_back(val);
    return res;
}

template<typename T>
struct PathChunk
{
    T weight;
    vector<int> nodes;
//ublic:
    PathChunk(T weight, vector<int> nodes) : weight(weight), nodes(nodes) {}

    int head_vertex() const { return nodes[nodes.size() - 1]; }

    PathChunk<T> add_head(int v, T w) const {
        return PathChunk<T> {
            weight + w,
            append(nodes, v)
        };
    }

    bool contains_vertex(int v) const { return (std::find(nodes.begin(), nodes.end(), v) != nodes.end());}

    const vector<int>& get_nodes() const { return nodes; }
};

template<typename T>
class PathCompare
{
    const vector<T>& distances; //distances labeled during forward sweep
public:
    PathCompare(const vector<T>& distances) : distances(distances) {}
    bool operator() (const PathChunk<T>& lhs, const PathChunk<T>& rhs) const
    {
        return distances[lhs.nodes[lhs.nodes.size() - 1]] + lhs.weight  
                < distances[rhs.nodes[rhs.nodes.size() - 1]] + rhs.weight;
    }
};

//Returns k best paths from backtrack
template<typename T>
vector<PathChunk<T>> backtrack_kbest(
        const vector<vector<Edge<T>>>& rev_graph,
        int begin,
        int end,
        vector<T>& distances,
        int num_min_paths) {
    int paths_found = 0;
    vector<PathChunk<T>> result;
    PathCompare<T> comparer(distances);
    set<PathChunk<T>, PathCompare<T>> backtrack(comparer);
    backtrack.insert(PathChunk<T>(T(), vector<int> {end} ));
    while (backtrack.size() != 0) {
        PathChunk<T> shortest_chunk = *backtrack.begin(); //copy path array! :(
        backtrack.erase(backtrack.begin());
        for (Edge<T> back_edge : rev_graph[shortest_chunk.head_vertex()]) {
            if (shortest_chunk.contains_vertex(back_edge.other_vertex_index)) {
                continue;
            }

            auto new_chunk = shortest_chunk.add_head(back_edge.other_vertex_index, back_edge.weight);
            if (back_edge.other_vertex_index == begin) {
                result.push_back(new_chunk);
                ++paths_found;
            } else {
                backtrack.insert(new_chunk);
            }

            while (backtrack.size() > num_min_paths) {
                backtrack.erase(prev(backtrack.end()));
            }
        }
    }

    return result;
}


using EdgeD = Edge<double>;

int main() {
    vector<vector<EdgeD>> graph = {
        {EdgeD{1.0, 1}, EdgeD{2.0, 2}},
        {EdgeD{0.5, 2}, EdgeD{3.0, 3}},
        {/*EdgeD{0.5, 1},*/ EdgeD{0.5, 3}},
        {},
    };

    vector<vector<EdgeD>> rev_graph = revert_graph(graph);

    vector<double> d(4, numeric_limits<double>::max());
    vector<int> prev(4, -1);

    dijkstra_fwd(graph, 0, d, prev);

    auto path = backtrack(rev_graph, 3, d, prev);
    for (auto v: path) {
        cout << v << ' ';
    }
    cout << endl;

    auto kpaths = backtrack_kbest(rev_graph, 0, 3, d, 3); 

    return 0;
}