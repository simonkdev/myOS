#include <stdio.h>
#include <kernel/tty.h>

// Declare shell_main (defined in shell.c)
extern void shell_main(void);

void kernel_main(void) {
    terminal_initialize();
    printf("Hello, kernel World!\n");
    // Enter the shell loop
    shell_main();
}
