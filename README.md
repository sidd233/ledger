# ledger

Ledger is a terminal-based habit tracker with lightweight RPG-style progression.
It records actions locally, converts effort into structured progress, and presents your status through a simple command-line interface.

The goal is consistency, not optimization.

## Features

- Terminal-first CLI
- Local, offline data storage
- Progress, ranks, and attributes
- Simple item-based modifiers
- No accounts, no cloud, no UI

## Usage

```bash
ledger init
ledger log "coding 25m"
ledger status
ledger items
```

## Design Principles

- Minimal surface area
- Fast feedback
- Calm, neutral output
- Completion over complexity

## Installation

Build from source:

```bash
g++ -std=c++20 main.cpp -o ledger && ./ledger
```

## Data

All data is stored locally in a single file created during initialization.
No network access is used.

## Status

This is a small, self-contained side project intended for personal use.
