// Combined C++ Delhi Metro App with Graph and Heap classes
#include<bits/stdc++.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <limits>
#include <algorithm>
#include <functional>
#include <sstream>
using namespace std;

// ------------------ Heap Class ------------------
template <typename T>
class Heap {
public:
    vector<T> data;
    unordered_map<T, int> map;

    void add(T item) {
        data.push_back(item);
        map[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    void upheapify(int ci) {
        int pi = (ci - 1) / 2;
        if (ci > 0 && isLarger(data[ci], data[pi]) > 0) {
            swap(ci, pi);
            upheapify(pi);
        }
    }

    void swap(int i, int j) {
        T temp = data[i];
        data[i] = data[j];
        data[j] = temp;
        map[data[i]] = i;
        map[data[j]] = j;
    }

    int size() const {
        return data.size();
    }

    bool isEmpty() const {
        return data.empty();
    }

    T remove() {
        swap(0, data.size() - 1);
        T removed = data.back();
        data.pop_back();
        map.erase(removed);
        downheapify(0);
        return removed;
    }

    void downheapify(int pi) {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int maxi = pi;

        if (lci < size() && isLarger(data[lci], data[maxi]) > 0) maxi = lci;
        if (rci < size() && isLarger(data[rci], data[maxi]) > 0) maxi = rci;

        if (maxi != pi) {
            swap(pi, maxi);
            downheapify(maxi);
        }
    }

    T get() const {
        return data[0];
    }

    int isLarger(const T& a, const T& b) const {
        return a > b ? 1 : (a < b ? -1 : 0);
    }

    void updatePriority(const T& item) {
        if (map.find(item) != map.end()) {
            int index = map[item];
            upheapify(index);
        }
    }
};

// ------------------ Graph Class ------------------
class Graph {
public:
    struct Vertex {
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

    void addVertex(string vname) {
        vtces[vname] = Vertex();
    }

    void addEdge(string vname1, string vname2, int cost) {
        if (!containsVertex(vname1) || !containsVertex(vname2)) return;
        vtces[vname1].nbrs[vname2] = cost;
        vtces[vname2].nbrs[vname1] = cost;
    }

    bool containsVertex(string vname) {
        return vtces.find(vname) != vtces.end();
    }

    void displayStations() {
        int i = 1;
        for (auto &v : vtces) {
            cout << i++ << ". " << v.first << endl;
        }
    }

    void displayMap() {
        cout << "\t Delhi Metro Map\n\t------------------\n";
        for (auto &p : vtces) {
            cout << p.first << " =>\n";
            for (auto &nbr : p.second.nbrs) {
                cout << "\t" << nbr.first << "\t" << nbr.second << endl;
            }
        }
    }

    int dijkstra(string src, string dest, bool use_time) {
        unordered_map<string, int> dist;
        set<pair<int, string>> pq;

        for (auto &v : vtces) {
            dist[v.first] = INT_MAX;
        }

        dist[src] = 0;
        pq.insert({0, src});

        while (!pq.empty()) {
            auto top = *pq.begin();
            pq.erase(pq.begin());

            string node = top.second;
            int d = top.first;

            if (node == dest) return d;

            for (auto &nbr : vtces[node].nbrs) {
                int weight = nbr.second;
                int cost = use_time ? (d + 120 + 40 * weight) : (d + weight);

                if (cost < dist[nbr.first]) {
                    pq.erase({dist[nbr.first], nbr.first});
                    dist[nbr.first] = cost;
                    pq.insert({cost, nbr.first});
                }
            }
        }
        return -1;
    }

    string getMinDistance(string src, string dst) {
        unordered_map<string, bool> visited;
        string best = "";
        int min_dist = INT_MAX;

        function<void(string, int, string)> dfs = [&](string node, int dist, string path) {
            if (node == dst) {
                if (dist < min_dist) {
                    min_dist = dist;
                    best = path + " " + node;
                }
                return;
            }
            visited[node] = true;
            for (auto &nbr : vtces[node].nbrs) {
                if (!visited[nbr.first])
                    dfs(nbr.first, dist + nbr.second, path + " " + node);
            }
            visited[node] = false;
        };

        dfs(src, 0, "");
        return best + "\nTotal Distance: " + to_string(min_dist);
    }

    string getMinTime(string src, string dst) {
        unordered_map<string, bool> visited;
        string best = "";
        int min_time = INT_MAX;

        function<void(string, int, string)> dfs = [&](string node, int time, string path) {
            if (node == dst) {
                if (time < min_time) {
                    min_time = time;
                    best = path + " " + node;
                }
                return;
            }
            visited[node] = true;
            for (auto &nbr : vtces[node].nbrs) {
                if (!visited[nbr.first])
                    dfs(nbr.first, time + 120 + 40 * nbr.second, path + " " + node);
            }
            visited[node] = false;
        };

        dfs(src, 0, "");
        return best + "\nTotal Time (mins): " + to_string((min_time + 59) / 60);
    }
};

// ------------------ Main App ------------------
#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <limits>
#include <functional>
#include <sstream>
using namespace std;

// Heap (max‑peak style) + Graph classes same as before...

// ... (Heap & Graph code identical)

int main(){
    Graph metro;
    // Add all stations (Orange Line)
    vector<string> orange = {"Automotive Square","Nari Road","Indora Square","Kadbi Square",
        "Gaddi Godam Square","Kasturchand Park","Zero Mile","Sitabuldi","Congress Nagar",
        "Rahate Colony","Ajni Square","Chhatrapati Square","Jaiprakash Nagar","Ujjwal Nagar",
        "Airport","Airport South","New Airport","Khapri"};
    for(auto &s: orange) metro.addVertex(s);

    // Aqua Line
    vector<string> aqua = {"Prajapati Nagar","Vaishno Devi Square","Ambedkar Square",
        "Telephone Exchange","Chitar Oli Square","Agrasen Square","Dosar Vaisya Square",
        "Nagpur Railway Station","Cotton Market","Sitabuldi","Jhansi Rani Square",
        "Institution of Engineers","Shankar Nagar Square","LAD Square","Dharampeth College",
        "Subhash Nagar","Rachana Ring Road","Vasudev Nagar","Bansi Nagar","Lokmanya Nagar"};
    for(auto &s: aqua) if(!metro.containsVertex(s)) metro.addVertex(s);

    // Orange edges (~1.2 km steps)
    for(int i=0;i+1<orange.size();i++){
        metro.addEdge(orange[i],orange[i+1], 1200);
    }
    // Aqua edges based on chainage differences
    for(int i=0;i+1<aqua.size();i++){
        int dif = 1200; // you can replace with actual chainage diff lookup
        metro.addEdge(aqua[i],aqua[i+1], dif);
    }

    cout<<"**** WELCOME TO NAGPUR METRO APP ****\\n";
 while (true) {
    cout << "\n=====================================================\n";
    cout << "          🚇 WELCOME TO NAGPUR METRO APP 🚇\n";
    cout << "=====================================================\n";
    cout << "Choose an option from the menu below:\n\n";
    cout << "  1. 📋  List All Stations\n";
    cout << "  2. 🗺️   Show Metro Map (Stations + Connections)\n";
    cout << "  3. 🚏  Get Shortest Distance Between Two Stations\n";
    cout << "  4. ⏱️   Get Shortest Time Between Two Stations\n";
    cout << "  5. ❌  Exit\n";
    cout << "-----------------------------------------------------\n";
    cout << "Enter your choice (1–5): ";

    int c;
    cin >> c;
    cin.ignore();  // consume leftover newline

    if (c == 5) break;

    if (c == 1) metro.displayStations();
    else if (c == 2) metro.displayMap();
    else if (c == 3 || c == 4) {
        cout << "\nEnter source station: ";
        string src, dst;
        getline(cin, src);
        cout << "Enter destination station: ";
        getline(cin, dst);

        bool byTime = (c == 4);
        int res = metro.dijkstra(src, dst, byTime);
        if (res < 0) cout << "\n⚠️  Invalid station names or no path exists.\n";
        else cout << "\n✅ " << (byTime ? "Estimated Time: " : "Shortest Distance: ") 
                  << res << (byTime ? " minutes\n" : " meters\n");
    }
    else {
        cout << "\n❌ Invalid choice. Please select between 1 and 5.\n";
    }
}

    return 0;
}

