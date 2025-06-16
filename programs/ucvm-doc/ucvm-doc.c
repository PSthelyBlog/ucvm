/* UCVM Documentation Reader
 * A comprehensive documentation browser for the Unified Claude-Mediated Virtual Machine
 * Compile: gcc -o ucvm-doc ucvm-doc.c
 * Usage: ./ucvm-doc [section] [search-term]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_SECTIONS 100
#define CLEAR_SCREEN "\033[2J\033[H"
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

typedef struct {
    char title[256];
    char* content;
    int line_start;
    int line_count;
} Section;

Section sections[MAX_SECTIONS];
int section_count = 0;
char* full_doc = NULL;

/* Documentation content embedded in the program */
const char* embedded_doc = 
"# Unified Claude-Mediated Virtual Machine (UCVM) Specification\n"
"\n"
"**Version:** 2.1\n"
"**Date:** 2024-01-15\n"
"**Purpose:** A unified specification combining instruction-level architecture with OS-level abstractions\n"
"\n"
"## 1. Introduction\n"
"\n"
"The Unified Claude-Mediated Virtual Machine (UCVM) is a mathematically rigorous, multi-level virtual machine that can operate at both hardware and operating system abstraction levels. It provides:\n"
"\n"
"- **Dual-mode operation**: Simplified (OS-level) and Full (hardware-level) modes\n"
"- **Seamless mode switching**: Transition between abstraction levels as needed\n"
"- **Educational value**: Expose CPU internals when desired\n"
"- **Practical efficiency**: Direct system call execution for common tasks\n"
"- **Full Unix compatibility**: Compliance with Single Unix Specification\n"
"\n"
"## 2. Mathematical Foundation\n"
"\n"
"The UCVM is defined as a state machine: **M = (Σ, I, O, δ, λ, C)**\n"
"\n"
"Where:\n"
"- **Σ**: State space (configurable abstraction level)\n"
"- **I**: Input space (natural language or formal operations)\n"
"- **O**: Output space (stdout, stderr, signals)\n"
"- **δ**: State transition function\n"
"- **λ**: Abstraction level controller\n"
"- **C**: Computational delegation function\n"
"\n"
"### 2.1 Notation\n"
"\n"
"- Sets: ℕ (naturals), ℤ (integers), 𝔹 = {0,1} (booleans)\n"
"- A → B: Function from A to B\n"
"- A × B: Cartesian product\n"
"- ℘(A): Power set of A\n"
"- ⊥: Undefined/null value\n"
"\n"
"## 3. State Space Definition\n"
"\n"
"### 3.1 Unified State Structure\n"
"\n"
"**Σ = (mode, output_mode, Σ_OS, Σ_HW, Σ_IO)**\n"
"\n"
"Where:\n"
"- **mode** ∈ {SIMPLIFIED, FULL}: Current abstraction level\n"
"- **output_mode** ∈ {RAW, VERBOSE, DEBUG}: Output verbosity level\n"
"- **Σ_OS**: Operating system state (always present)\n"
"- **Σ_HW**: Hardware state (active in FULL mode)\n"
"- **Σ_IO**: I/O subsystem state (shared)\n"
"\n"
"### 3.2 Operating System State\n"
"\n"
"**Σ_OS = (P, F, FS, K)**\n"
"\n"
"#### Process Table (P)\n"
"P: PID → ProcessState\n"
"\n"
"ProcessState = {\n"
"  pid: ℕ,                          // Process ID\n"
"  ppid: ℕ,                         // Parent PID\n"
"  state: ProcessStatus,            // RUNNING, READY, BLOCKED, ZOMBIE\n"
"  uid: ℕ,                         // User ID\n"
"  gid: ℕ,                         // Group ID\n"
"  context: Context,               // CPU context (registers/state)\n"
"  memory: MemoryMap,              // Virtual memory mapping\n"
"  fds: FD → FileHandle,           // File descriptor table\n"
"  cwd: Path,                      // Current working directory\n"
"  env: String → String,           // Environment variables\n"
"  signals: SignalState            // Signal handlers and pending\n"
"}\n"
"\n"
"## 4. System Calls\n"
"\n"
"### Process Management\n"
"| Number | Name | Signature | Description |\n"
"|--------|------|-----------|-------------|\n"
"| 0 | fork | () → pid_t | Create child process |\n"
"| 1 | exec | (path, argv, envp) → int | Execute program |\n"
"| 2 | exit | (status) → ∅ | Terminate process |\n"
"| 3 | wait | (status_ptr) → pid_t | Wait for child |\n"
"| 4 | getpid | () → pid_t | Get process ID |\n"
"\n"
"### File Operations\n"
"| Number | Name | Signature | Description |\n"
"|--------|------|-----------|-------------|\n"
"| 10 | open | (path, flags, mode) → fd | Open file |\n"
"| 11 | close | (fd) → int | Close file |\n"
"| 12 | read | (fd, buf, count) → ssize_t | Read data |\n"
"| 13 | write | (fd, buf, count) → ssize_t | Write data |\n"
"| 14 | lseek | (fd, offset, whence) → off_t | Seek position |\n"
"\n"
"## 5. Instruction Set (FULL mode)\n"
"\n"
"### Data Movement\n"
"| Opcode | Mnemonic | Operation |\n"
"|--------|----------|-----------||\n"
"| 0x01 | MOV r,r | Register move |\n"
"| 0x02 | MOV r,i | Immediate load |\n"
"| 0x03 | MOV r,[m] | Memory load |\n"
"| 0x04 | MOV [m],r | Memory store |\n"
"\n"
"### Arithmetic\n"
"| Opcode | Mnemonic | Operation |\n"
"|--------|----------|-----------||\n"
"| 0x10 | ADD r,r | Addition |\n"
"| 0x11 | SUB r,r | Subtraction |\n"
"| 0x12 | MUL r,r | Multiplication |\n"
"| 0x13 | DIV r,r | Division |\n"
"\n"
"### Control Flow\n"
"| Opcode | Mnemonic | Operation |\n"
"|--------|----------|-----------||\n"
"| 0x20 | JMP a | Jump |\n"
"| 0x21 | JZ a | Jump if zero |\n"
"| 0x22 | JNZ a | Jump not zero |\n"
"| 0x24 | CALL a | Call |\n"
"| 0x25 | RET | Return |\n"
"\n"
"## 6. Output Modes\n"
"\n"
"**Command Syntax**: `output [mode]`\n"
"\n"
"**Output Modes**:\n"
"- `RAW` (default): Pure program output only\n"
"- `VERBOSE`: Include Claude's explanations\n"
"- `DEBUG`: Full state visibility\n"
"\n"
"## 7. Mode Switching\n"
"\n"
"- `mode simplified` - Switch to OS-level\n"
"- `mode full` - Switch to hardware-level\n"
"- `mode auto` - Let Claude choose\n"
"\n"
"## 8. Quick Reference\n"
"\n"
"### Common Commands\n"
"- `ls` - List directory contents\n"
"- `cd <dir>` - Change directory\n"
"- `cat <file>` - Display file contents\n"
"- `echo <text>` - Print text\n"
"- `ps` - List processes\n"
"- `kill <pid>` - Terminate process\n"
"\n"
"### Debugging Commands\n"
"- `dump registers` - Show CPU state (FULL mode)\n"
"- `dump memory <addr>` - Show memory contents\n"
"- `syscall trace` - Trace system calls\n"
"- `breakpoint <addr>` - Set execution breakpoint\n"
"\n"
"## 9. Memory Layout\n"
"\n"
"Address Space Layout:\n"
"[0x0000, 0x1000) - Kernel space (protected)\n"
"[0x1000, 0x8000) - User text (code)\n"
"[0x8000, 0xC000) - User data/heap\n"
"[0xC000, 0xF000) - User stack\n"
"[0xF000, 0x10000) - Memory-mapped I/O\n"
"\n"
"## 10. Examples\n"
"\n"
"### Simple Command Execution\n"
"```\n"
"User: echo hello world\n"
"Output: hello world\n"
"```\n"
"\n"
"### Creating and Running a Program\n"
"```\n"
"User: cat > hello.c << EOF\n"
"#include <stdio.h>\n"
"int main() {\n"
"    printf(\"Hello UCVM!\\n\");\n"
"    return 0;\n"
"}\n"
"EOF\n"
"\n"
"User: gcc hello.c -o hello\n"
"User: ./hello\n"
"Output: Hello UCVM!\n"
"```\n"
"\n"
"### Assembly Programming (FULL mode)\n"
"```\n"
"User: mode full\n"
"User: asm\n"
"MOV r0, 5\n"
"MOV r1, 3\n"
"ADD r0, r1\n"
"```\n";

