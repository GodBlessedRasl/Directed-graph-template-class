#include <iostream>
#include <vector>
#include <utility>
#include <list>
#include <string>
#include <set>
#include <fstream>
#include <string.h>
#include <sstream>
#include <queue>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

template <typename V = int, typename E = int>
struct Edge {
	V a_vertex;
	V b_vertex;
	E weight;
};

template<typename V = int, typename E = int>
class Graph {
public:
    list<Edge<V, E>> adj_;
    set<V> vertexes;

    // дефолтный конструктор
    Graph() = default;

    // конструктор с заданным списком ребер
    Graph(vector<Edge<V, E>> const &edges) {

        // добавляем ребра в ориентированный graph
        for (auto& it : edges)
        {
            int a = it.a_vertex;
            int b = it.b_vertex;
            int w = it.weight;
            adj_.push_back({a,b,w});
            vertexes.insert(a);
            vertexes.insert(b);
        }
    }

    //copy constructor
    Graph(Graph<V, E>& other) {
        for (auto it = other.adj_.begin(); it != other.adj_.end(); it++) {
            adj_.push_back(*it);
        }
        for (auto it = other.vertexes.begin(); it != other.vertexes.end(); it++) {
            vertexes.insert(*it);
        }
    }

    // move constructor
    Graph(Graph<V, E>&& other) noexcept {
        adj_.clear();
        vertexes.clear();
        for (auto it = other.adj_.begin(); it != other.adj_.end(); it++) {
            adj_.push_back(*it);
        }
        for (auto it = other.vertexes.begin(); it != other.vertexes.end(); it++) {
            vertexes.insert(*it);
        }
        other.adj_.clear();
        other.vertexes.clear();
    }

    //assignment operator
    Graph& operator = (const Graph<V, E>& other) {
        adj_.clear();
        vertexes.clear();
        for (auto it = other.adj_.begin(); it != other.adj_.end(); it++) {
            adj_.push_back(*it);
        }
        for (auto it = other.vertexes.begin(); it != other.vertexes.end(); it++) {
            vertexes.insert(*it);
        }
        return *this;
    }

    // move operator
    Graph& operator = (const Graph<V, E>&& other) {
        if (this != &other) {
            adj_.clear();
            vertexes.clear();
            for (auto it = other.adj_.begin(); it != other.adj_.end(); it++) {
                adj_.push_back(*it);
            }
            for (auto it = other.vertexes.begin(); it != other.vertexes.end(); it++) {
                vertexes.insert(*it);
            }
            other.adj_.clear();
            other.vertexes.clear();
        }
        return *this;
    }

    // is empty
    bool empty() {
        return vertexes.size() == 0;
    }

    // size
    size_t size() {
        return vertexes.size();
    }

    // clear
    void clear() {
        adj_.clear();
        vertexes.clear();
    }

    // swap
    void swap(Graph<V, E>& other) {
        Graph<V, E> tmp = *this;
        *this = other;
        other = tmp;
    }

    //begin
    auto begin() {
        return vertexes.begin();
    }

    //end
    auto end() {
        return vertexes.end();
    }

    //cbegin
    auto cbegin() const{
        return vertexes.cbegin();
    }

    //cend
    auto cend() const{
        return vertexes.cend();
    }

    // degree_in
    size_t degree_in(V key) {
        size_t cnt = 0;
        try {
            for (auto it = adj_.begin(); it != adj_.end(); ++it) {
                cnt += (*it).b_vertex == key;
            }
            if (!cnt) {
                std::exception error("key hasn't been found");
                throw error;
            }
        }
        catch (std::exception& err) {
            std::cout << err.what() << "\n";
        }
        return cnt;
    }

    // degree_out
    size_t degree_out(V key) {
        size_t cnt = 0;
        try {
            for (auto it = adj_.begin(); it != adj_.end(); ++it) {
                cnt += (*it).a_vertex == key;
            }
            if (!cnt) {
                std::exception error("key hasn't been found");
                throw error;
            }
        }
        catch (std::exception& err) {
            std::cout << err.what() << "\n";
        }
        return cnt;
    }

