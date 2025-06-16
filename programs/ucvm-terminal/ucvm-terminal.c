/* UCVM Terminal Emulator
 * Provides ANSI/VT100 terminal emulation for programs running in UCVM
 * Compile: gcc -o term ucvm-terminal.c
 * Usage: ./term <command> [args...]
 * Example: ./term ./ucvm-doc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#define BUFFER_SIZE 4096
#define MAX_PARAMS 16
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

/* Terminal state */
typedef struct {
    int cursor_x;
    int cursor_y;
    int saved_cursor_x;
    int saved_cursor_y;
    int foreground_color;
    int background_color;
    int bold;
    int underline;
    int reverse;
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1];
    char attr[SCREEN_HEIGHT][SCREEN_WIDTH];
} TerminalState;

TerminalState term;

/* ANSI color codes to simple representation */
enum {
    COLOR_BLACK = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_DEFAULT = 9
};

/* Initialize terminal state */
void init_terminal() {
    term.cursor_x = 0;
    term.cursor_y = 0;
    term.saved_cursor_x = 0;
    term.saved_cursor_y = 0;
    term.foreground_color = COLOR_DEFAULT;
    term.background_color = COLOR_DEFAULT;
    term.bold = 0;
    term.underline = 0;
    term.reverse = 0;
    
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        memset(term.screen[y], ' ', SCREEN_WIDTH);
        term.screen[y][SCREEN_WIDTH] = '\0';
        memset(term.attr[y], 0, SCREEN_WIDTH);
    }
}

/* Clear screen */
void clear_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        memset(term.screen[y], ' ', SCREEN_WIDTH);
        memset(term.attr[y], 0, SCREEN_WIDTH);
    }
    term.cursor_x = 0;
    term.cursor_y = 0;
}

/* Move cursor */
void move_cursor(int x, int y) {
    if (x < 0) x = 0;
    if (x >= SCREEN_WIDTH) x = SCREEN_WIDTH - 1;
    if (y < 0) y = 0;
    if (y >= SCREEN_HEIGHT) y = SCREEN_HEIGHT - 1;
    term.cursor_x = x;
    term.cursor_y = y;
}

/* Scroll screen up by one line */
void scroll_up() {
    for (int y = 0; y < SCREEN_HEIGHT - 1; y++) {
        memcpy(term.screen[y], term.screen[y + 1], SCREEN_WIDTH);
        memcpy(term.attr[y], term.attr[y + 1], SCREEN_WIDTH);
    }
    memset(term.screen[SCREEN_HEIGHT - 1], ' ', SCREEN_WIDTH);
    memset(term.attr[SCREEN_HEIGHT - 1], 0, SCREEN_WIDTH);
}

/* Put character at current cursor position */
void put_char(char c) {
    if (c == '\n') {
        term.cursor_x = 0;
        term.cursor_y++;
        if (term.cursor_y >= SCREEN_HEIGHT) {
            scroll_up();
            term.cursor_y = SCREEN_HEIGHT - 1;
        }
    } else if (c == '\r') {
        term.cursor_x = 0;
    } else if (c == '\t') {
        term.cursor_x = ((term.cursor_x / 8) + 1) * 8;
        if (term.cursor_x >= SCREEN_WIDTH) {
            term.cursor_x = 0;
            term.cursor_y++;
            if (term.cursor_y >= SCREEN_HEIGHT) {
                scroll_up();
                term.cursor_y = SCREEN_HEIGHT - 1;
            }
        }
    } else if (c >= 32 && c < 127) {
        if (term.cursor_x < SCREEN_WIDTH && term.cursor_y < SCREEN_HEIGHT) {
            term.screen[term.cursor_y][term.cursor_x] = c;
            
            /* Store attributes */
            char attr = 0;
            if (term.bold) attr |= 0x01;
            if (term.underline) attr |= 0x02;
            if (term.reverse) attr |= 0x04;
            attr |= (term.foreground_color & 0x0F) << 4;
            term.attr[term.cursor_y][term.cursor_x] = attr;
            
            term.cursor_x++;
            if (term.cursor_x >= SCREEN_WIDTH) {
                term.cursor_x = 0;
                term.cursor_y++;
                if (term.cursor_y >= SCREEN_HEIGHT) {
                    scroll_up();
                    term.cursor_y = SCREEN_HEIGHT - 1;
                }
            }
        }
    }
}

