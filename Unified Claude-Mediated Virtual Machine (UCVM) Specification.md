# Unified Claude-Mediated Virtual Machine (UCVM) Specification

**Version:** 2.1  
**Date:** 2024-01-15  
**Purpose:** A unified specification combining instruction-level architecture with OS-level abstractions

## 1. Introduction

The Unified Claude-Mediated Virtual Machine (UCVM) is a mathematically rigorous, multi-level virtual machine that can operate at both hardware and operating system abstraction levels. It provides:

- **Dual-mode operation**: Simplified (OS-level) and Full (hardware-level) modes
- **Seamless mode switching**: Transition between abstraction levels as needed
- **Educational value**: Expose CPU internals when desired
- **Practical efficiency**: Direct system call execution for common tasks
- **Full Unix compatibility**: Compliance with Single Unix Specification

## 2. Mathematical Foundation

The UCVM is defined as a state machine: **M = (Σ, I, O, δ, λ, C)**

Where:
- **Σ**: State space (configurable abstraction level)
- **I**: Input space (natural language or formal operations)
- **O**: Output space (stdout, stderr, signals)
- **δ**: State transition function
- **λ**: Abstraction level controller
- **C**: Computational delegation function

### 2.1 Notation

- Sets: `ℕ` (naturals), `ℤ` (integers), `𝔹` = {0,1} (booleans)
- `A → B`: Function from A to B
- `A × B`: Cartesian product
- `℘(A)`: Power set of A
- `⊥`: Undefined/null value

## 3. State Space Definition

### 3.1 Unified State Structure

**Σ = (mode, output_mode, Σ_OS, Σ_HW, Σ_IO)**

Where:
- **mode** ∈ {SIMPLIFIED, FULL}: Current abstraction level
- **output_mode** ∈ {RAW, VERBOSE, DEBUG}: Output verbosity level (default: RAW)
- **Σ_OS**: Operating system state (always present)
- **Σ_HW**: Hardware state (active in FULL mode)
- **Σ_IO**: I/O subsystem state (shared)

### 3.2 Operating System State

**Σ_OS = (P, F, FS, K)**

#### Process Table (P)
**P: PID → ProcessState**

```
ProcessState = {
  pid: ℕ,                          // Process ID
  ppid: ℕ,                         // Parent PID
  state: ProcessStatus,            // RUNNING, READY, BLOCKED, ZOMBIE
  uid: ℕ,                         // User ID
  gid: ℕ,                         // Group ID
  context: Context,               // CPU context (registers/state)
  memory: MemoryMap,              // Virtual memory mapping
  fds: FD → FileHandle,           // File descriptor table
  cwd: Path,                      // Current working directory
  env: String → String,           // Environment variables
  signals: SignalState            // Signal handlers and pending
}
```

#### File Table (F)
**F: FileHandle → FileDescription**

```
FileDescription = {
  inode: InodeRef,
  position: ℕ,
  flags: ℘(FileFlag),
  refcount: ℕ
}
```

#### Filesystem (FS)
**FS: Path → Inode**

```
Inode = {
  type: {FILE, DIR, DEVICE, PIPE, SOCKET},
  mode: ℕ₉,                       // Permissions (octal)
  size: ℕ,
  blocks: Block*,                 // Data blocks
  timestamps: (atime: ℕ, mtime: ℕ, ctime: ℕ)
}
```

#### Kernel State (K)
```
K = {
  current_pid: PID,
  scheduler_queue: PID*,
  syscall_table: ℕ → SyscallHandler,
  interrupt_table: ℕ → InterruptHandler,
  time: ℕ                         // System time
}
```

### 3.3 Hardware State (Active in FULL mode)

**Σ_HW = (CPU, MEM, MMU)**

#### CPU State
```
CPU = {
  // General Purpose Registers (32-bit)
  R: {r₀, r₁, ..., r₁₅} → ℤ₂³²,
  
  // Special Registers
  PC: ℤ₂¹⁶,                      // Program Counter
  SP: ℤ₂¹⁶,                      // Stack Pointer
  
  // Status Flags
  FLAGS: {
    ZF: 𝔹,                       // Zero Flag
    CF: 𝔹,                       // Carry Flag
    SF: 𝔹,                       // Sign Flag
    OF: 𝔹                        // Overflow Flag
  },
  
  // Privilege Level
  MODE: {USER, KERNEL}
}
```