    // loop
    bool loop(V key) {
        bool ans = false;
        try {
            for (auto it = adj_.begin(); it != adj_.end(); ++it) {
                if ((*it).a_vertex == key && (*it).b_vertex == key) {
                    ans = true;
                    return true;
                }
            }
            if (vertexes.count(key)==0) {
                std::exception error("key hasn't been found");
                throw error;
            }
        }
        catch (std::exception& err) {
            std::cout << err.what() << "\n";
        }
        return ans;
    }

    //insert vertex
    auto insert_node(V node) {
        return vertexes.insert(node);
    }

    //insert edge
    auto insert_edge(V a_vertex, V b_vertex, E weight) {
        bool found = true;
        try {
            if (!vertexes.count(a_vertex) || !vertexes.count(b_vertex)) {
                found = false;
                std::exception error("not valid key");
                throw error;
            }
        }
        catch (std::exception& err) {
            std::cout << err.what() << "\n";
        }
        auto ans = make_pair(adj_.end(),found);
        adj_.push_back(Edge<V, E>{a_vertex, b_vertex, weight});
        return ans;
    }

    // insert or assign edge
    auto insert_or_assign_edge(V a_vertex, V b_vertex, E weight) {
        bool found = true;
        try {
            if (!vertexes.count(a_vertex) || !vertexes.count(b_vertex)) {
                found = false;
                std::exception error("not valid key");
                throw error;
            }
        }
        catch (std::exception& err) {
            std::cout << err.what() << "\n";
        }
        for (auto it = adj_.begin(); it != adj_.end(); ++it) {
            if ((*it).a_vertex == a_vertex && (*it).b_vertex == b_vertex) {
                (*it).weight = weight;
                return make_pair(it, found);
            }
        }
        auto ans = make_pair(adj_.end(), found);
        adj_.push_back(Edge<V, E>{a_vertex, b_vertex, weight});
        return ans;
    }
    

    // clear edges
    void clear_edges() {
        adj_.clear();
    }

    // erase edges go from
    bool erase_edges_go_from(V key) {
        if (!vertexes.count(key)) {
            return false;
        }
        for (auto it = adj_.begin(); it != adj_.end(); ++it) {
            if ((*it).a_vertex == key) {
                it = adj_.erase(it);
            }
        }
        return true;
    }

    // erase edges go to
    bool erase_edges_go_to(V key) {
        if (!vertexes.count(key)) {
            return false;
        }
        for (auto it = adj_.begin(); it != adj_.end(); ++it) {
            if ((*it).b_vertex == key) {
                it = adj_.erase(it);
            }
        }
        return true;
    }

    // erase_node
    bool erase_node(V key) {
        bool found = (vertexes.count(key) != 0);
        if (found) {
            vertexes.erase(key);
            for (auto it = adj_.begin(); it != adj_.end(); ++it) {
                if ((*it).a_vertex == key || (*it).b_vertex == key) {
                    it = adj_.erase(it);
                }
            }
        }
        return found;
    }
    // load from file 
    bool load_from_file(const string &path) {
        ifstream in(path);
        if (in.is_open()) {
            string line;
            int condition = 0;
            while (std::getline(in, line))
            {
                if (line == "Vertexes") {
                    condition = 1;
                    continue;
                }
                else if (line == "Edges:") {
                    condition = 2;
                    continue;
                }
                if (condition == 1) {
                    V value;
                    std::stringstream cast(line);
                    cast >> value;
                    vertexes.insert(value);
                }
                else if (condition == 2) {
                    vector<string>items;
                    for (int i = 0; i < line.size(); ++i) {
                        string current = "";
                        while (i < line.size() && line[i] != ' ') {
                            current += line[i];
                            ++i;
                        }
                        items.push_back(current);
                    }
                    V a_vertex, b_vertex;
                    E weight;
                    std::stringstream cast(items[0]);
                    cast >> a_vertex;
                    std::stringstream cast1(items[1]);
                    cast1 >> b_vertex;
                    std::stringstream cast2(items[2]);
                    cast >> weight;
                    
                    adj_.push_back(Edge<V, E>{a_vertex, b_vertex, weight });
                }
            }

            in.close();
            return true;
        }
        else {
            return false;
        }
    }

