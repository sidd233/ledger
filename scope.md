# Ledger — Scope Boundary

This file defines what Ledger **is** and **is not**.
If a change violates this document, it is **not allowed**.

---

## Purpose

Ledger exists to:
- make small, real actions feel complete
- restore the habit of building
- provide calm, terminal-native feedback

Ledger does **not** exist to optimize life, enforce discipline, or scale.

---

## Core Definition

Ledger is:
- a single-user
- terminal-based
- offline
- local-first
habit tracker with lightweight RPG-style progression.

Ledger is **not** a platform, service, or framework.

---

## Allowed Features (Exhaustive)

Only the following features may exist.

### Commands
- `ledger init`
- `ledger log "<text> <minutes>"`
- `ledger status`
- `ledger items`

No new commands may be added.

---

### Data
- One local state file
- Plain text (JSON or equivalent)
- Stored in a single location
- No history beyond totals

---

### Mechanics
- Rank (level)
- Progress (XP)
- One primary attribute (Focus)
- Passive item modifiers

No skills, classes, trees, or quests.

---

### Interface
- Plain text output
- No colors
- No animations
- No interactive prompts
- Program runs and exits

---

## Explicitly Forbidden

The following are **never allowed**:

- Web interface
- Background processes
- Accounts or profiles
- Syncing or cloud storage
- Configuration files
- Plugins or extensions
- Multiple users
- Notifications or reminders
- Charts or visualizations
- Gamification language in output

---

## Technical Limits

- Single binary
- No external dependencies unless required by the OS
- No build system beyond direct compilation
- No refactors that do not unlock a listed feature

---

## Stop Conditions

Ledger is considered **complete** when:
- all allowed commands work
- data persists correctly
- output is readable and stable

After completion:
- only bug fixes are permitted
- no feature work may continue

---

## Exit Clause

This project may only be expanded if:
1. This file is consciously deleted or rewritten
2. The repository is considered archived
3. A new project is started

Until then:
**Do not cross this boundary.**
