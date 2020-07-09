#include <iostream>
#include <cstddef>
#include <fstream>
#include <string>

struct Vertex;
struct Edge { // a node in a linked list of edges
    Edge(Vertex *vertex, int weight, Edge *nextEdge = 0)
        : vertex(vertex), weight(weight), nextEdge(nextEdge) {}
    Vertex *vertex;
    int weight; // or length
    Edge *nextEdge;
};

struct Vertex { // a node in a linked list of vertexes
    Vertex(char name, Vertex *nextVertex = NULL, Edge *edgeList = NULL)
        : name(name), nextVertex(nextVertex), edgeList(edgeList) {}
    char name;
    Vertex *nextVertex;
    Edge *edgeList;
    int index; // index, final_, and *pre are set by shortestPath()
    bool final_; // final is a C++ keyword
    Vertex *pre;
};

// builds a lowest path s->..->f in graph g if possible, o/w f->final_ is false
void shortestPath(Vertex *g, const int inf, Vertex *s, Vertex *f) {
    Vertex *v = g;
    while (v != NULL) {
        v->index = inf;
        v->final_ = false;
        v = v->nextVertex;
    }
    s->index = 0;
    s->final_ = true;
    s->pre = NULL;
    Vertex *last = s;
    while (!f->final_) {
        Edge *x = last->edgeList;
        while (x != NULL) {
            Vertex *v = x->vertex;
            if (!v->final_ && v->index > last->index + x->weight) {
                v->index = last->index + x->weight;
                v->pre = last;
            }
            x = x->nextEdge;
        }
        Vertex *v = g, *w = NULL;
        while (v != NULL) {
            if (!v->final_) {
                if (w == NULL || v->index < w->index) {
                    w = v;
                }
            }
            v = v->nextVertex;
        }
        if (w == NULL || w->index == inf) // data error; f is unreached
            return;
        w->final_ = true;
        last = w;
    }
}

// adds to a graph a NEW vertex v with name nm in a SORTED way
void addVertex(Vertex *&g, char nm, Vertex *&v) {
    if (g == NULL) {
        v = g = new Vertex(nm);
    } else {
        Vertex *prev = NULL;
        v = g;
        while (v != NULL && v->name < nm) {
            prev = v;
            v = v->nextVertex;
        }
        if (v == NULL) {
            prev->nextVertex = v = new Vertex(nm);
        } else if (v->name > nm && prev == NULL) {
            g = v = new Vertex(nm, v);
        } else if (v->name > nm) {
            prev->nextVertex = v = new Vertex(nm, v);
        }
    }
}

// adds to a vertex v an edge points at a NEW vertex u of the weigth vuwei
void addEdge(Vertex *v, Vertex *u, int vuwei) {
    Edge *e = v->edgeList;
    if (e == NULL) {
        v->edgeList = new Edge(u, vuwei);
    } else {
        while (e->nextEdge != NULL && e->vertex != u) {
            e = e->nextEdge;
        }
        if (e->nextEdge == NULL && e->vertex != u) {
            e->nextEdge = new Edge(u, vuwei);
        }
    }
}

// sets v to the vertex in a graph g named nm, or to NULL
void findVertex(Vertex *g, char nm, Vertex *&v) {
    v = g;
    while (v != NULL && v->name != nm) {
        v = v->nextVertex;
    }
}

void destroyGraph(Vertex *&g) {
    Vertex *oldg;
    Edge *e, *olde;
    while (g != NULL) {
        e = g->edgeList;
        while (e != NULL) {
            olde = e;
            e = e->nextEdge;
            delete olde;
        }
        oldg = g;
        g = g->nextVertex;
        delete oldg;
    }
}

int main(int argc, char **argv) {
    if (argc != 4)
        return 1; // filename and 2 vertexes must be input from the command line
    if (!(argv[2][0] && !argv[2][1]))
        return 2; // any vertex name is a single character
    if (!(argv[3][0] && !argv[3][1]))
        return 3; // the name of a vertex must be a single character

    std::ifstream data(argv[1]);
    if (!data)
        return 4; // the file cannot be opened

    Vertex *g = NULL, *v, *u;
    char src, dst; // lines in the file have format: src,dsr, wei
    int wei;
    int inf = 1; // 1 + sum of all weights
    while (data.peek(), data.good()) { // while the file can be read
        data >> src;
        data.ignore(1, ',');
        data >> dst;
        data.ignore(1, ',');
        data >> wei;
        addVertex(g, src, v);
        addVertex(g, dst, u);
        addEdge(v, u, wei);
        inf += wei;
        data.ignore(1, '\n'); // it is OK if the last line ends with NL
    }
    if (data.bad()) // the file wasn't read until EOF
        return 5;
    if (g == NULL) // the file was empty
        return 6;

    Vertex *s, *f;
    findVertex(g, argv[2][0], s);
    if (s == NULL)
        return 7; // the name is incorrect since it was not found
    findVertex(g, argv[3][0], f);
    if (f == NULL)
        return 8; // there is no vertex with such a name

    shortestPath(g, inf, s, f);
    std::cout << "The shortest path in reverse order: ";
    if (!f->final_) {
        std::cout << "No such path exist\n";
    } else {
        Vertex *v = f;
        while (v != NULL) { // traverse the vertexes on the path
            if (v != s) {
                std::cout << v->name << ", ";
            } else {
                std::cout << v->name << "\n";
            }
            v = v->pre;
        }
    }

    destroyGraph(g);
}