/* Process CSI (Control Sequence Introducer) sequences */
void process_csi(const char* seq) {
    int params[MAX_PARAMS] = {0};
    int param_count = 0;
    const char* p = seq;
    
    /* Parse parameters */
    while (*p && param_count < MAX_PARAMS) {
        if (isdigit(*p)) {
            params[param_count] = atoi(p);
            while (isdigit(*p)) p++;
            param_count++;
        }
        if (*p == ';') p++;
        else if (!isdigit(*p)) break;
    }
    
    /* Default parameter values */
    if (param_count == 0) {
        params[0] = 1;
        param_count = 1;
    }
    
    /* Process command */
    switch (*p) {
        case 'A': /* Cursor up */
            move_cursor(term.cursor_x, term.cursor_y - params[0]);
            break;
            
        case 'B': /* Cursor down */
            move_cursor(term.cursor_x, term.cursor_y + params[0]);
            break;
            
        case 'C': /* Cursor forward */
            move_cursor(term.cursor_x + params[0], term.cursor_y);
            break;
            
        case 'D': /* Cursor backward */
            move_cursor(term.cursor_x - params[0], term.cursor_y);
            break;
            
        case 'H': /* Cursor position */
        case 'f':
            if (param_count >= 2) {
                move_cursor(params[1] - 1, params[0] - 1);
            } else {
                move_cursor(0, 0);
            }
            break;
            
        case 'J': /* Erase display */
            if (params[0] == 2) {
                clear_screen();
            }
            break;
            
        case 'K': /* Erase line */
            if (params[0] == 0) { /* Clear to end of line */
                for (int x = term.cursor_x; x < SCREEN_WIDTH; x++) {
                    term.screen[term.cursor_y][x] = ' ';
                    term.attr[term.cursor_y][x] = 0;
                }
            }
            break;
            
        case 'm': /* Set graphics mode */
            for (int i = 0; i < param_count; i++) {
                switch (params[i]) {
                    case 0: /* Reset */
                        term.bold = 0;
                        term.underline = 0;
                        term.reverse = 0;
                        term.foreground_color = COLOR_DEFAULT;
                        term.background_color = COLOR_DEFAULT;
                        break;
                    case 1: /* Bold */
                        term.bold = 1;
                        break;
                    case 4: /* Underline */
                        term.underline = 1;
                        break;
                    case 7: /* Reverse */
                        term.reverse = 1;
                        break;
                    case 30: case 31: case 32: case 33:
                    case 34: case 35: case 36: case 37:
                        term.foreground_color = params[i] - 30;
                        break;
                    case 39: /* Default foreground */
                        term.foreground_color = COLOR_DEFAULT;
                        break;
                    case 40: case 41: case 42: case 43:
                    case 44: case 45: case 46: case 47:
                        term.background_color = params[i] - 40;
                        break;
                    case 49: /* Default background */
                        term.background_color = COLOR_DEFAULT;
                        break;
                }
            }
            break;
            
        case 's': /* Save cursor position */
            term.saved_cursor_x = term.cursor_x;
            term.saved_cursor_y = term.cursor_y;
            break;
            
        case 'u': /* Restore cursor position */
            term.cursor_x = term.saved_cursor_x;
            term.cursor_y = term.saved_cursor_y;
            break;
    }
}

