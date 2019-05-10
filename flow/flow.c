
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
        return pair(dref(list_cons(nodes, di32(currentNode))), di32(bottleneck));
    }
    Stack* adj = stack_init(1 + ilog10(n));
    int i;
    
    Iterator* nodeIte = list_iterator(nodes);
    for (i = 0; i < n; i++)
    {
        if (i != currentNode && res_network[currentNode][i] > 0 && nodesContains(nodeIte, i))
        {
            stack_push(adj, di32(i));
        }
    }
    iterator_destroy(nodeIte);
    Iterator* adjacent = stack_iterator(adj);
    Option opt;
    for (opt = iterator_next(adjacent); opt.type == SOME; opt = iterator_next(adjacent))
    {
        int nextNode = i32(coerce(opt));
        int cap = res_network[currentNode][nextNode];
        int currentMin = (cap < bottleneck) ? cap : bottleneck;
        Dynamic next = pair(dref(list_cons(nodes, di32(currentNode))), di32(currentMin));
        Dynamic path = findPath(next, nextNode);
        nodeIte = list_iterator(ref(fst(path)));
        if (!nodesContains(nodeIte, t)) {
            continue;
        }
        iterator_destroy(nodeIte);
        return path;
    }
    iterator_destroy(nodeIte);
    return acc;
}

int main()
{
    printf("Ready!\n");
    //Parse:
    scanf("%d %d %d %d", &n, &m, &s, &t);
    printf("Parsed first line:\n%d %d %d %d\n", n, m, s, t);
    int i;

    network = malloc(sizeof(int*)*n);
    for (i = 0; i < n; i++)
    {
        network[i] = malloc(sizeof(int) * n);
    }
    
    res_network = malloc(sizeof(int*)*n);
    for (i = 0; i < n; i++)
    {
        network[i] = malloc(sizeof(int) * n);
    }

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
