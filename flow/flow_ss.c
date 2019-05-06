#include <execinfo.h>

#include <stdbool.h>

#include <stdio.h>

#include <stdlib.h>

#include <inttypes.h>

#ifndef FLOW
#define FLOW


#define long long long

#ifndef FAIL_H
#define FAIL_H


/**
 * @brief The fail-module.
 * 
 * @file fail.h
 * @author WingCorp
 * @date 2018-06-19
 */

/**
 * @brief Exit the program with an error value.
 */
void fail();

/**
 * @brief Print the error cause and exit the program with an error value.
 * 
 * @param cause the cause to print before exiting.
 */
void failwith(char* cause);

#endif

#define FAIL_RED "\x1b[31m"
#define FAIL_RESET "\x1b[0m"

void* callstack[1024];

void trace_stack()
{
    int frames = backtrace(callstack, 1024);
    char** strs = backtrace_symbols(callstack, frames);
    int i;
    printf(FAIL_RED);
    for (i = 0; i < frames; i++)
    {
        printf("%s\n", strs[i]);
    }
    printf(FAIL_RESET);
    free(strs);
}

void fail()
{
    trace_stack();
    exit(EXIT_FAILURE);
}

void failwith(char* cause)
{
    printf(FAIL_RED);
    printf("%s\n", cause);
    printf(FAIL_RESET);
    trace_stack();
    exit(EXIT_FAILURE);
}

#ifndef MATH
#define MATH
/***
 * The math module.
 * 
 **/

long math_longAbs(long l);
int math_intAbs(int i);

double math_doubleAbs(double d);
float math_floatAbs(float f);

int ilog10(int i);

#endif
/**
 * Math
 * @brief An attempt at a simple math module in C.
 * 
 * @file math.c
 * @author Jon Voigt Tøttrup
 * @date 2018-06-14
 */

double math_epsilon = 0.0000001;

long math_longAbs(long l) 
{
    if(l < 0) 
    {
        return l * -1;
    }
    else return l;
}

int math_intAbs(int i) 
{
    return (int) math_longAbs((long) i);
}

double math_doubleAbs(double d)
{
    if(d + math_epsilon < 0)
    {
        return d * -1;
    }
    else return d;
}

float math_floatAbs(float f)
{
    return (float) math_doubleAbs((double) f);
}

int ilog10(int i)
{
    return 
        (i > 999999999) ? 9 : (i >= 99999999) ? 8 :
        (i > 9999999) ? 7 : (i > 999999) ? 6 : (i > 99999) ? 5 : 
        (i > 9999) ? 4 : (i > 999) ? 3 : (i > 99) ? 2 : (i > 9) ? 1 : 0;
}

#ifndef DYNAMIC_H
#define DYNAMIC_H
/**
 * @brief Union type for emulating dynamic typing.
 * 
 * @file dynamic.h
 * @author WingCorp
 * @date 2018-06-15
 */


typedef struct _Dynamic Dynamic;

typedef enum _DynType 
{
    BOOL,
    CHAR,
    INT,
    LONG,
    UINT,
    ULONG,
    FLOAT,
    DOUBLE,
    REFERENCE,
    PAIR
} DynType;

typedef struct _Pair
{
    Dynamic* left;
    Dynamic* right;
} Pair;

typedef union _DynValue
{
    bool bol;
    char chr;
    int i32;
    unsigned int ui32;
    long i64;
    unsigned long ui64;
    float f32;
    double f64;
    void* ref;
    Pair pair;
} DynValue;

typedef struct _Dynamic
{
    DynType type;
    DynValue value;
} Dynamic;

/**
 * @brief Creates a new pair from two dynamic values.
 * 
 * @param left The left value.
 * @param right The right value.
 * @return Pair The constructed pair.
 */
Dynamic pair(Dynamic left, Dynamic right);

/**
 * @brief Get the first value (leftmost) stored in a pair.
 * 
 * @param pair The pair.
 * @return Dynamic The first value.
 */
Dynamic fst(Dynamic pair);

/**
 * @brief Returns the type of the first value of the pair.
 * 
 * @param pair The pair.
 * @return DynType The type of the first value.
 */
