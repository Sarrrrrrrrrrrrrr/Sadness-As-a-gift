#include <stdio.h>
#include <string.h>
#include <unistd.h>

void win() {
    printf("\n[+] You hijacked control flow. Flag: FLAG{stack_smash_1s_fun}\n");
    fflush(stdout);
}

void vulnerable_function() {
    char buffer[64];
    printf("Enter your name: ");
    fflush(stdout);
    scanf("%s", buffer); /* deliberately unsafe: no bounds checking, no width limit */
    printf("Hello, %s!\n", buffer);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("=== Guestbook v1.0 ===\n");
    vulnerable_function();
    printf("Goodbye.\n");
    return 0;
}
