#include <stdlib.h>

#include <stdio.h>

#include <time.h>

#define N 20
// 364 5 9 4 8 100 25
// 573 75 25 2 5 8 7
int nb_num;
int tab[N];
int stack_ptr;
int stack_op[N];
int stack_res[60];
int result;
int solutions;
int best;

int big[4] = {
    100,
    75,
    50,
    25
};
int little[20];

int nbig = 2;

int find(int n, int i1, int a, int b, int op) {
    int i, j;
    int c;
    // printf("%d %d %d %d %c\n", n, i1, a, b, op);
    //printf("stack: %d\n", op);
    //printf("nb: %d\n", nb_num);
    if (stack_ptr >= 0) {
        stack_res[3 * stack_ptr] = a;
        stack_op[stack_ptr] = op;
        stack_res[3 * stack_ptr + 1] = b;
        stack_res[3 * stack_ptr + 2] = n;
        if (n == result) {
            solutions++;
            //printf("Solution difficulty: %d\n", stack_ptr+1);
            //printf("Solution difficulty: %d\n", nb_num);
            if (best > stack_ptr) {
                best = stack_ptr;
                printf("New best: %d ops\n", best + 1);

                for (i = 0; i <= stack_ptr; i++) {
                    printf("%d %c %d = %d\n",
                        stack_res[3 * i], stack_op[i],
                        stack_res[3 * i + 1], stack_res[3 * i + 2]);
                }
            }
            if (best == 1)
                return 1;
        }
        // return 1;
        tab[i1] = n;
    }

    for (i = 0; i < nb_num; i++) {
        for (j = i + 1; j < nb_num; j++) {
            a = tab[i];
            b = tab[j];
            //printf("a: %d\n", a);
            // printf("b: %d\n", b);
            if (a > 0 && b > 0) {

                tab[j] = 0;
                stack_ptr++;

                if (find(a + b, i, a, b, '+'))
                    return 1;
                if (a - b != b && b < a)
                    if (find(a - b, i, a, b, '-'))
                        return 1;
                if (a < b)
                    if (find(b - a, i, b, a, '-'))
                        return 1;
                if (a > 1 && b > 1 && a * b < 1000)
                    if (find(a * b, i, a, b, '*'))
                        return 1;
                if (b > 1 && b <= a) {
                    c = a / b;
                    if ((float) c == (float) a / b)
                        if (find(c, i, a, b, '/'))
                            return 1;
                }

                if (a > 1 && a <= b) {
                    c = b / a;
                    if ((float) c == (float) a / b)
                        if (find(c, i, b, a, '/'))
                            return 1;
                }

                stack_ptr--;
                tab[i] = a;
                tab[j] = b;
            }
        }
    }

    return 0;
}

void shuffle(int * array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

int main(int argc, char ** argv) {
    int i, res, p;
    for (i = 0; i < 10; i++) {
        little[i] = little[i + 10] = i + 1;
    }

    srand(time(NULL));


    shuffle(little, 20);
    shuffle(big, 4);
    //for(i=0;i<20;i++) {
    //printf("little: %d\n", little[i]);
    //}
    solutions = 0;
    best = 9;
    if (argc < 3) {
        if (argc > 2) {
            printf("usage: %s: result numbers...\n"
                "Try to find result from numbers with the 4 basic operations.\n", argv[0]);
            exit(1);
        }
    }

    p = 1;
    if (argc > 3) {
        result = atoi(argv[p]);

        nb_num = 0;
        for (i = p + 1; i < argc; i++) {
            tab[nb_num++] = atoi(argv[i]);
        }
    } else {
        result = (rand() % 900) + 100;

        if (argc == 2)
            nbig = atoi(argv[1]);

        if (nbig < 0 || nbig > 4)
            nbig = 2;
        printf("big numbers: %d\n", nbig);

        for (i = 0; i < nbig; i++)
            tab[nb_num++] = big[i];

        for (; i < 6; i++)
            tab[nb_num++] = little[i];
    }
    stack_ptr = -1;
    printf("result=%d\n", result);
    printf("Numbers:\n");
    for (i = 0; i < nb_num; i++)
        printf("%d ", tab[i]);
    printf("\n");
    res = find(0, 0, 0, 0, ' ');
    printf("Done\n");

    if (best < 9) {
        for (i = 0; i <= stack_ptr; i++) {
            printf("%d %c %d = %d\n",
                stack_res[3 * i], stack_op[i],
                stack_res[3 * i + 1], stack_res[3 * i + 2]);
        }

    }
    printf("Solutions found: %d\n", solutions);
    printf("best: %d\n", best + 1);
    if (best == 9) {
        printf("Impossible\n");
        return 1;
    }

    return 0;

}
