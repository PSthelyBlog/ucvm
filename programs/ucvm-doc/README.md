# UCVM Documentation Reader (OBSOLETE PROGRAM)

A comprehensive documentation browser for the Unified Claude-Mediated Virtual Machine (UCVM) specification. This program provides an interactive interface to navigate, search, and read the complete UCVM documentation directly within the UCVM environment.

## Features

- 📚 **Complete Documentation**: Full UCVM specification embedded in the binary
- 🔍 **Search Functionality**: Case-insensitive search with highlighted results
- 🎨 **Formatted Output**: Color-coded display with proper formatting
- 📑 **Section Navigation**: Easy browsing through numbered sections
- 💻 **Dual Modes**: Interactive shell and command-line interface
- 🚀 **Zero Dependencies**: Pure C implementation, no external files needed

## Installation

### Compile from Source

```bash
gcc -o ucvm-doc ucvm-doc.c
```

### Make Executable

```bash
chmod +x ucvm-doc
```

## Usage

### Interactive Mode

Launch the program without arguments to enter interactive mode:

```bash
./ucvm-doc
```

Interactive commands:
- `list` - Display all documentation sections
- `read <n>` - Read section number n
- `search <term>` - Search for a term in the documentation
- `help` - Show available commands
- `quit` - Exit the program

### Command-Line Mode

Access documentation directly from the command line:

```bash
# List all sections
./ucvm-doc list

# Read a specific section
./ucvm-doc 5

# Search for a term
./ucvm-doc search "syscall"
```

## Documentation Sections

The embedded documentation includes:

1. **Introduction** - Overview of UCVM capabilities
2. **Mathematical Foundation** - Formal specification
3. **State Space Definition** - System architecture
4. **System Calls** - Complete syscall reference
5. **Instruction Set** - Assembly instructions (FULL mode)
6. **Output Modes** - RAW, VERBOSE, and DEBUG modes
7. **Mode Switching** - Transition between SIMPLIFIED and FULL
8. **Quick Reference** - Common commands and debugging
9. **Memory Layout** - Address space organization
10. **Examples** - Practical usage scenarios

## Examples

### Interactive Session

```
ucvm-doc> list
Available Sections:
───────────────────
 1. Introduction
 2. Mathematical Foundation
 3. State Space Definition
 ...

ucvm-doc> read 4
System Calls
════════════════════════════════════════════════════════════════

### Process Management
| Number | Name | Signature | Description |
|--------|------|-----------|-------------|
| 0 | fork | () → pid_t | Create child process |
...

ucvm-doc> search memory
Search Results for 'memory':
═══════════════════════════════
[Section 3: State Space Definition]
  memory: MemoryMap,              // Virtual memory mapping

[Section 9: Memory Layout]
  Address Space Layout:
```

### Command-Line Usage

```bash
# Quick lookup of system calls
./ucvm-doc 4 | grep "open"

# Search for instruction set details
./ucvm-doc search "MOV" | head -20

# Pipe to less for pagination
./ucvm-doc 3 | less
```

## Integration with UCVM

The documentation reader is designed to work seamlessly within the UCVM environment:

```bash
# In UCVM, create an alias for easy access
echo 'alias docs="/bin/ucvm-doc"' >> ~/.bashrc

# Now use simply:
docs list
docs search "process"
```

## Output Formatting

The program uses ANSI escape codes for enhanced readability:

- **Bold text** for headers and important information
- **Cyan** for section markers and prompts
- **Yellow** for search result highlighting
- **Green** for code blocks and commands

To disable colors (for piping or redirection):

```bash
./ucvm-doc list 2>&1 | sed 's/\x1b\[[0-9;]*m//g'
```

## Technical Details

- **Language**: Pure C (C99 compatible)
- **Memory**: Dynamic allocation for documentation parsing
- **Size**: ~15KB executable + embedded documentation
- **Platform**: Unix-like systems (UCVM environment)

## Troubleshooting

### Compilation Errors

If you encounter compilation issues:

```bash
# Use C99 standard explicitly
gcc -std=c99 -o ucvm-doc ucvm-doc.c

# For older compilers
gcc -D_GNU_SOURCE -o ucvm-doc ucvm-doc.c
```

### Display Issues

If colors don't display correctly:

```bash
# Check terminal support
echo $TERM

# Force color output
export TERM=xterm-256color
```

### Search Not Finding Results

The search is case-insensitive but requires exact substring matches. Try:
- Shorter search terms
- Common keywords from the documentation
- Section titles for broader results

## Contributing

To update the embedded documentation:

1. Edit the `embedded_doc` string in `ucvm-doc.c`
2. Maintain the markdown formatting
3. Recompile the program
4. Test all sections display correctly

## License

This program is part of the UCVM project and follows the same licensing terms.

## Version History

- **v1.0** - Initial release with full UCVM 2.1 specification
  - Complete documentation browser
  - Search functionality
  - Interactive and command-line modes

## See Also

- UCVM Specification v2.1
- UCVM Quick Reference Card
- Unix Manual Pages (`man` command)

---

*Built for the Unified Claude-Mediated Virtual Machine environment*
