#include <iostream>
#include <vector>
#include <list>
#define endl '\n'
#define INFINITE 10000

using namespace std;

struct NodeWeight { // objeto nó com o peso.
    int link; // ligacao com o nó
    double weight; // peso do nó

    NodeWeight(int link, double weight) : link(link), weight(weight) {}

};


struct MinHeap { // MinHeap construída com os nós e pesos
    NodeWeight **f;
    int arraySize;
    int heapSize;

    MinHeap() {
        this->f = new NodeWeight*[2];
        this->arraySize = 2;
        this->heapSize = 0;
    }

    void heap_insert(NodeWeight *n) {
        if (this->arraySize == this->heapSize) {
            double_size();
        }
        f[this->heapSize] = n;
        this->heapSize++;
        bubble_up(heapSize);
    }

    void heap_update(NodeWeight *n) {
        bool a = true;
        for (int i = 0; i < heapSize && a; ++i) {
            if (f[i]->link == n->link) {
                f[i]->weight = n->weight;
                bubble_up(heapSize);
                a = false;
            }
        }

        if(a) {
           heap_insert(n);
        }
    }

    void double_size() {
        NodeWeight **aux = new NodeWeight*[2* this->arraySize];
        for (int i = 0; i < this->arraySize; ++i) {
            aux[i] = this->f[i];
        }
        this->f = aux;
        this->arraySize = 2* this->arraySize;
    }

    void bubble_up(int heapSize) {
        int i = heapSize - 1;

        while (i > 0 && this->f[i]->weight < this->f[(i-1)/2]->weight) {
            NodeWeight *aux2 = this->f[i];
            this->f[i] = f[(i-1)/2];
            this->f[(i-1)/2] = aux2;
            i = (i-1)/2;
        }
    }

    NodeWeight* heap_extract() {
        NodeWeight *aux3 = this->f[0];
        this->f[0] = this->f[this->heapSize - 1];
        this->f[this->heapSize - 1] = nullptr;
        this->heapSize--;
        heapify(0);

        return aux3;
    }

    void heapify(int i) {
        int m = i;
        int l = (2 * i) + 1;
        int r = (2 * i) + 2;

        if (l < this->heapSize && this->f[l]->weight <= this->f[m]->weight) {
            m = l;
        }

        if (r < this->heapSize && this->f[r]->weight <= this->f[m]->weight) {
            m = r;
        }

        if (m != i) {
            NodeWeight *aux4 = this->f[i];
            this->f[i] = this->f[m];
            this->f[m] = aux4;
            heapify(m);
        }

    }
};


struct Graph {
    int nodes; // número de nós
    list<NodeWeight> *adjacent; // lista dos vértices adjacentes com o peso

    Graph(int nodes) {
        this->nodes = nodes;
        this->adjacent = new list<NodeWeight>[this->nodes];
    }

    void addEdge(int nodeX, int nodeY, NodeWeight *x, NodeWeight *y) { // adiciona a conexão entre os vértices
        this->adjacent[nodeX].push_back(*x);
        this->adjacent[nodeY].push_back(*y);
    }

    int* dijkstra(int origin) {
        double weights[this->nodes]; // vetor com os pesos entre os vértices
        int *precursor = new int[this->nodes]; // vetor com os precusores do menor caminho


        for(int i = 0; i < this->nodes; ++i) {
            weights[i] = INFINITE;
            precursor[i] = -1;
        }

        weights[origin] = 0;
        precursor[origin] = origin;

        MinHeap *h = new MinHeap();
        NodeWeight *aux = new NodeWeight(origin, 0);

        h->heap_insert(aux);

        while(h->heapSize != 0) { // enquanto tiver elemento na MinHeap
            aux = h->heap_extract();

            if(aux->weight > weights[aux->link]) {
                continue;
            }

            int u = aux->link;

            list<NodeWeight>::iterator it;

            for (it = this->adjacent[u].begin(); it != this->adjacent[u].end(); it++) {
                int v = it->link;
                double c = it->weight;

                if (weights[v] > weights[u] + c) {
                    weights[v] = weights[u] + c;
                    precursor[v] = u;
                    aux = new NodeWeight(v, weights[v]);
                    h->heap_update(aux);
                }

            }

        }

        return precursor;

    }

};


int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double p, w;
    int numNodes, links, nodeX, nodeY, speed, cost, origin, destiny;

    cin >> p;
    cin >> numNodes >> links;

    Graph g(numNodes);

    for(int i = 0; i < links; ++i) {
        cin >> nodeX >> nodeY >> speed >> cost;

        w = (p*speed + ((1-p)*cost)) / (speed+cost);

        NodeWeight *x = new NodeWeight(nodeY, w);
        NodeWeight *y = new NodeWeight(nodeX, w);

        g.addEdge(nodeX, nodeY, x, y);

    }

    while(cin >> origin >> destiny) {
        int *precursor;
        precursor = g.dijkstra(origin);

        vector<int> shortest_path;
        shortest_path.push_back(destiny);

        for (int j = destiny; precursor[j] != origin; j = precursor[j]) {
            shortest_path.push_back(precursor[j]);
        }

        shortest_path.push_back(origin);

        while(!shortest_path.empty()) {
            cout << shortest_path.back();
            shortest_path.pop_back();

            if(!shortest_path.empty()) {
                cout << " ";
            } else {
                cout << endl;
            }
        }
    }

    return 0;
}
