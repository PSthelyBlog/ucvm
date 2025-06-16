# Unified Claude-Mediated Virtual Machine (UCVM)

A mathematically rigorous, dual-mode virtual machine that operates through natural language, combining hardware-level simulation with OS-level abstractions.

## Overview

UCVM is a unique virtual machine where Claude AI serves as the interpreter, translating natural language commands into formal mathematical operations on a JSON-based VM state. Unlike traditional VMs, UCVM can seamlessly switch between:

- **SIMPLIFIED mode**: Direct OS-level operations for practical tasks
- **FULL mode**: Complete hardware simulation for educational purposes

The entire system is specified using formal mathematics rather than implementation code, ensuring precision and verifiability.

## Key Features

- **🧮 Mathematical Foundation**: Formally specified as a state machine M = (Σ, I, O, δ, λ, C)
- **🔄 Dual-Mode Operation**: Switch between OS-level and hardware-level abstractions
- **💬 Natural Language Interface**: Control the VM using plain English
- **🐧 Unix Compliance**: Full Single Unix Specification compatibility
- **📚 Educational Value**: Step through CPU instructions, inspect registers, trace system calls
- **⚡ Intelligent Execution**: Automatic delegation of complex computations to REPL
- **🔒 Security**: Memory protection, privilege separation, resource limits

## Quick Start

### Basic Usage

Simply describe what you want to do in natural language:

```
User: create a file hello.txt containing "Hello, World!"
Output: (file created)

User: run echo "Hello from UCVM"
Output: Hello from UCVM

User: list files in the current directory
Output: hello.txt
```

### Mode Switching

```
User: mode full
System: [Switched to FULL mode - hardware simulation active]

User: mode simplified  
System: [Switched to SIMPLIFIED mode - OS-level operations]

User: mode auto
System: [AUTO mode - Claude will choose optimal mode]
```

### Output Control

UCVM supports three output modes to control verbosity:

```
User: output raw      # Default - pure program output only
Output mode: RAW

User: output verbose  # Include Claude's helpful explanations
Output mode: VERBOSE

User: output debug   # Show all state changes and traces
Output mode: DEBUG
```

## Architecture

### System Layers

```
┌─────────────────────────────────┐
│   Natural Language Interface    │ ← You are here
├─────────────────────────────────┤
│        Claude Interpreter       │ ← Translates intent to operations
├─────────────────────────────────┤
│    Mathematical Specification   │ ← Formal state machine
├─────────────────┬───────────────┤
│  SIMPLIFIED Mode    │ FULL Mode │
│  (OS Abstractions)  │ (Hardware)│
├─────────────────────┴───────────┤
│      JSON State Representation  │
└─────────────────────────────────┘
```

### State Components

- **OS State**: Processes, files, filesystem, kernel
- **Hardware State**: CPU registers, memory, MMU (FULL mode only)
- **I/O State**: Streams, devices, buffers

### Memory Layout

```
[0x0000-0x1000)  Kernel Space (protected)
[0x1000-0x8000)  User Code (.text)
[0x8000-0xC000)  User Data/Heap
[0xC000-0xF000)  User Stack
[0xF000-0x10000) Memory-mapped I/O
```

## Programs Repository

UCVM includes a growing collection of utility programs designed specifically for the virtual machine environment. These programs are available in the `programs/` directory:

### Available Programs

#### 📖 **ucvm-doc** - Documentation Reader
An interactive documentation browser that provides complete access to the UCVM specification without leaving the VM environment.

```bash
# Interactive mode
./ucvm-doc

# Read specific section
./ucvm-doc 4  # View system calls

# Search documentation
./ucvm-doc search "memory"
```

Features:
- Full UCVM v2.1 specification embedded
- Color-coded formatting
- Search functionality
- Section navigation

#### 🖥️ **ucvm-terminal** - Terminal Emulator
Enables proper display of programs that use ANSI escape sequences for colors and formatting.

```bash
# Run a program with terminal emulation
./term ./ucvm-doc

# Interactive terminal mode
./term
```

Features:
- 80x24 character display
- ANSI/VT100 escape sequence support
- Color parsing and cursor control
- Compatible with programs expecting terminal features

### Installing Programs

Programs can be compiled within UCVM:

```bash
# Compile the documentation reader
gcc -o /bin/ucvm-doc programs/ucvm-doc/ucvm-doc.c

# Compile the terminal emulator
gcc -o /bin/term programs/ucvm-terminal/ucvm-terminal.c

# Create convenient aliases
echo 'alias docs="/bin/ucvm-doc"' >> ~/.bashrc
echo 'alias vt100="/bin/term"' >> ~/.bashrc
```

### Contributing Programs