void init_documentation() {
    full_doc = strdup(embedded_doc);
    
    char* line = strtok(full_doc, "\n");
    int line_num = 0;
    Section* current_section = NULL;
    
    while (line != NULL) {
        if (line[0] == '#' && line[1] == ' ') {
            // Main section
            if (section_count < MAX_SECTIONS) {
                current_section = &sections[section_count++];
                strncpy(current_section->title, line + 2, 255);
                current_section->line_start = line_num;
                current_section->line_count = 0;
                current_section->content = line;
            }
        } else if (line[0] == '#' && line[1] == '#' && line[2] == ' ') {
            // Subsection
            if (section_count < MAX_SECTIONS) {
                current_section = &sections[section_count++];
                strncpy(current_section->title, line + 3, 255);
                current_section->line_start = line_num;
                current_section->line_count = 0;
                current_section->content = line;
            }
        }
        
        if (current_section) {
            current_section->line_count++;
        }
        
        line = strtok(NULL, "\n");
        line_num++;
    }
}

void print_header() {
    printf(CLEAR_SCREEN);
    printf(BOLD CYAN "╔══════════════════════════════════════════════════════════════╗\n");
    printf("║           UCVM Documentation Reader v1.0                     ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n" RESET);
}

void print_menu() {
    printf("\n" YELLOW "Commands:\n" RESET);
    printf("  " GREEN "list" RESET "     - List all sections\n");
    printf("  " GREEN "read <n>" RESET " - Read section number n\n");
    printf("  " GREEN "search" RESET "   - Search documentation\n");
    printf("  " GREEN "help" RESET "     - Show this help\n");
    printf("  " GREEN "quit" RESET "     - Exit reader\n\n");
}