DynType t_fst(Dynamic pair);

/**
 * @brief Get the second value (rightmost) stored in a pair.
 * 
 * @param pair The pair.
 * @return Dynamic The second value.
 */
Dynamic snd(Dynamic pair);

/**
 * @brief Return the type of the second value of the pair.
 * 
 * @param pair The pair.
 * @return DynType The type of the second value.
 */
DynType t_snd(Dynamic pair);


Dynamic dbol(bool val);
Dynamic dchr(char val);
Dynamic di32(int val);
Dynamic dui32(unsigned int val);
Dynamic di64(long i64);
Dynamic dui64(unsigned long ui64);
Dynamic df32(float val);
Dynamic df64(double val);
Dynamic dref(void* ref);


//Shorthands to get the values of a dynamic.
//Be wary of segmentation fault.

bool bol(Dynamic dyn);
char chr(Dynamic dyn);
int i32(Dynamic dyn);
unsigned int ui32(Dynamic dyn);
long i64(Dynamic dyn);
unsigned long ui64(Dynamic dyn);
float f32(Dynamic dyn);
double f64(Dynamic dyn);
void* ref(Dynamic dyn);

/**
 * @brief Delete a dynamic if it is on the heap.
 *        Frees the memory held by a dynamic reference or a dynamic pair.
 * 
 * @param dyn A dynamic holding a reference or a pair-dynamic.
 */
void delete(Dynamic dyn);

#endif


Dynamic dbol(bool val)
{
    return (Dynamic) { .type = BOOL, .value = (DynValue) { .chr = val } };
}

Dynamic dchr(char val)
{
    return (Dynamic) { .type = CHAR, .value = (DynValue) { .chr = val } };
}

Dynamic di32(int val)
{
    return (Dynamic) { .type = INT, .value = (DynValue) { .i32 = val } };
}

Dynamic dui32(unsigned int val)
{
    return (Dynamic) { .type = UINT, .value = (DynValue) { .ui32 = val } };
}

Dynamic di64(long val)
{
    return (Dynamic) { .type = LONG, .value = (DynValue) { .i64 = val } };
}

Dynamic dui64(unsigned long val)
{
    return (Dynamic) { .type = ULONG, .value = (DynValue) { .ui64 = val } };
}

Dynamic df32(float val)
{
    return (Dynamic) { .type = FLOAT, .value = (DynValue) { .f32 = val } };
}

Dynamic df64(double val)
{
    return (Dynamic) { .type = DOUBLE, .value = (DynValue) { .f64 = val } };
}

Dynamic dref(void* ref)
{
    return (Dynamic) { .type = REFERENCE, .value = (DynValue) { .ref = ref } };
}

Dynamic pair(Dynamic left, Dynamic right)
{
    Dynamic* lp = malloc(sizeof(Dynamic));
    lp->type = left.type;
    lp->value = left.value;
    Dynamic* rp = malloc(sizeof(Dynamic));
    rp->type = right.type;
    rp->value = right.value;
    return (Dynamic) 
    { 
        .type = PAIR, 
        .value = (DynValue) 
        { 
            .pair = (Pair)
            { 
                .left = lp,
                .right = rp
            } 
        }
    };
}

Dynamic fst(Dynamic pair)
{
    if (pair.type != PAIR)
    {
        failwith("Call to fst() expected Dynamic of type PAIR!");
    }
    return (*pair.value.pair.left);
}

DynType t_fst(Dynamic pair)
{
    if (pair.type != PAIR)
    {
        failwith("Call to t_fst() expected Dynamic of type PAIR!");
    }
    return pair.value.pair.left->type;
}

Dynamic snd(Dynamic pair)
{
    if (pair.type != PAIR)
    {
        failwith("Call to snd() expected Dynamic of type PAIR!");
    }
    return (*pair.value.pair.right);
}

DynType t_snd(Dynamic pair)
{
    if (pair.type != PAIR)
    {
        failwith("Call to t_snd() expected Dynamic of type PAIR!");
    }
    return pair.value.pair.right->type;
}

bool bol(Dynamic dyn)
{
    if (dyn.type != BOOL)
    {
        failwith("Call to bol() expected Dynamic of type BOOL!");
    }
    return dyn.value.bol;
}

