#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct DC_node {
    unsigned int index;
    unsigned int shortestPathsSum;
    struct DC_node *next;
    struct DC_node *prev;
};

typedef struct DC_node *doubleCircularList;

typedef struct {
    int index;
    unsigned int shortestPathsSum;
    int pred;
} queue;

unsigned int readNumber_ui();
long int readNumber_l(); 
void min_heapify(queue *A, unsigned int n, unsigned int b);
void create_min_heap(queue *A, unsigned int b);
unsigned int delete_min(queue *A, unsigned int b);
void swap_ui(unsigned int *a, unsigned int *b); 
void swap_i(int *a, int *b); 
unsigned int dijkstra(unsigned int *m, long int d);
doubleCircularList insert_head(doubleCircularList l, unsigned int index, unsigned int shortestPathsSum);
doubleCircularList insert_in_order(doubleCircularList l, unsigned int index, unsigned int shortestPathsSum);
doubleCircularList removeQueue(doubleCircularList l);
void print2(doubleCircularList l, long int j);
void print1(doubleCircularList l, unsigned int j); 

int main() {
    doubleCircularList final = NULL; 
    int check_first_row = 0, same_matrix = 0;
    unsigned int i = 0, elem = 0, index1 = 0, sum = 0;
    long int d = 0, k = 0;
    char ch;

    d = readNumber_l();
    k = readNumber_l();

    unsigned int matrad[d][d];

    for (int count1 = 0; count1 < d; count1++) {
        for (int count2 = 0; count2 < d; count2++) {
            matrad[count1][count2] = 0;
        }
    }

    while ((ch = getchar_unlocked()) != EOF) {
        if (i == d) {
            sum = 0;
            check_first_row = 0;
            for (int bb = 1; bb < d; bb++) 
            {
                if (matrad[0][bb] != 0) {
                    check_first_row = 1;
                    break;
                }
            }
            if (check_first_row != 0) { 
                if (same_matrix == 0) 
                    sum = (d - 1) * matrad[0][0];
                else 
                    sum = dijkstra(&matrad[0][0], d);
            }
            if (index1 < k) {
                final = insert_in_order(final, index1, sum);
            } else {
                if (final->prev->shortestPathsSum > sum) {
                    final = removeQueue(final);
                    final = insert_in_order(final, index1, sum);
                }
            }
            index1++;
            i = 0; 
        }
        if (ch == 'A') {
            same_matrix = 0; 
            for (int l = 0; l < d; l++) {
                for (int ll = 0; ll < d; ll++) {
                    matrad[l][ll] = readNumber_ui(); 
                    if (l == 0 && ll == 0)
                        elem = matrad[0][0]; 
                    if (l == 0 && matrad[l][ll] != elem)
                        same_matrix = 1;
                }
            }
            i = d;
        } else if (ch == 'T') {
            if (index1 == 0) printf("\n");
            else if (index1 < k) { 
                print1(final, index1);
            } else {
                print2(final, k);
            }
            i = 0; 
        }
    }
    return 0;
}


