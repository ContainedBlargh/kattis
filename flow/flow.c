
#include "../hdc/hdc.h"

int n;
int m;
int s;
int t;

int** network;
int** res_network;

bool nodesContains(Iterator* nodes, int i)
{
    Option opt;
    for (opt = iterator_next(nodes); opt.type == SOME; opt = iterator_next(nodes))
    {
        int other = i32(coerce(opt));
        if (i == other)
        {
            return true;
        }        
    }
    return false;
}

Dynamic findPath(Dynamic acc, int currentNode)
{
    List* nodes = ref(fst(acc));
    int bottleneck = i32(snd(acc));
    if (currentNode == t)
    {
        return pair((dref(list_cons(nodes, currentNode)), di32(bottleneck)));
    }
    Stack* adj = stack_init(1 + ilog10(n));
    int i;
    
    Iterator* nodes = list_iterator(nodes);
    for (i = 0; i < n; i++)
    {
        if (i != currentNode && res_network[currentNode][i] > 0 && nodesContains(nodes, i))
        {
            stack_push(adj, di32(i));
        }
    }
    iterator_destroy(nodes);
    Iterator* adjacent = stack_iterator(adj);
    Option opt;
    for (opt = iterator_next(adjacent); opt.type == SOME; opt = iterator_next(adjacent))
    {
        int nextNode = i32(coerce(opt));
        int cap = res_network[currentNode][nextNode];
        int currentMin = (cap < bottleneck) cap : bottleneck;
        int next = pair(dref(list_cons(nodes, currentNode)), di32(currentMin));
        Dynamic path = findPath(next, nextNode);
        nodes = list_iterator(ref(fst(path)));
        if (!nodesContains(nodes, t)) {
            continue;
        }
        iterator_destroy(nodes);
        return path;
    }
    iterator_destroy(nodes);
    return acc;
}

int main()
{
    printf("Ready!\n");
    //Parse:
    scanf("%d %d %d %d", &n, &m, &s, &t);
    printf("Parsed first line:\n%d %d %d %d\n", n, m, s, t);
    int i;

    int n_network[n][n];
    network = n_network;
    int n_res_network[n][n];
    res_network = n_res_network;
    
    List* path;
    int bottleneck;

    int u;
    int v;
    int c;
    
    for (i = 0; i < m; i++)
    {
        scanf("%d %d %d", &u, &v, &c);
        network[u][v] = c;
        res_network[u][v] = c;
    }
    printf("Parsed remanining %d lines\n", m);
}
