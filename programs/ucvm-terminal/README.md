# UCVM Terminal Emulator

A lightweight ANSI/VT100 terminal emulator for the Unified Claude-Mediated Virtual Machine (UCVM) environment. This program enables proper display of applications that use ANSI escape sequences for colors, cursor control, and text formatting in UCVM's RAW output mode.

## Overview

The UCVM Terminal Emulator acts as an intermediary layer between programs that output ANSI escape sequences and UCVM's standard output. It intercepts, interprets, and renders these sequences into properly formatted text output.

## Features

### Core Functionality
- 🖥️ **80x24 Character Display**: Standard terminal dimensions
- 🎨 **ANSI Color Support**: Processes foreground/background color codes
- 📍 **Cursor Control**: Full cursor positioning and movement
- 📜 **Scrolling**: Automatic screen scrolling when content exceeds display
- ✨ **Text Attributes**: Bold, underline, and reverse video support
- 🔄 **Live Processing**: Real-time interpretation of escape sequences

### Supported ANSI Sequences
- **Cursor Movement**: Up, Down, Forward, Backward
- **Cursor Positioning**: Direct positioning and save/restore
- **Display Control**: Clear screen, clear line
- **Graphics Rendition**: Colors, bold, underline, reverse
- **Common Sequences**: CSI, SGR, and standard VT100 codes

## Installation

```bash
# Compile the terminal emulator
gcc -o term ucvm-terminal.c

# Make executable (if needed)
chmod +x term
```

### Compilation Options

```bash
# With optimization
gcc -O2 -o term ucvm-terminal.c

# With debugging symbols
gcc -g -o term ucvm-terminal.c

# For C99 compliance
gcc -std=c99 -o term ucvm-terminal.c
```

## Usage

### Command Mode

Run a single program with terminal emulation:

```bash
./term <program> [arguments...]
```

Examples:
```bash
# Run the UCVM documentation reader
./term ./ucvm-doc

# Run ls with color output
./term ls --color=always

# Run any program that uses ANSI codes
./term ./color-demo
```

### Interactive Mode

Launch an interactive terminal session:

```bash
./term
```

In interactive mode:
```
UCVM Terminal Emulator - Interactive Mode
Type 'exit' to quit

term> ./ucvm-doc list
[Terminal-emulated output appears here]

term> ./another-program
[More emulated output]

term> exit
```

## How It Works

### Architecture

```
┌─────────────────┐     pipe      ┌──────────────────┐
│  Child Process  │ ──────────>   │ Terminal Emulator│
│  (with ANSI)    │               │   - Parse ANSI   │
└─────────────────┘               │   - Update screen│
                                  │   - Render output│
                                  └──────────────────┘
                                           │
                                           v
                                  ┌──────────────────┐
                                  │   Clean Output   │
                                  │  (to UCVM/user)  │
                                  └──────────────────┘
```

### Processing Pipeline

1. **Child Process Creation**: Forks and executes the target program
2. **Output Capture**: Redirects stdout/stderr through a pipe
3. **ANSI Parsing**: Identifies and interprets escape sequences
4. **Screen Buffer Update**: Maintains internal 80x24 character buffer
5. **Rendering**: Outputs clean, formatted text

## Supported ANSI/VT100 Codes

### Cursor Control
| Sequence | Description |
|----------|-------------|
| `ESC[{n}A` | Cursor up n lines |
| `ESC[{n}B` | Cursor down n lines |
| `ESC[{n}C` | Cursor forward n columns |
| `ESC[{n}D` | Cursor backward n columns |
| `ESC[{r};{c}H` | Set cursor position |
| `ESC[s` | Save cursor position |
| `ESC[u` | Restore cursor position |

### Display Control
| Sequence | Description |
|----------|-------------|
| `ESC[2J` | Clear entire screen |
| `ESC[K` | Clear to end of line |
| `ESC[0K` | Clear from cursor to end of line |