char chr(Dynamic dyn)
{
    if (dyn.type != CHAR)
    {
        failwith("Call to chr() expected Dynamic of type CHAR!");
    }
    return dyn.value.chr;
}

int i32(Dynamic dyn)
{
    if (dyn.type != INT)
    {
        failwith("Call to i32() expected Dynamic of type INT!");
    }
    return dyn.value.i32;
}

unsigned int ui32(Dynamic dyn)
{
    if (dyn.type != UINT)
    {
        failwith("Call to ui32() expected Dynamic of type UINT!");
    }
    return dyn.value.ui32;
}

long i64(Dynamic dyn)
{
    if (dyn.type != LONG)
    {
        failwith("Call to i64() expected Dynamic of type LONG!");
    }
    return dyn.value.i64;
}

unsigned long ui64(Dynamic dyn)
{
    if (dyn.type != ULONG)
    {
        failwith("Call to ui64() expected Dynamic of type ULONG!");
    }
    return dyn.value.ui64;
}

float f32(Dynamic dyn)
{
    if (dyn.type != FLOAT)
    {
        failwith("Call to f32() expected Dynamic of type FLOAT!");
    }
    return dyn.value.f32;
}

double f64(Dynamic dyn)
{
    if (dyn.type != DOUBLE)
    {
        failwith("Call to f64() expected Dynamic of type DOUBLE!");
    }
    return dyn.value.f64;
}

void* ref(Dynamic dyn)
{
    if (dyn.type != REFERENCE)
    {
        failwith("Call to ref() expected Dynamic of type REFERENCE!");
    }
    return dyn.value.ref;
}

void delete(Dynamic dyn)
{
    if (dyn.type != REFERENCE || dyn.type != PAIR) {
        failwith("Call to delete() expected Dynamic of type REFERENCE or PAIR!");
    }
    if (dyn.type == REFERENCE) {
        free(dyn.value.ref);
    }
    if (dyn.type == PAIR) {
        free(dyn.value.pair.left);
        free(dyn.value.pair.right);
    }
}

#ifndef OPTION_H
#define OPTION_H
/**
 * @brief The option monad
 * 
 * @file option.h
 * @author WingCorp
 * @date 2018-06-15
 */


typedef enum _OptionType
{
    NONE,
    SOME
} OptionType;

typedef struct _Option
{
    OptionType type;
    Dynamic value;
} Option;

/**
 * @brief The 'none' return-function.
 * 
 * @return Option 
 */
Option none();

/**
 * @brief The 'some' return-function
 * 
 * @param value 
 * @return Option 
 */
Option some(Dynamic value);

/**
 * @brief Takes an option and a function 
 *        that takes the result of the given option
 *        and applies the option to the given function
 *        if the option was some.
 *
 * @param opt an Option value.
 * @param optFunc a function of signature: Option <name>(Dynamic <arg>); 
 * @return Option an option value.
 */
Option success(Option opt, Option (*optFunc)(Dynamic));

/**
 * @brief Coerces an option to its value.
 *        Might cause a failure, if no value is present.
 * 
 * @param opt the option to coerce.
 * @return Dynamic the value to find;
 */
Dynamic coerce(Option opt);

#endif

Option none()
{
    return (Option) { .type = NONE };
}

Option some(Dynamic value)
{
    return (Option) { .type = SOME, .value = value };
}

Option success(Option opt, Option (*optFunc)(Dynamic))
{
    if (opt.type == NONE)
    {
        return none();
    }
    return (*optFunc)(opt.value);
}

Dynamic coerce(Option opt)
{
    if (opt.type == NONE)
    {
        failwith("Could not coerce value from empty option!\n");
    }
    return opt.value;
}
#ifndef ITERATOR_H
#define ITERATOR_H


typedef struct _Iterator Iterator;

/**
 * @brief Initializes a new iterator from a Dynamic pointer.
 * 
 * @param data The data to iterate from.
 * @param length The length of the data.
 * @return Iterator An Iterator struct.
 */
Iterator* iterator_init_eager(Dynamic* data, int length);

