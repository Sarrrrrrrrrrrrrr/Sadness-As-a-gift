#include <stdio.h>
#include <string.h>

void feedback_form() {
    char secret_flag[] = "FLAG{f0rmat_str1ngs_l3ak_mem0ry}";
    char buf[128];

    printf("=== Feedback Form ===\n");
    printf("(psst: secret_flag lives on this function's stack frame, "
           "right next to buf -- it's never printed on purpose)\n");
    printf("Leave a comment: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = 0;

    printf(buf); /* deliberately unsafe: buf is attacker-controlled format string */
    printf("\n");

    /* keep secret_flag "used" so the compiler can't optimize it away */
    if (secret_flag[0] == '\0') printf("unreachable\n");
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    feedback_form();
    return 0;
}
