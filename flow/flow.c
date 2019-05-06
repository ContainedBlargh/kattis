
#include "./hdc/hdc.h"

typedef struct _Edge
{
    int u;
    int v;
    int cap;

    int residual;
    int flow;
} Edge;

int edge_residiualTo(Edge edge, int dest)
{
    return (dest == edge.v) ? edge.residual : edge.flow;
}

int edge_addFlowTo(Edge edge, int dest, int flow)
{
    if (dest == edge.u)
    {
        edge.flow -= flow;
        edge.residual += flow;
    }
    else
    {
        edge.residual -= flow;
        edge.flow += flow;
    }

    return edge.flow;
}

int edge_other(Edge edge, int node)
{
    return (edge.u == node) ? edge.v : edge.u;
}


int n;
int m;
int s;
int t;

bool hasAugmentingPath(Stack** adj, Edge* edgeTo)
{
    printf("hasAugmentingPath params: n = %d, m = %d, s = %d, t = %d\n", n, m, s, t);
    printf("Searching for augmenting path\n");
    bool marked[n];
    int queue[n];
    int qp = 0;

    queue[qp] = s;
    marked[s] = true;

    Iterator* iterator;

    while(qp != 0 && !marked[t]) {
        int v = queue[qp];
        qp--;
        printf("v: %d\n", v);
        iterator = stack_iterator(adj[v]);
        printf("v adj size: %d", iterator_remaining(iterator));
        Edge* e;
        while(iterator_hasNext(iterator)){
            e = (Edge*) ref(coerce(iterator_next(iterator)));

            if (marked[t]) {
                return true;
            }

            int w = edge_other(*e, v);

            if (edge_residiualTo(*e, w) > 0 && !marked[w]) {
                edgeTo[w] = *e;
                marked[w] = true;
                queue[++qp] = w;
                if (marked[t]) {
                    return true;
                }                
            }
        }
    }
    return marked[t];
}

int bottleneck(Edge* edgeTo)
{
    int bottle = INT32_MAX;
    int v;
    for (v = t; v != s; v = edge_other(edgeTo[v], v))
    {
        int eh = edge_residiualTo(edgeTo[v], v);
        bottle = (eh < bottle) ? bottle : eh;
    }
    return bottle;
}

int main()
{
    printf("Ready!\n");
    //Parse:
    scanf("%d %d %d %d", &n, &m, &s, &t);
    printf("Parsed first line:\n %d %d %d %d\n", n, m, s, t);
    Edge edges[n];
    bool adjInitialized[n];
    Stack** adj = malloc(sizeof(Stack*) * n);
    int i;

    int u;
    int v;
    int c;
    
    for (i = 0; i < m; i++)
    {
        scanf("%d %d %d", &u, &v, &c);
        edges[i] = (Edge) { .u = u, .v = v, .cap = c, .residual = c, .flow = 0 };
        if (!adjInitialized[u])
        {
            adj[u] = stack_init(1 + ilog10(n) * 2);
        }
        if (!adjInitialized[v])
        {
            adj[v] = stack_init(1 + ilog10(n) * 2);
        }        
        stack_push(adj[u], dref(&edges[i]));
        stack_push(adj[v], dref(&edges[i]));
    }
    
    printf("Parsed remanining %d lines\n", m);

    int value = 0;
    Edge* edgeTo = malloc(sizeof(Edge) * n);

    while (hasAugmentingPath(adj, edgeTo))
    {
        printf("Found augmenting path\n");
        int bottle = bottleneck(edgeTo);
        printf("Bottleneck: %d\n", bottle);
        int v;
        printf("Adding flow along path\n");
        for (v = t; v != s; edge_other(edgeTo[v], v))
        {
            edge_addFlowTo(edgeTo[v], v, bottle);
        }

        for (i = 0; i < n; i++)
        {
            edgeTo = realloc(edgeTo, sizeof(Edge) * n);
        }
        value += bottle;
    }

    Stack* toPrint = stack_init(1 + ilog10(n) * 2);
    int edgesUsed = 0;

    for (i = 0; i < n; i++)
    {
        Edge e = edges[i];
        if (e.flow < 1)
        {
            continue;
        }
        stack_push(toPrint, di32(i));
        edgesUsed++;
    }
    printf("%d %d %d\n", n, value, edgesUsed);
    Iterator* iterator = stack_iterator(toPrint);
    while(iterator_hasNext(iterator)){
        int i = i32(coerce(iterator_next(iterator)));
        Edge e = edges[i];
        printf("%d %d %d\n", e.u, e.v, e.flow);
    }
    return 0;
}
