# ledger

Ledger is a terminal-based habit tracker with lightweight RPG-style progression.
It records actions locally, converts effort into structured progress, and presents your status through a simple command-line interface.

The goal is consistency, not optimization.

## Features

- Terminal-first CLI
- Local, offline data storage
- Rank, XP, and Focus tracking

## Usage

```bash
ledger init
ledger log "coding 25m"
ledger status
ledger reset
```

## Design Principles

- Minimal surface area
- Fast feedback
- Calm, neutral output
- Completion over complexity

## Installation

Build from source:

```bash
g++ -std=c++20 main.cpp -o ledger
```

Place the binary somewhere on your `$PATH`, e.g.:

```bash
cp ledger ~/.local/bin/ledger
```

## Data

All data is stored locally in a single plain-text file at `~/.config/ledger/ledger.state`.
No network access is used.