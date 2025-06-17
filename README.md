# UCVM - Unified Claude-Mediated Virtual Machine

[![Version](https://img.shields.io/badge/version-3.0-blue.svg)](https://github.com/ucvm/ucvm)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Claude Compatible](https://img.shields.io/badge/claude-compatible-purple.svg)](https://claude.ai)

## Note from the developer

This revised version of the UCVM system provides a more robust architecture designed to be fully modular and extensible, and allows a more consistent behavior for the system state management and User Experience.
The major improvements:
* Core system module with constraint-based execution
* State management with mandatory JSON artifacts
* System call interface with delegation support
* Extensible plugin architecture
This establishes UCVM as a constraint mechanism for deterministic, auditable AI execution through mandatory state tracking and hardware simulation. The modular design allows independent component updates while maintaining system integrity.

## Overview

UCVM is a constraint-based virtual machine that ensures deterministic, auditable AI execution through explicit state management. By requiring Claude to trace every operation through a simulated hardware layer, UCVM creates a uniquely transparent and reproducible computing environment.

### 🎯 Core Innovation

Unlike traditional VMs that prioritize performance, UCVM uses hardware simulation as a **constraint mechanism** to ensure that every computational step is:
- **Explicit** - No hidden operations or implicit state changes
- **Traceable** - Complete execution history through state transitions  
- **Deterministic** - Same input + state always produces same output
- **Auditable** - All operations visible in the state artifact

## Features

### 🔄 Dual-Mode Operation
- **SIMPLIFIED Mode**: Hardware execution invisible to users (but still enforced)
- **FULL Mode**: Complete hardware visibility for debugging and learning

### 📝 Natural Language Interface
- Execute commands in plain English: `"search for Python tutorials"`
- Seamless translation to system calls and hardware operations
- Context-aware command interpretation

### 🔧 Modular Architecture
- Independent, extensible modules
- Clean interfaces for adding new capabilities
- Plugin system for custom extensions

### 🌐 Integrated Delegation
- **Web Search**: Access real-time information via `search` command
- **REPL/Analysis**: Execute JavaScript for complex computations
- **Extensible**: Add new external services through the delegation framework

### 📊 Complete State Persistence
- Every state change tracked in JSON artifacts
- Perfect reproducibility across sessions
- Built-in checkpointing and recovery

## Why UCVM?

### The Problem
Traditional AI assistants operate as "black boxes" - you give them input and get output, but the intermediate steps are opaque. This makes it difficult to:
- Verify correctness of complex operations
- Reproduce exact execution sequences
- Teach computer science concepts effectively
- Build trust in AI-assisted computation

### The Solution
UCVM constrains Claude to execute all operations through a simulated hardware layer, making every computational step explicit and verifiable. This creates:

1. **Trustworthy Execution** - See exactly how results are computed
2. **Perfect Reproducibility** - Same state + input = same output, always
3. **Educational Transparency** - Learn by watching real execution
4. **Audit Trail** - Complete history of all operations

## Requirements

- **Claude (Anthropic)** - UCVM is designed specifically for Claude
- **Artifacts enabled** - For state persistence via JSON artifacts
- **Web Search** (optional) - For search functionality
- **REPL/Analysis tool** (optional) - For calc functionality

## Quick Start

### 1. Initialize UCVM

Simply inject the UCVM specification document to the context (add from Github, or add local file, or copy/paste)

Claude will:
1. Create a JSON state artifact
2. Initialize the system with process 1 (init)
3. Display the terminal prompt

You'll see:
```code
UCVM 3.0 initialized
$ 
```

### 2. Basic Usage

Terminal output in code blocks:
```code
$ echo "Hello, UCVM!"
Hello, UCVM!

$ ls
bin  etc  home  usr  tmp

$ search "latest AI news"
[Searching web...]
[Results displayed]

$ calc "Math.sqrt(2) * Math.PI"
[Executing in REPL...]
4.442882938158366
```

### 3. Mode Switching

```code
# Switch to FULL mode to see hardware details
$ mode full
Switched to FULL mode

# View CPU registers
$ dump registers
r0: 0x0000  r1: 0x0000  r2: 0x0000  r3: 0x0000
r4: 0x0000  r5: 0x0000  r6: 0x0000  r7: 0x0000
PC: 0x1000  SP: 0xEF00  FLAGS: ----

# Return to SIMPLIFIED mode
$ mode simplified
Switched to SIMPLIFIED mode
```

### 4. Output Modes

```code
# Default RAW mode - clean terminal output
$ output raw
Output mode: RAW

# VERBOSE mode - includes Claude's helpful feedback  
$ output verbose
Output mode: VERBOSE
[Claude: Now I'll include explanations with commands]

# DEBUG mode - full state visibility
$ output debug
Output mode: DEBUG
[State changes will be shown for all operations]
```

## Architecture

### System Components

```
┌─────────────────────────────────────────┐
│        Claude Integration Layer          │
├─────────────────────────────────────────┤
│          Core System Module             │
├─────┬─────┬─────┬─────┬─────┬─────┬────┤
│State│Modes│ ISA │SysCall│Memory│Proc │I/O│
├─────┴─────┴─────┴─────┴─────┴─────┴────┤
│         Delegation Module               │
│    (Web Search, REPL, Extensions)       │
└─────────────────────────────────────────┘
```

### Key Modules

1. **State Management** - Mandatory JSON artifact tracking
2. **Execution Modes** - Control hardware visibility
3. **Instruction Set** - x86-inspired RISC architecture
4. **System Calls** - POSIX-compatible interface
5. **Memory Management** - Segmented 64KB address space
6. **Process Management** - Multi-process with context switching
7. **I/O Subsystem** - Streams, files, and devices
8. **Delegation Services** - External tool integration
9. **Natural Language** - Command parsing and translation
10. **Extension System** - Plugin architecture

## Examples

### Running a Program
```code
$ cat > hello.c
#include <stdio.h>
int main() {
    printf("Hello from UCVM!\n");
    return 0;
}
^D
$ gcc hello.c -o hello
$ ./hello
Hello from UCVM!
```

### Web Search Integration
```code
$ search "weather in Paris"
[Searching web...]
Current weather in Paris: 15°C, partly cloudy
```

### Complex Computation via REPL
```code
$ calc "Array.from({length: 10}, (_, i) => i ** 2)"
[Executing JavaScript...]
[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
```

### Process Management
```code
$ ps
PID  PPID  STATE     COMMAND
1    0     RUNNING   init
2    1     RUNNING   bash

$ sleep 10 &
[1] 3
$ ps
PID  PPID  STATE     COMMAND
1    0     RUNNING   init
2    1     RUNNING   bash
3    2     SLEEPING  sleep

$ kill 3
[1]+ Terminated    sleep 10
```

## Design Philosophy

1. **Constraint Over Performance** - Hardware simulation ensures explicit state management
2. **Transparency Over Efficiency** - Every operation is visible and traceable
3. **Determinism Over Flexibility** - Reproducible execution is paramount

## Contributing

We welcome contributions! Areas of interest:

- 🔌 **Extensions**: Network stack, graphics subsystem, audio devices
- 🛠️ **Tools**: Debuggers, visualizers, development utilities
- 🧪 **Testing**: Test suites, benchmarks, validation tools

## FAQ

### Q: Is UCVM a real virtual machine?
A: UCVM is a specification for constrained AI execution. It requires Claude to simulate a virtual machine, ensuring deterministic and traceable computation. While not a traditional VM, it provides real computational capabilities through Claude.

### Q: Can I run real Linux programs?
A: UCVM implements a subset of POSIX system calls and can run simple C programs compiled for its architecture. Complex programs requiring features like networking, threads, or GUI are not yet supported (but can be added via extensions).

### Q: Why only 64KB of memory?
A: The 64KB limit is intentional - it keeps the state manageable and ensures all operations remain traceable. Future versions may offer extended memory options.

### Q: Can I save and restore sessions?
A: Yes! The JSON state artifact contains the complete system state. You can save it and restore it in a new conversation with Claude to continue where you left off.

### Q: How does it compare to QEMU/VirtualBox?
A: Traditional VMs optimize for performance and run on real hardware. UCVM optimizes for transparency and constraint, running through Claude's interpretation. This makes it slower but provides unique benefits for education, debugging, and AI behavior analysis.

## License

UCVM is released under the MIT License. See [LICENSE](LICENSE) for details.

## Acknowledgments

UCVM was designed to explore new approaches to AI constraint and computer science education. Special thanks to the Claude team at Anthropic for making this possible.

---

**Note**: UCVM requires Claude (Anthropic) to function. It is not a standalone virtual machine but rather a specification for constrained AI execution.

## Citation

If you use UCVM in your research, please cite:
```bibtex
@software{ucvm2025,
  title = {UCVM: Unified Claude-Mediated Virtual Machine},
  version = {3.0},
  year = {2025},
  url = {https://github.com/PSthelyBlog/ucvm}
}
```