/**
 * @brief Initializes a new iterator from an int -> Dynamic function.
 * 
 * @param nextFun A function that takes the position of the iterator
 *                and returns the corresponding Dynamic.
 * @param length The length of the iterator.
 * @return Iterator* An Iterator struct.
 */
Iterator* iterator_init_lazy(Dynamic (*nextFun)(int), int length);

/**
 * @brief Whether an iterator holds another value.
 * 
 * @param iterator 
 * @return bool a bool value.
 */
bool iterator_hasNext(Iterator* iterator);

/**
 * @brief Retrieves the next value from the iterator.
 * 
 * @param iterator the iterator to get the value from.
 * @return Option some if there is a value, none if there isn't.
 */
Option iterator_next(Iterator* iterator);

/**
 * @brief How many entries that remain in the iterator.
 * 
 * @param iterator 
 * @return int 
 */
int iterator_remaining(Iterator* iterator);

/**
 * @brief Resets iterator.
 * 
 * @param iterator the iterator.
 */
void iterator_reset(Iterator* iterator);

/**
 * @brief Destroys the iterator and the values that the iterator is reading from.
 * 
 * @param iterator the iterator to destroy.
 */
void iterator_destroy(Iterator* iterator);

#endif

typedef union _Source
{
    Dynamic* data;
    Dynamic (*nextFun)(int);
} Source;

typedef enum _Type
{
    EAGER,
    LAZY
} Type;

typedef struct _Iterator
{
    int position;
    int length;
    Source* source;
    Type type;
} Iterator;

Iterator* iterator_init_eager(Dynamic* data, int length)
{
    Iterator* iterator = malloc(sizeof(Iterator));
    iterator->position = 0;
    iterator->length = length;
    iterator->type = EAGER;
    iterator->source = malloc(sizeof(Source));
    iterator->source->data = data;
    return iterator;
}

Iterator* iterator_init_lazy(Dynamic (*nextFun)(int), int length)
{
    Iterator* iterator = malloc(sizeof(Iterator));
    iterator->position = 0;
    iterator->length = length;
    iterator->type = LAZY;
    iterator->source = malloc(sizeof(Source));
    iterator->source->nextFun = nextFun;
    return iterator;
}

bool iterator_hasNext(Iterator* iterator)
{
    return iterator->position < iterator->length;
}

Option iterator_next(Iterator* iterator)
{
    if(iterator->position < iterator->length)
    {
        Dynamic toReturn;
        switch(iterator->type)
        {
            case EAGER:
                toReturn = iterator->source->data[iterator->position];
                break;
            case LAZY:
                toReturn = iterator->source->nextFun(iterator->position);
                break;
        }
        iterator->position += 1;
        return some(toReturn);
    }
    else
    {
        return none();
    }
}

int iterator_remaining(Iterator* iterator)
{
    return iterator->length - iterator->position;
}

void iterator_reset(Iterator* iterator)
{
    iterator->position = 0;
}

void iterator_destroy(Iterator* iterator)
{
    free(iterator);
}

#ifndef STACK_H
#define STACK_H

/**
 * @brief A resizable stack for working with dynamics.
 * 
 * @file stack.h
 * @author WingCorp
 * @date 2018-06-15
 */


typedef struct _Stack Stack;

/**
 * @brief Initializes a new Stack data structure.
 * 
 * @param initialCap the initial capacity of the stack.
 * @return Stack* a pointer to the new stack.
 */
Stack* stack_init(int initialCap);

/**
 * @brief Frees the memory occupied by a Stack data structure.
 * 
 * @param stack the stack to destroy.
 */
void stack_destroy(Stack* stack);

/**
 * @brief Pushes a value on the stack.
 * 
 * @param stack the stack to push the value on.
 * @param dyn the dynamic to put on the stack.
 */
void stack_push(Stack* stack, Dynamic dyn);

/**
 * @brief Pops a value from the stack.
 * 
 * @param stack the stack to pop from.
 * @return Option the value from the stack, if any.
 */
Option stack_pop(Stack* stack);

/**
 * @brief Determines number of values currently stored in the stack.
 * 
 * @param stack the stack to check.
 * @return int the number of values in the stack.
 */