/* Process ANSI escape sequences */
int process_escape(const char* input, int len) {
    if (len < 2) return 0;
    
    if (input[1] == '[') {
        /* CSI sequence */
        int i = 2;
        while (i < len && i < 20) {
            if ((input[i] >= 'A' && input[i] <= 'Z') ||
                (input[i] >= 'a' && input[i] <= 'z')) {
                process_csi(input + 2);
                return i + 1;
            }
            i++;
        }
    }
    
    return 0;
}

/* Render terminal screen to output */
void render_screen() {
    /* Simple rendering - could be enhanced with color support */
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        int last_char = SCREEN_WIDTH - 1;
        while (last_char >= 0 && term.screen[y][last_char] == ' ') {
            last_char--;
        }
        
        if (last_char >= 0) {
            for (int x = 0; x <= last_char; x++) {
                /* Check for special attributes */
                char attr = term.attr[y][x];
                if (attr & 0x01) { /* Bold */
                    /* Could output special marker or use actual bold */
                }
                
                putchar(term.screen[y][x]);
            }
        }
        
        /* Don't print newline for last line if it's empty */
        if (y < SCREEN_HEIGHT - 1 || last_char >= 0) {
            putchar('\n');
        }
    }
}

/* Process output from child process */
void process_output(const char* buffer, int len) {
    int i = 0;
    
    while (i < len) {
        if (buffer[i] == '\033') {
            /* Escape sequence */
            int seq_len = process_escape(buffer + i, len - i);
            if (seq_len > 0) {
                i += seq_len;
            } else {
                /* Incomplete sequence or not recognized */
                put_char(buffer[i]);
                i++;
            }
        } else {
            /* Regular character */
            put_char(buffer[i]);
            i++;
        }
    }
}

/* Create pipe and fork child process */
int run_with_terminal(char* argv[]) {
    int pipefd[2];
    pid_t pid;
    
    /* Create pipe for capturing child output */
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {
        /* Child process */
        close(pipefd[0]); /* Close read end */
        
        /* Redirect stdout and stderr to pipe */
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        
        /* Execute the program */
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }
    
    /* Parent process */
    close(pipefd[1]); /* Close write end */
    
    /* Read from child and process output */
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    /* Make pipe non-blocking for better interactivity */
    int flags = fcntl(pipefd[0], F_GETFL, 0);
    fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);
    
    int status;
    while (1) {
        bytes_read = read(pipefd[0], buffer, BUFFER_SIZE);
        
        if (bytes_read > 0) {
            process_output(buffer, bytes_read);
        } else if (bytes_read == -1 && errno != EAGAIN) {
            break;
        }
        
        /* Check if child has exited */
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == pid) {
            /* Read any remaining data */
            while ((bytes_read = read(pipefd[0], buffer, BUFFER_SIZE)) > 0) {
                process_output(buffer, bytes_read);
            }
            break;
        }
        
        /* Small delay to prevent busy waiting */
        usleep(1000);
    }
    
    close(pipefd[0]);
    
    /* Render final screen */
    render_screen();
    
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

/* Interactive terminal mode */
int interactive_terminal() {
    printf("UCVM Terminal Emulator - Interactive Mode\n");
    printf("Type 'exit' to quit\n\n");
    
    char command[256];
    while (1) {
        printf("term> ");
        fflush(stdout);
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        /* Remove newline */
        command[strcspn(command, "\n")] = 0;
        
        if (strcmp(command, "exit") == 0) {
            break;
        }
        
        /* Parse command into argv */
        char* argv[64];
        int argc = 0;
        char* token = strtok(command, " ");
        
        while (token != NULL && argc < 63) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;
        
        if (argc > 0) {
            init_terminal();
            run_with_terminal(argv);
            printf("\n");
        }
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        /* Interactive mode */
        return interactive_terminal();
    }
    
    /* Initialize terminal state */
    init_terminal();
    
    /* Run the specified program with terminal emulation */
    return run_with_terminal(argv + 1);
}