void swap_ui(unsigned int *a, unsigned int *b) {
    unsigned int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_i(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(queue *A, unsigned int n, unsigned int b) {
    unsigned int posmax = 0, l = 2 * n + 1, r = 2 * n + 2;
    if (n == 0) {
        l = 1;
        r = 2;
    }
    if (l < b && A[l].shortestPathsSum < A[n].shortestPathsSum)
        posmax = l;
    else
        posmax = n;
    if (r < b && A[r].shortestPathsSum < A[posmax].shortestPathsSum)
        posmax = r;
    if (posmax != n) {
        swap_ui(&A[n].shortestPathsSum, &A[posmax].shortestPathsSum);
        swap_i(&A[n].index, &A[posmax].index);
        swap_i(&A[n].pred, &A[posmax].pred);
        min_heapify(A, posmax, b);
    }
}

void create_min_heap(queue *A, unsigned int b) {
    for (unsigned int i = (b - 1) / 2; i >= 0 && i <= (b - 1) / 2; i--) {
        min_heapify(A, i, b);
    }
}

unsigned int delete_min(queue *A, unsigned int b) {
    if (b > 0) {
        unsigned int min = A[0].shortestPathsSum;
        A[0].index = A[b - 1].index;
        A[0].pred = A[b - 1].pred;
        A[0].shortestPathsSum = A[b - 1].shortestPathsSum;
        b = b - 1;
        min_heapify(A, 0, b);
        return min;
    }
    return 0;
}

unsigned int dijkstra(unsigned int *m, long int d) {
    unsigned int ndis = 0, sum = 0, minimum = 0, empty = 0;
    queue queue[d];  

    queue[0].index = 0;
    queue[0].shortestPathsSum = 0;
    queue[0].pred = -1;
    for (int f = 1; f < d; f++) {
        queue[f].index = f;
        queue[f].shortestPathsSum = pow(2, 32) - 1;
        queue[f].pred = -1;
    }

    while (empty < d) {
        for (int f = 1; f < d - empty; f++) {
            while ((queue[0].index == queue[f].pred) && f < d - empty) {
                f++;
            }
            if (*(m + d * queue[0].index + queue[f].index) != 0) { 
                ndis = queue[0].shortestPathsSum + *(m + d * queue[0].index + queue[f].index); 
                if (queue[f].shortestPathsSum > ndis) {
                    queue[f].shortestPathsSum = ndis;
                    queue[f].pred = queue[0].index;
                }
            }
        }
        create_min_heap(queue, d - empty);
        minimum = delete_min(queue, d - empty);
        sum += minimum;
        empty++; 
    }
    return sum;
}

unsigned int readNumber_ui() {
    unsigned int a = 0;
    char ch = getchar_unlocked();
    while (ch < '0' || ch > '9') {
        ch = getchar_unlocked();
    }
    while (ch >= '0' && ch <= '9') {
        a = a * 10 + ch - 48;
        ch = getchar_unlocked();
    }
    return a;

}

long int readNumber_l() {
    long int a = 0;
    char ch = getchar_unlocked();
    while (ch < '0' || ch > '9') {
        ch = getchar_unlocked();
    }
    while (ch >= '0' && ch <= '9') {
        a = a * 10 + ch - 48;
        ch = getchar_unlocked();
    }
    return a;

}

doubleCircularList insert_head(doubleCircularList l, unsigned int index, unsigned int shortestPathsSum) {
    doubleCircularList temp = malloc(sizeof(struct DC_node));
    temp->index = index;
    temp->shortestPathsSum = shortestPathsSum;
    if (l == NULL) {
        temp->prev = temp;
        temp->next = temp;
        return temp;
    }
    if (l->next == NULL) {
        temp->next = l;
        l->prev = temp;
        temp->prev = l;
        l->next = temp;
        return temp;
    }
    temp->next = l;
    temp->prev = l->prev;
    doubleCircularList cur = l;
    cur = cur->prev;
    cur->next = temp;
    l->prev = temp;
    return temp;
}

doubleCircularList insert_in_order(doubleCircularList l, unsigned int index, unsigned int shortestPathsSum) {
    doubleCircularList cur = l;
    if (l == NULL || l->shortestPathsSum > shortestPathsSum) {
        return insert_head(l, index, shortestPathsSum);
    } else if (l->prev->shortestPathsSum < shortestPathsSum) {
        l->prev->next = insert_head(l->prev->next, index, shortestPathsSum);
        return l;
    } else {
        while (cur->next != l && (cur->next->shortestPathsSum) < shortestPathsSum) {
            cur = cur->next;
        }
        cur->next = insert_head(cur->next, index, shortestPathsSum);
        return l;
    }
}

doubleCircularList removeQueue(doubleCircularList l) {
    if (l != NULL) {
        if (l->next == l) {
            return NULL;
        } else {
            doubleCircularList temp = l->prev;
            doubleCircularList cur = l->prev->prev;
            cur->next = l;
            l->prev = l->prev->prev;
            free(temp);
        }
    }
    return l;
}

void print2(doubleCircularList l, long int j) {
    for (int i = 0; i < j - 1; i++) {
        printf("%u ", l->index);
        l = l->next;
    }
    printf("%u\n", l->index);
}

void print1(doubleCircularList l, unsigned int j) {
    for (int i = 0; i < j - 1; i++) {
        printf("%u ", l->index);
        l = l->next;
    }
    printf("%u\n", l->index);
}

