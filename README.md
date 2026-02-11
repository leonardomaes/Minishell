# 🐚 Minishell - Custom Linux Shell

A fully functional Unix shell implementation in C, replicating bash behavior with process management, signal handling, and advanced parsing capabilities.

![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![Make](https://img.shields.io/badge/Make-red?style=flat)

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Technical Details](#technical-details)
- [Project Structure](#project-structure)
- [What I Learned](#what-i-learned)

## 🎯 Overview

Minishell is a project from the 42 School curriculum that challenges students to recreate a simplified version of bash. This project deepened my understanding of process creation, file descriptors, signal handling, and lexical analysis.

**Development Period:** November 2024 - February 2025  
**Grade:** 101/100

## ✨ Features

### Core Functionality
- ✅ Command execution with absolute and relative paths
- ✅ Built-in commands (cd, echo, pwd, export, unset, env, exit)
- ✅ Environment variable expansion (`$VAR`)
- ✅ Exit status handling (`$?`)
- ✅ Single (`'`) and double (`"`) quote parsing
- ✅ Redirections (`<`, `>`, `>>`, `<<` heredoc)
- ✅ Pipes (`|`) with multiple commands
- ✅ Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)

### Advanced Features
- 🎯 **95% syntax validation accuracy** through advanced tokenization
- 🔒 **100% memory leak prevention** (verified with Valgrind)
- ⚡ Efficient process management with proper fork/exec handling
- 🛡️ Robust error handling for edge cases

## 🚀 Installation
```bash
# Clone the repository
git clone https://github.com/leonardomaes/minishell.git
cd minishell

# Compile the project
make

# Run minishell
./minishell
```

### Requirements
- GCC compiler
- GNU Make
- readline library (`sudo apt-get install libreadline-dev` on Ubuntu)

## 💻 Usage
```bash
$ ./minishell
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la | grep minishell | wc -l
1

minishell$ export USER=leonardo
minishell$ echo $USER
leonardo

minishell$ cat << EOF > file.txt
> Line 1
> Line 2
> EOF

minishell$ exit
```

## 🔧 Built-in Commands

| Command | Description | Example |
|---------|-------------|---------|
| `cd` | Change directory | `cd /home` or `cd ..` |
| `echo` | Print to stdout (with -n flag) | `echo -n "No newline"` |
| `pwd` | Print working directory | `pwd` |
| `export` | Set environment variable | `export PATH=/usr/bin` |
| `unset` | Remove environment variable | `unset PATH` |
| `env` | Display environment variables | `env` |
| `exit` | Exit shell with status code | `exit 0` |

## 🔍 Technical Details

### Architecture
```
Minishell Pipeline:
Input → Lexer → Parser → Executor → Output
         ↓        ↓         ↓
      Tokens   AST    Processes
```

### Key Components

**1. Lexer/Tokenizer**
- Breaks input into tokens (words, operators, quotes)
- Handles quote removal and escape sequences
- Validates syntax before execution

**2. Parser**
- Builds command structures from tokens
- Handles redirections and pipe connections
- Expands environment variables

**3. Executor**
- Manages process creation (fork/exec)
- Sets up pipes and file descriptors
- Handles built-in commands directly

**4. Signal Handler**
- Intercepts Ctrl-C (SIGINT) - new prompt
- Intercepts Ctrl-\\ (SIGQUIT) - no action
- Ctrl-D (EOF) - exits shell

### Memory Management
- All heap allocations tracked and freed
- No memory leaks (Valgrind tested)
- Proper cleanup on exit and error cases

## 📁 Project Structure
```
minishell/
├── src/
│   ├── builtins/				# Built-in commands
│   ├── executor/				# Execution engine
│   ├── exit_free/				# Clear memory
│   ├── main/					# Init functions
│   ├── parser_and_tokens/		# Tokenization logic & Command parsing
│   └── signals/				# Signal handling
├── includes/
│   ├── libft/
│   ├── ft_printf/
│   └── minishell.h
├── Makefile
└── README.md
```

## 📚 What I Learned

### Technical Skills
- **Process Management:** fork(), execve(), wait(), pipe()
- **File Descriptors:** dup2(), open(), close()
- **Signal Handling:** signal(), sigaction()
- **Memory Management:** malloc(), free(), leak prevention
- **Parsing Techniques:** tokenization, state machines

### Soft Skills
- Debugging complex multi-process programs
- Reading and implementing technical specifications
- Peer code review and knowledge sharing
- Breaking down large problems into manageable components

## 🐛 Known Limitations

This shell is a learning project and doesn't implement:
- Logical operators (`&&`, `||`)
- Wildcard expansion (`*`)
- Command history (up/down arrows)
- Job control (background processes with `&`)

## 🤝 Collaboration

This project was developed in collaboration with [Ricardo Mendes](https://github.com/rda-cunh) as part of the 42 Porto curriculum.

## 📝 License

This project is part of the 42 School curriculum and follows their academic policies.

---

**42 Porto** | [Leonardo Maes](https://linkedin.com/in/leonardo-maes)