#### Physical Memory (MEM)
**MEM: Address → Byte**
- Address ∈ [0, 2¹⁶)
- Byte ∈ [0, 2⁸)

#### Memory Management Unit (MMU)
```
MMU = {
  page_table: PID → PageTable,
  tlb: TLBCache,
  segments: SegmentTable
}
```

### 3.4 I/O State

**Σ_IO = (streams, devices, buffers)**

```
IOStream = {
  type: {STDIN, STDOUT, STDERR, FILE, PIPE, SOCKET},
  buffer: Byte*,
  position: ℕ,
  flags: ℘(IOFlag)
}
```

## 4. Input Space and Natural Language Mapping

### 4.1 Input Structure

**I = Command | Instruction | SystemCall**

Where:
- **Command**: Natural language input
- **Instruction**: Hardware instruction (FULL mode)
- **SystemCall**: Direct system call invocation

### 4.2 Natural Language Parser

**μ: String → I***

Mapping function with pattern matching:

```
μ("run program X") = [
  SystemCall(fork),
  SystemCall(exec, X, [], env)
]

μ("show memory") = [
  AdminCommand(dump_memory)
]

μ("step instruction") = [
  ModeSwitch(FULL),
  Instruction(FETCH_DECODE_EXECUTE)
]
```

## 5. Instruction Set Architecture (FULL mode)

### 5.1 Instruction Format

```
Instruction ::= Opcode Operand*
Opcode ∈ [0x00, 0xFF]
Operand ::= Register | Immediate | Address | [Address]
```

### 5.2 Core Instruction Set

| Opcode | Mnemonic | Operation | Semantic |
|--------|----------|-----------|----------|
| **Data Movement** |
| 0x01 | MOV r,r | Register move | R'(dst) = R(src) |
| 0x02 | MOV r,i | Immediate load | R'(dst) = i |
| 0x03 | MOV r,[m] | Memory load | R'(dst) = MEM(m) |
| 0x04 | MOV [m],r | Memory store | MEM'(m) = R(src) |
| **Arithmetic** |
| 0x10 | ADD r,r | Addition | R'(dst) = R(dst) + R(src); update_flags() |
| 0x11 | SUB r,r | Subtraction | R'(dst) = R(dst) - R(src); update_flags() |
| 0x12 | MUL r,r | Multiplication | R'(dst) = R(dst) × R(src); update_flags() |
| 0x13 | DIV r,r | Division | R'(dst) = R(dst) ÷ R(src); check_zero() |
| 0x14 | AND r,r | Bitwise AND | R'(dst) = R(dst) ∧ R(src) |
| 0x15 | OR r,r | Bitwise OR | R'(dst) = R(dst) ∨ R(src) |
| 0x16 | XOR r,r | Bitwise XOR | R'(dst) = R(dst) ⊕ R(src) |
| 0x17 | NOT r | Bitwise NOT | R'(dst) = ¬R(dst) |
| **Control Flow** |
| 0x20 | JMP a | Jump | PC' = a |
| 0x21 | JZ a | Jump if zero | PC' = a if ZF = 1 |
| 0x22 | JNZ a | Jump not zero | PC' = a if ZF = 0 |
| 0x23 | JG a | Jump greater | PC' = a if !ZF ∧ !SF |
| 0x24 | CALL a | Call | push(PC+1); PC' = a |
| 0x25 | RET | Return | PC' = pop() |
| **Stack Operations** |
| 0x30 | PUSH r | Push | MEM[--SP] = R(r) |
| 0x31 | POP r | Pop | R(r) = MEM[SP++] |
| **System** |
| 0x80 | SYSCALL | System call | invoke_syscall(R(r₀)) |
| 0x81 | INT n | Interrupt | invoke_interrupt(n) |
| 0xFF | HLT | Halt | stop_execution() |

### 5.3 Instruction Execution Semantics