void list_sections() {
    print_header();
    printf("\n" BOLD "Available Sections:\n" RESET);
    printf("───────────────────\n");
    
    for (int i = 0; i < section_count; i++) {
        printf("%2d. %s\n", i + 1, sections[i].title);
    }
    
    printf("\nUse 'read <number>' to view a section.\n");
}

void read_section(int section_num) {
    if (section_num < 1 || section_num > section_count) {
        printf("Invalid section number. Use 'list' to see available sections.\n");
        return;
    }
    
    print_header();
    Section* sect = &sections[section_num - 1];
    
    printf("\n" BOLD "%s\n" RESET, sect->title);
    printf("════════════════════════════════════════════════════════════════\n\n");
    
    // Reconstruct and print section content
    char* doc_copy = strdup(embedded_doc);
    char* line = strtok(doc_copy, "\n");
    int current_line = 0;
    int printing = 0;
    
    while (line != NULL) {
        if (current_line == sect->line_start) {
            printing = 1;
        }
        
        if (printing) {
            // Stop at next section
            if (current_line > sect->line_start && 
                line[0] == '#' && 
                (line[1] == ' ' || (line[1] == '#' && line[2] == ' '))) {
                break;
            }
            
            // Format output
            if (strncmp(line, "| ", 2) == 0) {
                // Table row
                printf("%s\n", line);
            } else if (strncmp(line, "**", 2) == 0) {
                // Bold text
                printf(BOLD "%s" RESET "\n", line);
            } else if (strncmp(line, "- ", 2) == 0) {
                // Bullet point
                printf("  • %s\n", line + 2);
            } else if (strncmp(line, "```", 3) == 0) {
                // Code block
                printf(GREEN "%s" RESET "\n", line);
            } else {
                printf("%s\n", line);
            }
        }
        
        line = strtok(NULL, "\n");
        current_line++;
    }
    
    free(doc_copy);
    printf("\n");
}