    // safe to file
    void save_to_file(const string &path) {
        ofstream out(path);
        if (out.is_open()) {

            out << "Vertexes:\n";
            for (auto &it : vertexes) {
                out << it << "\n";
            }

            out << "Edges:\n";
            for (auto it = adj_.begin(); it != adj_.end(); ++it) {
                out << (*it).a_vertex << " " <<  (*it).b_vertex << " " <<  (*it).weight << "\n";
            }
        }
        out.close();
    }

    void dfs(V vertex, set<V> used = {}) {
        used.insert(vertex);
        cout << "Visited " << vertex << "\n";
        vector<V>children;
        for (auto it = adj_.begin(); it != adj_.end(); ++it) {
            if ((*it).a_vertex == vertex) {
                children.push_back((*it).b_vertex);
            }
        }
        for (auto &child : children) {
            if (used.count(child) == 0) {
                dfs(child, used);
            }  
        }
    }

    void bfs(V vertex, set<V> used = {}) {
        queue<V> q;
        q.push(vertex);
        used.insert(vertex);
        while (!q.empty()) {
            V current = q.front();
            cout << "Visited " << current << "\n";
            q.pop();
            vector<V>children;
            for (auto it = adj_.begin(); it != adj_.end(); ++it) {
                if ((*it).a_vertex == current) {
                    children.push_back((*it).b_vertex);
                }
            }
            for (auto& child : children) {
                if (used.count(child) == 0) {
                    q.push(child);
                }
            }
        }
    }

    void print() {
        for (auto& it : adj_) {
            cout << it.a_vertex << " " << it.b_vertex << "\n";
        }
    }
};

// swap
template<typename V = int, typename E = int>
void swap(Graph<V, E>& a, Graph<V, E>& b) {
    Graph<V, E> tmp = a;
    a = b;
    b = tmp;
}

int main() {
    vector<Edge<int,int>> edges( { {0, 1, 6}, {1, 2, 7}, {2, 0, 5}, {2, 1, 4}, {3, 2, 10}, {5, 4, 1}, {4, 5, 3} });
    vector<Edge<int, int>> edges2({ {1, 2, 5}, {1, 3, 4}, {2, 5, 10}, {2, 4, 10} });

    // -------- testing constructors and operators -----------
    cout << "testing constructors and operators\n";
    Graph<int, int> g(edges);
    Graph<int, int> g2(edges2);
    Graph<int, int>gg(g);
    gg = g;
    // -------------------------------------------------------
    // -------------- testing basic interface ----------------
    cout << "\ntesting basic interface\n";
    gg.print();
    cout << gg.size() << "\n";
    cout << gg.empty() << "\n";
    swap(g, g2); 
    g.print();
    g2.clear();
    // -------------------------------------------------------
    // -------------- testing iterating tools ----------------  
    cout << "\ntesting iterating tools\n";
    cout << *(g.begin()) << "\n";
    cout << *(g.cbegin()) << "\n";
    for (auto it = g.begin(); it != g.end(); ++it) {
        cout << *it << " ";
    }
    cout << "\n";
    // -------------------------------------------------------
     // -------------- testing graph info tools --------------
    cout << "\ntesting graph info tools\n";
    cout << g.degree_in(2) << "\n";
    cout << g.degree_out(2) << "\n";
    cout << g.loop(2) << "\n";

    g2.insert_node(8);
    g2.insert_node(1);
    g2.insert_edge(8, 1, 10);
    g2.insert_or_assign_edge(8, 1, 11);
    g2.print();

    g2.clear_edges();
    g2.print();
    // --------------------------------------------------------
    // -------------- testing graph uploading -----------------
    cout << "\ntesting graph uploading\n";
    g.save_to_file("GraphData.txt");
    Graph<int, int>g3;
    g3.load_from_file("GraphData.txt");
    g3.print();  
    //---------------------------------------------------------
    // -------------- testing dfs/bfs -------------------------
    cout << "\ndfs log:\n";
    g3.dfs(1);
    cout << "\nbfs log:\n";
    g3.bfs(1);
   // ---------------------------------------------------------
}