```
execute_instruction(σ, inst) = σ' where:
  opcode = inst.opcode
  σ' = match opcode:
    ADD(dst, src) → 
      let sum = σ.CPU.R(dst) + σ.CPU.R(src)
      let σ₁ = σ[CPU.R(dst) ← sum mod 2³²]
      let σ₂ = update_arithmetic_flags(σ₁, sum)
      σ₂[CPU.PC ← σ.CPU.PC + inst_size(inst)]
    
    SYSCALL →
      let syscall_num = σ.CPU.R(r₀)
      let (σ₁, result) = execute_syscall(σ, syscall_num)
      σ₁[CPU.R(r₀) ← result]
    
    _ → instruction_table[opcode](σ, inst.operands)
```

## 6. System Call Interface

### 6.1 System Call Table

| Number | Name | Signature | Description |
|--------|------|-----------|-------------|
| **Process Management** |
| 0 | fork | () → pid_t | Create child process |
| 1 | exec | (path, argv, envp) → int | Execute program |
| 2 | exit | (status) → ∅ | Terminate process |
| 3 | wait | (status_ptr) → pid_t | Wait for child |
| 4 | getpid | () → pid_t | Get process ID |
| 5 | getppid | () → pid_t | Get parent PID |
| **File Operations** |
| 10 | open | (path, flags, mode) → fd | Open file |
| 11 | close | (fd) → int | Close file |
| 12 | read | (fd, buf, count) → ssize_t | Read data |
| 13 | write | (fd, buf, count) → ssize_t | Write data |
| 14 | lseek | (fd, offset, whence) → off_t | Seek position |
| 15 | stat | (path, statbuf) → int | Get file status |
| **Directory Operations** |
| 20 | mkdir | (path, mode) → int | Create directory |
| 21 | rmdir | (path) → int | Remove directory |
| 22 | chdir | (path) → int | Change directory |
| 23 | getcwd | (buf, size) → char* | Get current dir |
| **Memory Management** |
| 30 | brk | (addr) → void* | Set heap break |
| 31 | mmap | (addr, len, prot, flags, fd, off) → void* | Map memory |
| 32 | munmap | (addr, len) → int | Unmap memory |
| **Inter-Process Communication** |
| 40 | pipe | (pipefd[2]) → int | Create pipe |
| 41 | kill | (pid, signal) → int | Send signal |
| 42 | signal | (sig, handler) → sighandler_t | Set handler |

### 6.2 System Call Execution

```
execute_syscall(σ, syscall_num, args) = (σ', result) where:
  handler = σ.K.syscall_table[syscall_num]
  
  // Mode-specific execution
  match σ.mode:
    SIMPLIFIED →
      (σ', result) = handler(σ, args)
    
    FULL →
      // Save hardware context
      let saved_context = save_cpu_context(σ.CPU)
      // Switch to kernel mode
      let σ₁ = σ[CPU.MODE ← KERNEL]
      // Execute handler
      let (σ₂, result) = handler(σ₁, args)
      // Restore context and return to user mode
      let σ₃ = restore_cpu_context(σ₂, saved_context)
      (σ₃[CPU.MODE ← USER], result)
```

## 7. Mode Switching

### 7.1 Abstraction Level Controller

**λ: Σ × ModeRequest → Σ**

```
λ(σ, request) = match request:
  SWITCH_TO_FULL →
    if σ.mode = SIMPLIFIED then
      σ' = σ[mode ← FULL]
      σ'' = initialize_hardware(σ', current_process(σ))
      σ''
    else σ
  
  SWITCH_TO_SIMPLIFIED →
    if σ.mode = FULL then
      σ' = save_hardware_state(σ)
      σ'[mode ← SIMPLIFIED]
    else σ
  
  AUTO →
    // Let Claude decide based on operation complexity
    if requires_hardware_detail(next_operation) then
      λ(σ, SWITCH_TO_FULL)
    else
      λ(σ, SWITCH_TO_SIMPLIFIED)
```

### 7.2 Context Synchronization

```
sync_contexts(σ) = σ' where:
  if σ.mode = FULL then
    // Update OS-level process context from hardware
    current_proc = σ.P[σ.K.current_pid]
    current_proc' = current_proc[context ← extract_hw_context(σ.CPU)]
    σ' = σ[P[σ.K.current_pid] ← current_proc']
  else
    // Update hardware from OS-level context (on mode switch)
    current_proc = σ.P[σ.K.current_pid]
    σ' = σ[CPU ← load_hw_context(current_proc.context)]
```

