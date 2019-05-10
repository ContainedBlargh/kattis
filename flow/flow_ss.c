#include <stdbool.h>

#include <stdlib.h>

#include <stdio.h>

#include <execinfo.h>

#include <inttypes.h>

#include <stdarg.h>




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

long math_longAbs(long l) 
{
    return (l < 0L) ? l * -1L : l;
}

int math_intAbs(int i) 
{
    return (i < 0) ? i * -1 : i;
}

double math_doubleAbs(double d)
{
    return (d < 0.) ? d * -1. : d;
}

float math_floatAbs(float f)
{
    return (f < 0.f) ? f * -1.f : f;
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

#ifndef FOREACH_H
#define FOREACH_H

/**
 * @brief The foreach function.
 * 
 * @file foreach.h
 * @author WingCorp
 * @date 2018-06-16
 */


/**
 * @brief Performs an action on every value from an iterator using an action function.
 *  
 * @param iterator the iterator to loop through.
 * @param action the action function, must have signature: void <name>(Dynamic <arg>)
 */
void foreach(Iterator* iterator, void (*action) (Dynamic));

#endif
void foreach(Iterator* iterator, void (*action) (Dynamic))
{
    while (iterator_hasNext(iterator))
    {
        (*action)(iterator_next(iterator).value);
    }
    iterator_reset(iterator);
}
#ifndef LIST_H
#define LIST_H

/**
 * @brief Immutable linked list module.
 * 
 * @file linkedList.h
 * @author your name
 * @date 2018-06-17
 */



typedef struct _List List;

/**
 * @brief Creates a new, empty list.
 * 
 * @return List* an empty list.
 */
List* list_empty();

/**
 * @brief Creates a new list from the given items.
 * 
 * @param count The total number of items.
 * @param item A dynamic item.
 * @param ... A variable amount of dynamics.
 * @return List* 
 */
List* list_from(int count, Dynamic item, ...);

/**
 * @brief Cons a new value to the end of the list and return the list.
 * 
 * @param list the list to cons
 * @param val the value to cons
 * @return List the new list
 */
List* list_cons(List* list, Dynamic val);

/**
 * @brief Concatenates two lists together.
 * 
 * @param listA the first list
 * @param listB the second list
 * @return List the result of the concatenation.
 */
List* list_concatenate(List* listA, List* listB);

/**
 * @brief Retrieves the head-element from the list.
 * 
 * @param list the list to take the head from.
 * @return Option the head element.
 */
Option list_head(List* list);

/**
 * @brief Retrieves the tail of the list.
 * 
 * @param list the list.
 * @return List the tail of the list.
 */
List* list_tail(List* list);

/**
 * @brief Retrieves the item at given index in given list.
 * 
 * @param list the list
 * @param index the index
 * @return Option some item, if any
 */
Option list_item(List* list, int index);

/**
 * @brief Retrieves the number of elements in the list.
 * 
 * @param list the list.
 * @return int the length of the list in number of elements.
 */
int list_length(List* list);

/**
 * @brief Deallocs the given list.
 * 
 * @param list a list.
 */
void list_destroy(List* list);

/**
 * @brief Creates a new list from an iterator (consumes the iterator).
 * 
 * @param it the iterator to create the list from.
 * @return List* 
 */
List* list_initFromIterator(Iterator* it);

/**
 * @brief Initialize linked list with function.
 * 
 * @param size size of the linked list.
 * @param initFunc function that creates dynamics values from the index in the list.
 * @return List* the new list.
 */
List* list_initFromFunc(int size, Dynamic (*initFunc)(int));

/**
 * @brief Retrieves an iterator from the list;
 * 
 * @param list a list.
 * @return Iterator an iterator for the content of the given list.
 */
Iterator* list_iterator(List* list);

#endif

struct _Link;

typedef struct _Link
{
    Dynamic value;
    struct _Link* next;
    struct _Link* prev;
} Link;

typedef struct _List
{
    Link* start;
    Link* end;
    int length;
} List;

List* list_empty()
{
    List* listRef = malloc(sizeof(List));
    listRef->length = 0;
    listRef->start = NULL;
    listRef->end = NULL;
    return listRef;
}

List* list_cons(List* list, Dynamic val)
{
    // printf("Beginning cons by creating new list:\n");
    List* outlist = list_empty();
    // printf("malloc next link:\n");
    Link* link = malloc(sizeof(Link));
    // printf("assign val to link:\n");
    link->value = val;
    if(list->start)
    {
        // printf("Cons move end\n");
        link->prev = list->end;
        link->prev->next = link;
        outlist->start = list->start;
    }
    else
    {
        // printf("Cons to end\n");
        outlist->start = link;
    }
    outlist->end = link;
    outlist->length = list->length + 1;
    return outlist;
}

List* list_copy(List* list)
{
    List* outList = list_empty();
    Link* currentLink = list->start;
    while(currentLink->next)
    {
        outList = list_cons(outList, currentLink->value);
        currentLink = currentLink->next;
    }
    return list_cons(outList, currentLink->value);
}

List* list_concatenate(List* listA, List* listB)
{
    List* outList = list_empty();
    Link* currentLink = listA->start;
    while(currentLink->next)
    {
        outList = list_cons(outList, currentLink->value);
        currentLink = currentLink->next;
    }
    outList = list_cons(outList, currentLink->value);
    currentLink = listB->start;
    while(currentLink->next)
    {
        outList = list_cons(outList, currentLink->value);
        currentLink = currentLink->next;
    }
    outList = list_cons(outList, currentLink->value);
    return outList;
}

Option list_head(List* list)
{
    if (list->start)
    {
        return some(list->start->value);
    }
    else return none();
}

List* list_tail(List* list)
{
    List* outList = list_empty();
    Link* currentLink = list->start;
    while(currentLink->next)
    {
        outList = list_cons(outList, currentLink->value);
        currentLink = currentLink->next;
    }
    return outList;
}

Option list_item(List* list, int index)
{
    if (index < 0 || index >= list->length)
    {
        return none();
    }

    bool goReverse = index > (list->length / 2);
    
    int ptr;
    Link* currentLink;

    if (goReverse)
    {
        ptr = list->length - 1;
        currentLink = list->end;
    }
    else
    {
        ptr = 0;
        currentLink = list->start;
    }

    while(ptr != index)
    {
        if (goReverse && currentLink->prev)
        {
            currentLink = currentLink->prev;
            ptr -= 1;
        }
        else if(!goReverse && currentLink->next)
        {
            currentLink = currentLink->next;
            ptr += 1;
        }
        else 
        {
            return none();
        }
    }
    return some(currentLink->value);
}

int list_length(List* list)
{
    return list->length;
}

void list_destroy(List* list)
{
    free(list);
}

List* list_initFromIterator(Iterator* it)
{
    List* list = list_empty();
    while(iterator_hasNext(it))
    {
        list = list_cons(list, iterator_next(it).value);
    }
    return list;
}

List* list_initFromFunc(int size, Dynamic (*initFunc)(int))
{
    int i;
    List* list = list_empty();
    for(i = 0; i < size; i++)
    {
        Dynamic val = (*initFunc)(i);
        list = list_cons(list, val);
    }
    return list;
}

Iterator* list_iterator(List* list)
{
    int length = list->length;
    // printf("length: %d\n", length);

    int i = 0;
    // printf("malloc'ing length * sizeof(Dynamic): %lu\n", (length * sizeof(Dynamic)));
    Dynamic* dynArr = malloc(length * sizeof(Dynamic));
    // printf("Getting list start link: %p\n", list->start);
    Link* currentLink = list->start;
    // printf("Entering while loop...\n");
    while(currentLink != NULL && currentLink->next != NULL)
    {
        // printf("Current link: %p\n", currentLink);
        // printf("Next link: %p\n", currentLink->next);
        // printf("Current value at current link: %p\n", (currentLink->value.value.ref));
        Dynamic currentValue = currentLink->value;
        dynArr[i] = currentValue;
        currentLink = currentLink->next;
        i += 1;
    }
    // printf("finished walking through list\n");
    if(length > 0){
        dynArr[i] = currentLink->value;
    }
    return iterator_init_eager(dynArr, length);
}

List* list_from(int count, Dynamic item, ...)
{
    va_list args;
    va_start(args, item);
    
    List* list = list_empty();
    list_cons(list, item);

    int i;
    for (i = 1; i < count; i++)
    {
        Dynamic param = va_arg(args, Dynamic);
        list = list_cons(list, param);
    }
    return list;
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