The programs repository welcomes contributions. Guidelines:
- Write in standard C (C99 compatible)
- Include comprehensive README.md
- Design for UCVM's environment
- Follow Unix philosophy (do one thing well)
- Test in both SIMPLIFIED and FULL modes

## Examples

### Running Programs

```
User: compile and run a Hello World C program
System: [SIMPLIFIED] Compiling hello.c...
        [AUTO→FULL] Loading ELF binary...
        [FULL] Executing instructions...
Output: Hello, World!
```

### Assembly Programming

```
User: load this assembly program:
      MOV r0, 42
      MOV r1, 8  
      ADD r0, r1
      SYSCALL    ; exit with code in r0

System: [FULL mode]
        PC: 0x1000 → MOV r0, 42    [r0 = 42]
        PC: 0x1002 → MOV r1, 8     [r1 = 8]
        PC: 0x1004 → ADD r0, r1    [r0 = 50]
        PC: 0x1006 → SYSCALL       [exit(50)]
Output: Process exited with code 50
```

### System Call Tracing

```
User: trace system calls while running ls
System: [Trace enabled]
        open("/", O_RDONLY) = 3
        getdents(3, ...) = 48
        write(1, "bin\netc\nhome\n", 13) = 13
        close(3) = 0
Output: bin
        etc
        home
```

### Debugging

```
User: set breakpoint at address 0x1004 and run
System: [Breakpoint set at 0x1004]
        [Executing... stopped at breakpoint]
        PC: 0x1004, Next: ADD r0, r1
        Registers: r0=42, r1=8

User: step
System: [Executed ADD r0, r1]
        PC: 0x1006, r0=50, Flags: ZF=0
```

## Advanced Features

### Complex Computation Delegation

When operations exceed Claude's pattern-matching capabilities:

```
User: calculate 2^100 * 3^50
System: [Delegating to computational engine...]
Output: 515377520732011331036461129765621272702107522001
```

### Process Management

```
User: fork a child process that counts to 5
System: [Process 2 created]
        [Process 2]: 1
        [Process 2]: 2
        [Process 2]: 3
        [Process 2]: 4
        [Process 2]: 5
        [Process 2 exited]
```

### File System Operations

```
User: create directory /home/user and navigate there
System: mkdir("/home/user", 0755) = 0
        chdir("/home/user") = 0
        
User: show current directory
Output: /home/user
```

## Command Reference

### Mode Control
- `mode simplified` - OS-level operations only
- `mode full` - Enable hardware simulation
- `mode auto` - Automatic mode selection

### Output Control
- `output` - Show current output mode
- `output raw` - Pure VM output only (default)
- `output verbose` - Include Claude's explanations
- `output debug` - Show all state changes

### Debugging Commands
- `dump registers` - Show CPU state (FULL mode)
- `dump memory <addr>` - Inspect memory contents
- `breakpoint <addr>` - Set execution breakpoint
- `trace on/off` - Enable/disable verbose output
- `syscall trace` - Trace system calls only

### System Commands
- `ps` - List processes
- `kill <pid>` - Terminate process
- `signal <pid> <sig>` - Send signal
- `memory stats` - Show memory usage

## Important Notes

1. **Claude's Role**: Claude acts as the VM interpreter, not the VM itself. Claude translates your commands into VM operations while maintaining its identity as an AI assistant.

2. **State Persistence**: The VM state is maintained across messages in the conversation as JSON. Each interaction builds upon the previous state.

3. **Output Format**: By default (RAW mode), only stdout/stderr are shown. Use VERBOSE or DEBUG modes for Claude's explanations.

4. **Performance**: Complex calculations (cryptography, large numbers, compression) are automatically delegated to computational tools.

## Technical Specifications

- **Architecture**: 16-bit address space, 32-bit registers
- **Instruction Set**: RISC-like with 31 core instructions
- **System Calls**: POSIX-compliant (fork, exec, open, read, write, etc.)
- **Memory**: 64KB total, segmented protection
- **Process Model**: Preemptive multitasking with signals

## Contributing

This is a theoretical VM specification implemented through Claude's capabilities. To experiment with UCVM:

1. Start a conversation with Claude
2. Share the UCVM specification document
3. Begin issuing commands

To contribute programs or improvements:
1. Follow the mathematical specification rigorously
2. Test in both SIMPLIFIED and FULL modes
3. Document your additions thoroughly
4. Ensure Unix compatibility

## Version History

- **v2.1** (2024-01-15): Current version with enhanced output control
- **v2.0** (2024-01-15): Unified dual-mode architecture
- **v1.0** (2023-12-01): Initial hardware-only version

## License

This specification is provided for educational and research purposes.

---

*UCVM demonstrates how formal mathematical specifications can create precise, verifiable systems while maintaining practical usability through natural language interfaces.*
