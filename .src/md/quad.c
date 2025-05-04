#include <stdio.h>
#include <string.h>

#define MAX 10

typedef struct {
    char op[5];
    char arg1[10];
    char arg2[10];
    char result[10];
} Quadruple;

int is_commutative(const char* op) {
    return strcmp(op, "+") == 0 || strcmp(op, "*") == 0 || strcmp(op, "==") == 0;
}

int is_valid_operator(const char* op) {
    return strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
           strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
           strcmp(op, "=") == 0 || strcmp(op, "==") == 0;
}

int is_same_expr(Quadruple* q1, Quadruple* q2) {
    if (strcmp(q1->op, q2->op) != 0)
        return 0;

    if (is_commutative(q1->op)) {
        return (strcmp(q1->arg1, q2->arg1) == 0 && strcmp(q1->arg2, q2->arg2) == 0) ||
               (strcmp(q1->arg1, q2->arg2) == 0 && strcmp(q1->arg2, q2->arg1) == 0);
    } else {
        return strcmp(q1->arg1, q2->arg1) == 0 && strcmp(q1->arg2, q2->arg2) == 0;
    }
}

int is_valid_quad(Quadruple* q) {
    if (!is_valid_operator(q->op)) {
        printf(" Error: Unsupported operator '%s'. Skipping this quadruple.\n", q->op);
        return 0;
    }

    if (strlen(q->arg1) == 0 || strlen(q->result) == 0) {
        printf(" Error: Missing operand or result in quadruple (%s, %s, %s, %s). Skipping.\n",
               q->op, q->arg1, q->arg2, q->result);
        return 0;
    }

    return 1;
}

int main() {
    // Hardcoded quadruples (some invalid on purpose for demo)
    Quadruple quads[MAX] = {
        {"+", "a", "b", "t1"},     // valid
        {"*", "t1", "c", "t2"},    // valid
        {"+", "a", "b", "t3"},     // duplicate of t1
        {"-", "t3", "d", "t4"},    // valid
        {"=", "t4", "", "result"}  // valid
    };

    Quadruple optimized[MAX];
    int n = 5;
    int optimized_count = 0;

    // Print input quadruples
    printf("\n Input Quadruples:\n");
    for (int i = 0; i < n; i++) {
        printf("%s, %s, %s, %s\n", quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }

    for (int i = 0; i < n; i++) {
        if (!is_valid_quad(&quads[i]))
            continue;  // skip invalid

        int found = 0;
        if (strcmp(quads[i].op, "=") != 0) {
            for (int j = 0; j < optimized_count; j++) {
                if (is_same_expr(&quads[i], &optimized[j])) {
                    strcpy(optimized[optimized_count].op, "=");
                    strcpy(optimized[optimized_count].arg1, optimized[j].result);
                    strcpy(optimized[optimized_count].arg2, "");
                    strcpy(optimized[optimized_count].result, quads[i].result);
                    optimized_count++;
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            optimized[optimized_count++] = quads[i];
        }
    }

    printf("\n Optimized Quadruples:\n");
    for (int i = 0; i < optimized_count; i++) {
        printf("%s, %s, %s, %s\n", optimized[i].op, optimized[i].arg1,
               optimized[i].arg2, optimized[i].result);
    }

    return 0;
}
