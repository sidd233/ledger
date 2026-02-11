# Ledger — Project Context

## Purpose
Ledger is a terminal-based habit tracker with lightweight RPG-style progression.
It is a **side project** meant to rebuild the habit of coding through small, calm, low-friction steps.

This is **not** a product and does not need to scale.

## Tone
- Terminal-first
- Calm, neutral, product-like
- RPG mechanics are internal, not performative
- No hype, no productivity language

## Tech
- Language: C++
- Compiler: g++ 13.3
- Standard: C++20
- OS: Linux
- Single-file start (`main.cpp` in repo root)

## Mental Model
- Terminal runs executables, not source files
- `ledger` is a binary placed on `$PATH`
- Commands are short-lived programs (run → print → exit)

## Current State
- `ledger` binary compiles and runs
- Command routing works (`init`, `status`)
- `init` creates state file at `~/.config/ledger/`
- Determining state file format and contents

## Constraints
- No web, no UI, no daemon
- One local state file (format TBD)
- No external libraries unless pain appears
- Completion > elegance

## Commands (planned)

```
ledger init
ledger log "coding 25m"
ledger status
ledger items
```

## Next Goal
Implement `ledger init`:
- create local state file
- do not overwrite if it exists
- confirm initialization