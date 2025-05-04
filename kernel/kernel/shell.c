// kernel/kernel/shell.c

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>
#include <kernel/string.h>    // your freestanding strcmp, memset

// inb helper
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Scancode → ASCII map (US, set 1)
static const char kbdus[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',  // Enter
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',
    0,  ' '/*space*/, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void shell_main(void) {
    char buffer[128];
    int  pos;

    // Initial prompt
    terminal_writestring("> ");

    while (true) {
        pos = 0;
        memset(buffer, 0, sizeof(buffer));

        // Read one line
        while (1) {
            // wait for scancode
            while (!(inb(0x64) & 1)) { }
            uint8_t sc = inb(0x60);
            if (sc & 0x80) continue;          // ignore key releases
            char c = kbdus[sc];
            if (!c) continue;

            if (c == '\n') {
                terminal_putchar('\n');
                buffer[pos] = '\0';
                break;
            }
            else if (c == '\b') {
                if (pos > 0) {
                    pos--;
                    terminal_putchar('\b');
                }
            }
            else {
                if (pos < (int)sizeof(buffer)-1) {
                    buffer[pos++] = c;
                    terminal_putchar(c);
                }
            }
        }

        // Command dispatch
        if (strcmp(buffer, "hello") == 0) {
            terminal_writestring("Hello World\n");
        }
        else if (strcmp(buffer, "tux") == 0) {
            static const char* tux_art[] = {
                "                 .88888888:.",
                "                88888888.88888.",
                "              .8888888888888888.",
                "              888888888888888888",
                "              88' _`88'_  `88888",
                "              88 88 88 88  88888",
                "              88_88_::_88_:88888",
                "              88:::,::,:::::8888",
                "              88`:::::::::'`8888",
                "             .88  `::::'    8:88.",
                "            8888            `8:888.",
                "          .8888'             `888888.",
                "         .8888:..  .::.  ...:'8888888:.",
                "        .8888.'     :'     `'::`88:88888",
                "       .8888        '         `.888:8888.",
                "      888:8         .           888:88888",
                "    .888:88        .:           888:88888:",
                "    8888888.       ::           88:888888",
                "    `.::.888.      ::          .88888888",
                "   .::::::.888.    ::         :::`8888'.:.",
                "  ::::::::::.888   '         .::::::::::::",
                "  ::::::::::::.8    '      .:8::::::::::::.",
                " .::::::::::::::.        .:888:::::::::::::",
                " :::::::::::::::88:.__..:88888:::::::::::'",
                "  `'.:::::::::::88888888888.88:::::::::'",
                "miK     `':::_:' -- '' -'-' `':_::::'`",
                NULL
            };
            for (int i = 0; tux_art[i]; i++) {
                terminal_writestring(tux_art[i]);
                terminal_putchar('\n');
            }
        }
        else if (strcmp(buffer, "clear") == 0) {
            // clear screen & reset cursor
            terminal_initialize();
            // Immediately print a fresh prompt
            terminal_writestring("> ");
            continue;  // skip the extra prompt at bottom
        }
        else if (buffer[0] != '\0') {
            terminal_writestring("Unknown command: ");
            terminal_writestring(buffer);
            terminal_putchar('\n');
        }

        // Blank line + prompt
        terminal_putchar('\n');
        terminal_writestring("> ");
    }
}