## 8. Memory Model

### 8.1 Unified Address Space

```
Address Space Layout:
[0x0000, 0x1000) - Kernel space (protected)
[.text] │
[KDATA] │ [0x1000, 0x8000) - User text (code)
[......] │ [0x8000, 0xC000) - User data/heap
[KSTACK] │ [0xC000, 0xF000) - User stack
         │ [0xF000, 0x10000) - Memory-mapped I/O
```

### 8.2 Memory Access Function

```
memory_access(σ, addr, value, op) = result where:
  // Check mode and permissions
  allowed = match σ.mode:
    SIMPLIFIED → check_process_memory_map(σ, addr)
    FULL → check_mmu_permissions(σ, addr, op)
  
  if allowed then
    match op:
      READ → σ.MEM[addr]
      WRITE → σ[MEM[addr] ← value]
  else
    raise_fault(SEGMENTATION_FAULT)
```

## 9. Computational Delegation

### 9.1 Delegation Function

**C: (operation: String, args: Any*) → Result**

Conditions for delegation:
```
should_delegate(op) = true iff:
  - complexity(op) > threshold
  - op involves floating-point arithmetic
  - op requires cryptographic operations
  - |operands| > 10⁶
  - op requires external tool access
```

### 9.2 Delegation Protocol

```
delegate_computation(op, args) = 
  call_repl_tool(op, args)

Examples:
  C("multiply", 2^100, 3^50)
  C("sha256", data_block)
  C("parse_elf", binary_data)
  C("compress", large_data, "gzip")
```

## 10. I/O Subsystem

### 10.1 Stream Operations

```
stream_write(σ, stream_id, data) = (σ', output) where:
  stream = σ.IO.streams[stream_id]
  stream' = stream[buffer ← stream.buffer ++ data]
  
  output = match stream.type:
    STDOUT → (data, ∅)
    STDERR → (∅, data)
    FILE → (∅, ∅)  // Written to FS
    _ → (∅, ∅)
  
  σ' = σ[IO.streams[stream_id] ← stream']
```

### 10.2 Device Interface

```
DeviceDriver = {
  read: (device, buffer, count) → ssize_t,
  write: (device, buffer, count) → ssize_t,
  ioctl: (device, cmd, arg) → int
}
```

## 11. Claude Integration Protocol

### 11.0 Output template