int stack_size(Stack* stack);

/**
 * @brief Determines the capacity of the stack.
 * 
 * @param stack the stack to check.
 * @return int how many values the stack can hold, before reallocating.
 */
int stack_capacity(Stack* stack);

/**
 * @brief Trims the capacity of the stack to just the values.
 * 
 */
void stack_trim(Stack* stack);

/**
 * @brief Gets an iterator from the current stack.
 * 
 * @return Iterator an iterator.
 */
Iterator* stack_iterator(Stack* stack);

/**
 * @brief Maps every entry in the stack to a new stack using a mapper function.
 * 
 * @param stack the stack to map.
 * @param mapper the mapper function.
 * @return Stack* the mapped stack.
 */
Stack* stack_map(Stack* stack, Dynamic (*mapper) (Dynamic));

/**
 * @brief Folds over every entry in the stack.
 * 
 * @param stack the stack to fold.
 * @param state the initial state to fold onto.
 * @param folder the folder function with the signature Dynamic folder(Dynamic acc, Dynamic i)
 * @return Dynamic the final state.
 */
Dynamic stack_fold(Stack* stack, Dynamic state, Dynamic (*folder) (Dynamic, Dynamic));

#endif
/**
 * @brief A resizable stack for working with dynamics.
 * 
 * @file stack.c
 * @author WingCorp
 * @date 2018-06-15
 */


typedef struct _Stack
{
    int capacity;
    int size;
    Dynamic* dynamics;
} Stack;

Stack* stack_init(int initialCap)
{
    Stack* stack = malloc(sizeof(Stack));
    stack->capacity = initialCap;
    stack->size = 0;
    stack->dynamics = malloc(sizeof(Dynamic) * initialCap);
    return stack;
}

void stack_destroy(Stack* stack)
{
    free(stack->dynamics);
    free(stack);
}

void reallocStack(Stack* stack, float sizeChange)
{
    int oldCap = stack->capacity;
    int newCap = oldCap * sizeChange;
    stack->dynamics = realloc(stack->dynamics, newCap * sizeof(Dynamic));
    stack->capacity = newCap;
}

void increaseCapacity(Stack* stack)
{
    reallocStack(stack, 2.5);
}

void stack_push(Stack* stack, Dynamic dyn)
{
    int top = stack->size;
    if (top + 1 == stack->capacity)
    {
        increaseCapacity(stack);
    }
    stack->dynamics[top] = dyn;
    stack->size += 1;
}

void decreaseCapacity(Stack* stack)
{
    reallocStack(stack, 0.5);
}

Option stack_pop(Stack* stack)
{
    if (stack->size == 0) {
        return none();
    }
    int top = stack->size - 1;
    Dynamic toReturn = stack->dynamics[top];
    stack->size -= 1;
    if (stack->size * 2 < stack->capacity)
    {
        decreaseCapacity(stack);
    }    
    return some(toReturn);
}

int stack_size(Stack* stack)
{
    return stack->size;
}

int stack_capacity(Stack* stack)
{
    return stack->capacity;
}

void stack_trim(Stack* stack)
{
    int newCap = stack->size;
    stack->dynamics = realloc(stack->dynamics, newCap * sizeof(Dynamic));
    stack->capacity = newCap;
}

Iterator* stack_iterator(Stack* stack)
{
    return iterator_init_eager(stack->dynamics, stack->size);
}

Stack* stack_map(Stack* stack, Dynamic (*mapper) (Dynamic))
{
    Stack* mappedStack = stack_init(stack->size);
    int i;
    for (i = 0; i < stack->size; i++){
        Dynamic currentValue = stack->dynamics[i];
        Dynamic mappedValue = (*mapper)(currentValue);
        stack_push(mappedStack, mappedValue);
    }
    return mappedStack;
}

Dynamic stack_fold(Stack* stack, Dynamic state, Dynamic (*folder) (Dynamic, Dynamic))
{
    Dynamic currentState = state;
    int i;
    for (i = 0; i < stack->size; i++)
    {
        Dynamic currentValue = stack->dynamics[i];
        currentState = (*folder)(currentState, currentValue);
    }
    return currentState;
}

#endif

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
