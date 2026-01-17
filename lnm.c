#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 8

struct num {
    long base;
    char digits[];
};

struct num *nums[MAX];

void menu() {
    puts("\n--- Santa Base Converter ---");
    puts("L) Load number");
    puts("D) Delete number");
    puts("C) Convert base");
    puts("S) Show numbers");
    puts("V) Verify flag");
    puts("Q) Quit");
    printf("> ");
}

void load_num() {
    int idx;
    char buf[128];
    long base;

    printf("Index (0-7): ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX || nums[idx]) return;

    printf("Base (10 or 16): ");
    scanf("%ld", &base);
    if (base != 10 && base != 16) return;

    printf("Number: ");
    scanf("%127s", buf);

    size_t len = strlen(buf);
    struct num *n = malloc(sizeof(long) + len);

    n->base = base;
    strcpy(n->digits, buf);

    nums[idx] = n;
}

void delete_num() {
    int idx;
    printf("Index: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX || !nums[idx]) return;

    free(nums[idx]);
    nums[idx] = NULL;
}

void show_nums() {
    for (int i = 0; i < MAX; i++) {
        if (nums[i]) {
            printf("Slot %d | Base %ld | Number %s\n",
                   i, nums[i]->base, nums[i]->digits);
        }
    }
}

void convert_num() {
    int idx;
    printf("Index: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX || !nums[idx]) return;

    struct num *n = nums[idx];
    char tmp[128];

    if (n->base == 16) {
        long val = strtol(n->digits, NULL, 16);
        sprintf(tmp, "%ld", val);   // ⚠️ LONGER
        n->base = 10;
    } else {
        long val = strtol(n->digits, NULL, 10);
        sprintf(tmp, "%lx", val);   // ⚠️ SHORTER
        n->base = 16;
    }

    // ❌ Heap overflow
    strcpy(n->digits, tmp);
}

void verify_flag() {
    FILE *f = fopen("FLAG.txt", "r");
    if (!f) {
        puts("Flag missing!");
        return;
    }

    // Allocate FLAG ON HEAP
    char *buf = malloc(80);
    fread(buf, 1, 79, f);
    buf[79] = '\0';
    fclose(f);

    // 👇 STORE POINTER INTO nums[]
    nums[7] = (struct num *)buf;

    puts("Flag verified.");
}

int main() {
    setbuf(stdout, NULL);
    memset(nums, 0, sizeof(nums));

    char choice;
    while (1) {
        menu();
        scanf(" %c", &choice);

        switch (choice) {
            case 'L': load_num(); break;
            case 'D': delete_num(); break;
            case 'C': convert_num(); break;
            case 'S': show_nums(); break;
            case 'V': verify_flag(); break;
            case 'Q': return 0;
        }
    }
}
