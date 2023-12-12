#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <utility>

#include <iostream>

using std::vector;
using std::generate;
using std::swap;
using std::min;

using std::endl;
using std::cout;

template<typename T>
void heapify(vector<T>& v, int i, int n) 
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest = i;
    if (l < n && v[l] > v[largest]) 
    {
        largest = l;
    }
    if (r < n && v[r] > v[largest])
    {
        largest = r;        
    }
    if (largest != i) 
    {
        swap(v[i], v[largest]);
        heapify(v, largest, n);
    }
}

template<typename T>
void heapify_up(vector<T>& v, int i)
{
    while (i > 0 && v[(i - 1) / 2] < v[i])
    {
        swap(v[(i - 1) / 2], v[i]);
        i = (i - 1) / 2;
    }
}

template<typename T>
void build_heap(vector<T>& v)
{
    for (int i = v.size() / 2; i >= 0; --i) {
        heapify(v, i, v.size());
    }
}

template<typename T>
void heapsort(vector<T>& v)
{
    build_heap(v);
    for (int i = v.size() - 1; i > 0; --i) {
        swap(v[0], v[i]);
        heapify(v, 0, i);
    }
}

template<typename T>
T get_largest(vector<T>& v)
{
    return v[0];
}

template<typename T>
void pop_largest(vector<T>& v, int n)
{
    swap(v[0], v[n - 1]);
    heapify(v, 0, n - 1);
}

template<typename T>
void add(T elem, vector<T>& v, int n)
{
    if (n >= v.size()) {
        v.resize(n + 1);
    }
    v[n]  = elem;
    heapify_up(v, n);
}



int main() {
    srand(time(nullptr));
    int size = 20 + (rand() % 60);
    vector<int> v(size);

    auto gen = [](){
                   return rand() % 100;
               };

    generate(v.begin(), v.end(), gen);

    heapsort(v);
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << ' ';
    }
    cout << endl;

    vector<int> u;
    int n = 0;

    add(7, u, n++);
    add(3, u, n++);
    add(9, u, n++);

    cout << "Expect 9: " << get_largest(u) << endl;
    pop_largest(u, n--);
    cout << "Expect 7: " << get_largest(u) << endl;
    pop_largest(u, n--);

    add(5, u, n++);
    cout << "Expect 5: " << get_largest(u) << endl;   
    pop_largest(u, n--);
    cout << "Expect 3: " << get_largest(u) << endl;
}