### Graphics Rendition
| Sequence | Description |
|----------|-------------|
| `ESC[0m` | Reset all attributes |
| `ESC[1m` | Bold/bright |
| `ESC[4m` | Underline |
| `ESC[7m` | Reverse video |
| `ESC[30-37m` | Foreground colors |
| `ESC[40-47m` | Background colors |

## Examples

### Running Programs with Color Output

```bash
# Documentation reader with formatting
./term ./ucvm-doc

# Syntax-highlighted code viewer
./term ./syntax-cat program.c

# Color-enabled directory listing
./term ls -la --color=always
```

### Testing Terminal Emulation

Create a test script:
```bash
cat > test-ansi.sh << 'EOF'
#!/bin/sh
echo -e "\033[2J\033[H"           # Clear screen
echo -e "\033[1;36mBold Cyan\033[0m"
echo -e "\033[32mGreen text\033[0m"
echo -e "\033[1;4;33mBold Underline Yellow\033[0m"
EOF

chmod +x test-ansi.sh
./term ./test-ansi.sh
```

### Integration with UCVM

```bash
# Create an alias for convenience
echo 'alias vt100="./term"' >> ~/.bashrc

# Now use with any program
vt100 ./ucvm-doc
vt100 top
vt100 vim file.txt
```

## Limitations

### Current Implementation
- Fixed 80x24 screen size (no dynamic resizing)
- Simplified color rendering (attributes noted but not fully rendered)
- No mouse support
- No alternate screen buffer
- Basic Unicode/UTF-8 (ASCII only)

### Not Supported
- Terminal resizing sequences
- Advanced xterm extensions
- True color (24-bit) support
- Character set switching
- Application keypad mode

## Troubleshooting

### Common Issues

**Problem**: Output appears garbled
```bash
# Solution: Check if program outputs valid ANSI sequences
./program | od -c  # Examine raw output
```

**Problem**: Colors not displaying
```bash
# The emulator parses colors but renders simplified output
# Future versions may support full color rendering
```

**Problem**: Interactive programs don't work well
```bash
# Currently optimized for programs with streaming output
# Full interactive support (like vim) requires TTY emulation
```

### Debug Mode

To debug ANSI sequence processing, modify the source:
```c
// Add debug output in process_escape()
fprintf(stderr, "ESC sequence: %.*s\n", seq_len, input);
```

## Technical Details

### Memory Usage
- Static 80x24 screen buffer: ~2KB
- Attribute buffer: ~2KB
- I/O buffer: 4KB
- Total overhead: ~8KB per instance

### Performance
- Non-blocking I/O for responsiveness
- Minimal processing overhead
- Suitable for real-time output

### Compatibility
- POSIX compliant
- Requires: unistd.h, sys/wait.h, fcntl.h
- Tested on Unix-like systems

## Future Enhancements

Potential improvements for future versions:

1. **Dynamic screen sizing** - Support for variable terminal dimensions
2. **Full color rendering** - Actual color output in UCVM
3. **TTY emulation** - Support for interactive programs
4. **Unicode support** - UTF-8 character handling
5. **Mouse sequences** - Terminal mouse event processing
6. **Performance optimization** - Faster sequence parsing
7. **Configuration file** - User-customizable behavior

## Contributing

To enhance the terminal emulator:

1. **Add new sequences**: Update `process_csi()` function
2. **Improve rendering**: Enhance `render_screen()` function
3. **Add features**: Extend `TerminalState` structure
4. **Test thoroughly**: Create test cases for new sequences

## Related Projects

- UCVM Documentation Reader (`ucvm-doc`)
- UCVM Specification
- VT100 Terminal Reference
- ANSI Escape Code Standards

## License

Part of the UCVM project. Same licensing terms apply.

## Version History

- **v1.0** (Current)
  - Initial release
  - Basic ANSI/VT100 support
  - Command and interactive modes
  - 80x24 screen emulation

---

*Enabling ANSI terminal capabilities in the UCVM environment*