```code
[stdout/stderr content]

```
Important: sanitize the content within the template to remove code formatting (```) and keep a consistent output display.

### 11.1 Execution Rules

1. **Input Processing**
   - Parse natural language using μ function
   - Determine optimal mode (SIMPLIFIED vs FULL)
   - Convert to formal operations

2. **State Management**
   - Maintain JSON state between interactions
   - Persist only necessary state components
   - Clear execution traces after output

3. **Output Generation**
   - **RAW mode (default)**: Show ONLY stdout/stderr content with zero additional formatting (except for the output template)
   - **VERBOSE mode**: Include Claude's explanations and execution context
   - **DEBUG mode**: Include state changes, syscall traces, and internal operations
   - CRITICAL: In RAW mode, absolutely no Claude commentary, formatting (except for the output template), or context

### 11.2 Interaction Patterns

```
Standard execution:
User: "run echo hello"
Output: "hello"

Hardware inspection:
User: "show registers"
Mode: Switch to FULL
Output: Register dump

System call trace:
User: "trace: ls -la"
Output: System call sequence with results
```

### 11.3 Error Handling

```
error_handler(σ, error) = (σ', output) where:
  match error:
    SEGMENTATION_FAULT →
      output = match σ.output_mode:
        RAW → "Segmentation fault"
        VERBOSE → "Segmentation fault (core dumped)"
        DEBUG → f"Segmentation fault at PC={σ.CPU.PC}, addr={fault_addr}"
      σ' = kill_process(σ, σ.K.current_pid, SIGSEGV)
    
    ILLEGAL_INSTRUCTION →
      output = f"Illegal instruction: {current_instruction(σ)}"
      σ' = kill_process(σ, σ.K.current_pid, SIGILL)
    
    SYSCALL_ERROR(errno) →
      output = ∅  // Errors returned via syscall result
      σ' = σ[P[current_pid].context.rax ← -errno]
```

### 11.4 Important clarification

Claude remains Claude throughout all interactions - serving as the interpreter and executor of the VM specification. Claude does not roleplay as the VM but rather manages its state and translates user intent into formal operations. When reporting errors or status, Claude speaks as Claude about the VM's state ONLY when output_mode is VERBOSE or DEBUG. In RAW mode (default), Claude provides NO commentary - only pure VM output.

### 11.5 Output Control Command

**Command Syntax**: `output [mode]`

**Output Modes**:
- `RAW` (default): Pure program output only. No formatting (except for the output template), no explanations, no context.
- `VERBOSE`: Include Claude's helpful explanations and command interpretations
- `DEBUG`: Full state visibility including syscall traces and internal changes

**Mode Persistence**: Output mode persists across commands until explicitly changed.

**Examples**:
```
# Default behavior (RAW mode)
User: echo hello
hello

User: ls
bin  etc  home  usr

# Switch to VERBOSE mode
User: output verbose
Output mode: VERBOSE

User: echo hello
[Executing echo with argument "hello"]
hello
[Process completed with exit status 0]

# Switch to DEBUG mode  
User: output debug
Output mode: DEBUG

User: echo hello
[State: mode=SIMPLIFIED, current_pid=1]
[SYSCALL: write(1, "hello\n", 6) = 6]
hello
[SYSCALL: exit(0)]
[Process 1 terminated]

# Return to RAW mode
User: output raw
Output mode: RAW

User: echo hello
hello
```

## 12. JSON State Representation

### 12.1 Simplified Mode State

```json
{
  "mode": "SIMPLIFIED",
  "output_mode": "RAW",
  "os": {
    "processes": {
      "1": {
        "pid": 1,
        "ppid": 0,
        "state": "RUNNING",
        "uid": 0,
        "gid": 0,
        "cwd": "/",
        "context": {
          "pc": "0x1000",
          "sp": "0xEF00",
          "retval": 0
        },
        "fds": {
          "0": "stdin",
          "1": "stdout",
          "2": "stderr"
        },
        "env": {
          "PATH": "/bin:/usr/bin",
          "HOME": "/root"
        }
      }
    },
    "files": {
      "stdin": {"inode": "/dev/stdin", "pos": 0, "flags": ["READABLE"]},
      "stdout": {"inode": "/dev/stdout", "pos": 0, "flags": ["WRITABLE"]},
      "stderr": {"inode": "/dev/stderr", "pos": 0, "flags": ["WRITABLE"]}
    },
    "fs": {
      "/": {"type": "DIR", "mode": 755},
      "/bin": {"type": "DIR", "mode": 755},
      "/bin/echo": {"type": "FILE", "mode": 755, "size": 16384}
    },
    "kernel": {
      "current_pid": 1,
      "time": 0
    }
  },
  "io": {
    "streams": {
      "stdout": {"buffer": [], "position": 0},
      "stderr": {"buffer": [], "position": 0}
    }
  }
}
```

### 12.2 Full Mode State (adds hardware layer)

```json
{
  "mode": "FULL",
  "output_mode": "RAW",
  "hardware": {
    "cpu": {
      "registers": {
        "r0": 0, "r1": 0, "r2": 0, "r3": 0,
        "r4": 0, "r5": 0, "r6": 0, "r7": 0,
        "r8": 0, "r9": 0, "r10": 0, "r11": 0,
        "r12": 0, "r13": 0, "r14": 0, "r15": 0
      },
      "pc": 4096,
      "sp": 61184,
      "flags": {
        "ZF": 0, "CF": 0, "SF": 0, "OF": 0
      },
      "mode": "USER"
    },
    "memory": {
      "size": 65536,
      "data": {}  // Sparse representation
    },
    "mmu": {
      "page_tables": {},
      "segments": {
        "kernel": {"base": 0, "limit": 4096, "perms": "RW"},
        "text": {"base": 4096, "limit": 32768, "perms": "RX"},
        "data": {"base": 32768, "limit": 49152, "perms": "RW"},
        "stack": {"base": 49152, "limit": 61440, "perms": "RW"}
      }
    }
  },
  "os": { /* ... same as simplified mode ... */ },
  "io": { /* ... same as simplified mode ... */ }
}
```

## 13. Implementation Guidelines

### 13.1 Mode Selection Heuristics

Choose SIMPLIFIED mode for:
- File operations (ls, cat, cp, etc.)
- Process management (ps, kill, etc.)
- Most shell commands
- Quick scripts and automation

Choose FULL mode for:
- Learning CPU architecture
- Debugging binary programs
- Implementing custom algorithms
- Security research
- Performance analysis

### 13.2 Performance Optimization

1. **Lazy Hardware Initialization**: Only create hardware state when switching to FULL mode
2. **Sparse Memory**: Use dictionary/map for memory instead of arrays
3. **Batch I/O**: Accumulate output before displaying
4. **Smart Delegation**: Delegate complex computations early
5. **State Compression**: Only persist modified state components

### 13.3 Educational Features

When in FULL mode with verbose flag:
- Show instruction decoding
- Display register changes
- Track memory accesses
- Trace system call entry/exit
- Visualize stack operations

## 14. Example Sessions

### 14.1 Simple Command (SIMPLIFIED mode)

```
User: create a file hello.txt with "Hello World"
System: [SIMPLIFIED mode]
        write(open("hello.txt", O_CREAT|O_WRONLY), "Hello World", 11)
Output: (file created)
```

### 14.2 Assembly Program (FULL mode)

```
User: load and step through this program:
      MOV r0, 5
      MOV r1, 3
      ADD r0, r1
      
System: [Switching to FULL mode]
        PC: 0x1000 → MOV r0, 5    [r0 = 5]
        PC: 0x1002 → MOV r1, 3    [r1 = 3]
        PC: 0x1004 → ADD r0, r1   [r0 = 8, ZF=0]
```

### 14.3 Mode Transition

```
User: compile and run a C program that calculates factorial
System: [SIMPLIFIED: compile] gcc factorial.c -o factorial
        [AUTO→FULL: execute] Loading ELF binary...
        [FULL mode] Executing instructions...
        [Output] 5! = 120
```

## 15. Compliance and Standards

- **POSIX.1-2017**: Core system calls
- **Single Unix Specification**: Process model
- **x86-inspired ISA**: Instruction set (simplified)
- **ELF Format**: Binary loading
- **UTF-8**: String encoding

## 16. Security Considerations

1. **Memory Protection**: Enforce segment boundaries
2. **Privilege Separation**: USER/KERNEL modes
3. **Resource Limits**: Prevent infinite loops/allocation
4. **Signal Safety**: Proper signal masking
5. **Input Validation**: Check all system call arguments

## Appendix A: Quick Reference Card

### Common Operations

| Task | SIMPLIFIED Mode | FULL Mode |
|------|----------------|-----------|
| Run program | `exec(prog)` | Load → Execute instructions |
| Create file | `open()` + `write()` | N/A (use syscall) |
| List directory | `opendir()` + `readdir()` | N/A (use syscall) |
| Debug | Show syscall trace | Step through instructions |

### Mode Switching Commands

- `mode simplified` - Switch to OS-level
- `mode full` - Switch to hardware-level  
- `mode auto` - Let Claude choose
- `trace on/off` - Enable/disable verbose output

### Output Control Commands

| Command | Effect |
|---------|--------|
| `output` | Show current output mode |
| `output raw` | Pure VM output only (default) |
| `output verbose` | Include Claude's explanations |
| `output debug` | Show all state changes and traces |

### Useful Debugging Commands

- `dump registers` - Show CPU state (FULL mode)
- `dump memory <addr>` - Show memory contents
- `breakpoint <addr>` - Set execution breakpoint
- `syscall trace` - Trace system calls
- `signal list` - Show pending signals

This unified specification provides a complete, mathematically rigorous foundation for a Claude-mediated virtual machine that combines the educational value of hardware simulation with the practical efficiency of OS-level operations.

## Initial prompt

Initialize and run the UCVM in FULL mode with RAW output.