void search_documentation(const char* term) {
    print_header();
    printf("\n" BOLD "Search Results for '%s':\n" RESET, term);
    printf("═══════════════════════════════\n\n");
    
    char* doc_copy = strdup(embedded_doc);
    char* line = strtok(doc_copy, "\n");
    int line_num = 0;
    int found = 0;
    
    while (line != NULL) {
        char line_lower[MAX_LINE_LENGTH];
        char term_lower[256];
        
        // Convert to lowercase for case-insensitive search
        int i;
        for (i = 0; line[i] && i < MAX_LINE_LENGTH - 1; i++) {
            line_lower[i] = tolower(line[i]);
        }
        line_lower[i] = '\0';
        
        for (i = 0; term[i] && i < 255; i++) {
            term_lower[i] = tolower(term[i]);
        }
        term_lower[i] = '\0';
        
        if (strstr(line_lower, term_lower) != NULL) {
            // Find which section this line belongs to
            int section_idx = -1;
            for (i = 0; i < section_count; i++) {
                if (line_num >= sections[i].line_start &&
                    (i == section_count - 1 || line_num < sections[i + 1].line_start)) {
                    section_idx = i;
                    break;
                }
            }
            
            if (!found) {
                found = 1;
            }
            
            printf(CYAN "[Section %d: %s]" RESET "\n", 
                   section_idx + 1, 
                   section_idx >= 0 ? sections[section_idx].title : "Unknown");
            
            // Highlight search term
            char* pos = strstr(line_lower, term_lower);
            if (pos) {
                int offset = pos - line_lower;
                printf("  ");
                fwrite(line, 1, offset, stdout);
                printf(YELLOW BOLD);
                fwrite(line + offset, 1, strlen(term), stdout);
                printf(RESET);
                printf("%s\n\n", line + offset + strlen(term));
            }
        }
        
        line = strtok(NULL, "\n");
        line_num++;
    }
    
    if (!found) {
        printf("No results found for '%s'\n", term);
    }
    
    free(doc_copy);
}

int main(int argc, char* argv[]) {
    init_documentation();
    
    if (argc > 1) {
        // Command line mode
        if (strcmp(argv[1], "list") == 0) {
            list_sections();
        } else if (strcmp(argv[1], "search") == 0 && argc > 2) {
            search_documentation(argv[2]);
        } else if (isdigit(argv[1][0])) {
            read_section(atoi(argv[1]));
        } else {
            printf("Usage: %s [list|search <term>|<section-number>]\n", argv[0]);
        }
        return 0;
    }
    
    // Interactive mode
    print_header();
    print_menu();
    
    char command[256];
    while (1) {
        printf(GREEN "ucvm-doc> " RESET);
        fflush(stdout);
        
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }
        
        // Remove newline
        command[strcspn(command, "\n")] = 0;
        
        // Parse command
        char* cmd = strtok(command, " ");
        if (cmd == NULL) continue;
        
        if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        } else if (strcmp(cmd, "list") == 0) {
            list_sections();
        } else if (strcmp(cmd, "help") == 0) {
            print_header();
            print_menu();
        } else if (strcmp(cmd, "read") == 0) {
            char* arg = strtok(NULL, " ");
            if (arg && isdigit(arg[0])) {
                read_section(atoi(arg));
            } else {
                printf("Usage: read <section-number>\n");
            }
        } else if (strcmp(cmd, "search") == 0) {
            char* term = strtok(NULL, "");
            if (term) {
                search_documentation(term);
            } else {
                printf("Usage: search <term>\n");
            }
        } else {
            printf("Unknown command. Type 'help' for available commands.\n");
        }
    }
    
    free(full_doc);
    return 0;
}