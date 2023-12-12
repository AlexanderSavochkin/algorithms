#pragma once

#include <vector>
#include <stack>
#include <algorithm>
#include <tuple>
#include <unordered_set>
#include <queue>

using std::vector;
using std::stack;
using std::sort;
using std::tuple;
using std::make_tuple;
using std::unordered_set;
using std::priority_queue;
using std::get;

namespace kdtree
{

template<
    typename TVector,
    typename TValue>
struct Sample {
    TVector location;
    TValue value; 
};

template<
    typename TCoord,
    typename TVector,
    typename TValue,
    int Ndim>
struct KdTreeNode 
{
    int dimenstion;
    TCoord threshold;
    Sample<TVector, TValue> sample;
    int less_branch_idx;
    int greater_branch_idx;

    static TCoord distance_square(const TVector& l, const TVector& r) {
        TCoord result = 0;
        for (int i = 0; i < Ndim; ++i) {
            result += (l[i] - r[i]) * (l[i] - r[i]);
        }
        return result;
    }
};

template<
    typename TCoord,
    typename TVector,    
    typename TValue,
    int Ndim>
class KdTree
{
    using Sample_ = Sample<TVector, TValue>;
    using KdTreeNode_ = KdTreeNode<TCoord, TVector, TValue, Ndim>;

    vector<KdTreeNode_> nodes;
public:
    void build(vector<Sample_>& samples)  //TODO: Current implementation reorders samples. Make stable version, using indices
    {
        stack<tuple<int, int, int, int>> st;
        int current_node_index = 0;
        st.push(make_tuple(0, samples.size(), current_node_index, 0));
        nodes.resize(samples.size());

        while (!st.empty()) {
            auto [from, to, node_idx, split_dimension] = st.top();
            st.pop();

            sort(
                samples.begin() + from,
                samples.begin() + to,
                [split_dimension](const Sample_& l, const Sample_& r) {
                    return l.location[split_dimension] < r.location[split_dimension];
                }               
            );

            int mid_point_indx = from + (to - from) / 2;

            int less_branch_idx = -1, greater_branch_idx = -1;

            if (mid_point_indx - from != 0) 
            {
                less_branch_idx = ++current_node_index;
                st.push(make_tuple(
                    from,
                    mid_point_indx,
                    less_branch_idx,
                    (split_dimension + 1) % Ndim));
            }

            if (to - mid_point_indx - 1 != 0)
            {
                greater_branch_idx = ++current_node_index;
                st.push(make_tuple(
                    mid_point_indx + 1,
                    to,
                    greater_branch_idx,
                    (split_dimension + 1) % Ndim));
            }

            nodes[node_idx] = KdTreeNode_ {
                split_dimension,
                samples[mid_point_indx].location[split_dimension],
                samples[mid_point_indx],
                less_branch_idx = less_branch_idx,
                greater_branch_idx = greater_branch_idx
            };
        }    
    }

    void find_k_nearest(
        const TVector &target_location,
        int k,
        vector<Sample_>& result)
    {
        int current_node_idx = 0;        
        stack<int> path_nodes_stack;
        unordered_set<int> visited_nodes;

        while (true) {
            path_nodes_stack.push(current_node_idx);
            auto& current_node = nodes[current_node_idx];
            if (target_location[current_node.dimenstion] < current_node.threshold) {
                if (current_node.less_branch_idx != -1) {
                    current_node_idx = current_node.less_branch_idx;
                } else {
                    break;
                }                 
            } 
            else {
                if (current_node.greater_branch_idx != -1) {
                    current_node_idx = current_node.greater_branch_idx;
                } else {
                    break;
                }
            }
        }

        auto comp = [](const tuple<TCoord, Sample_>& l, const tuple<TCoord, Sample_>& r) {
			return get<0>(l) < get<0>(r);
		};
        priority_queue<tuple<TCoord, Sample_>, vector<tuple<TCoord, Sample_>>, decltype(comp)> nearest_queue(comp);
        while (!path_nodes_stack.empty()) {
            int current_node_idx = path_nodes_stack.top();
            auto& current_node = nodes[current_node_idx];
            path_nodes_stack.pop();
            visited_nodes.insert(current_node_idx);

            if (nearest_queue.size() < k) {
                nearest_queue.push(make_tuple(
                    KdTreeNode_::distance_square(target_location, current_node.sample.location),
                    current_node.sample));
            } else {
                if (KdTreeNode_::distance_square(target_location, current_node.sample.location) < get<0>(nearest_queue.top())) {
                    nearest_queue.pop();
                    nearest_queue.push(make_tuple(
                        KdTreeNode_::distance_square(target_location, current_node.sample.location),
                        current_node.sample));
                }
            }

            TCoord d_target_coord_to_threshold = target_location[current_node.dimenstion] - current_node.threshold;
            if (d_target_coord_to_threshold * d_target_coord_to_threshold < get<0>(nearest_queue.top())) {

                if (current_node.greater_branch_idx != -1 && visited_nodes.find(current_node.greater_branch_idx) == visited_nodes.end()) {
                    path_nodes_stack.push(current_node.greater_branch_idx);
                }

                if (current_node.less_branch_idx != -1 && visited_nodes.find(current_node.less_branch_idx) == visited_nodes.end()) {
                    path_nodes_stack.push(current_node.less_branch_idx);
                }
            }
            else {
                if (
                        current_node.greater_branch_idx != -1 && 
                        visited_nodes.find(current_node.greater_branch_idx) == visited_nodes.end() &&
                        target_location[current_node.dimenstion] >= current_node.threshold) {
                    path_nodes_stack.push(current_node.greater_branch_idx);
                }

                if (    
                        current_node.less_branch_idx != -1 && 
                        visited_nodes.find(current_node.less_branch_idx) == visited_nodes.end() &&
                        target_location[current_node.dimenstion] < current_node.threshold) {
                    path_nodes_stack.push(current_node.less_branch_idx);
                }
            }
        }

        while (!nearest_queue.empty()) {
			result.push_back(
                get<1>(nearest_queue.top())
            );
			nearest_queue.pop();
		}

        reverse(result.begin(), result.end());
    }    
};

}