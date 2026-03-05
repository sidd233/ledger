# 1. Init Command

## 1.1 Detect Config Directory

- [x] Implement `get_config_home()` fallback logic
- [x] Verify `XDG_CONFIG_HOME`
- [x] Fallback to `$HOME/.config`
- [x] Handle missing HOME edge case

## 1.2 Create Ledger Directory

- [x] Build path `<config_home>/ledger`
- [x] Create directory using `std::filesystem::create_directories`

## 1.3 Detect Existing State

- [x] Construct state path `<config_home>/ledger/ledger.state`
- [x] Check if file exists using `std::filesystem::exists`

## 1.4 Prevent Overwrite

- [x] If state file exists:
  - [x] Print "Ledger already initialized"
  - [x] Exit without modifying file

## 1.5 Create Initial State File

- [x] Open file using `std::ofstream`
- [x] Write default values

```
rank=1
xp=0
focus=0
items=0
```

- [x] Close file

## 1.6 Confirm Initialization

- [ ] Print confirmation message

```
Ledger initialized.
```

---

# 2. State File Handling

## 2.1 Define State Variables

- [ ] `int rank`
- [ ] `int xp`
- [ ] `int focus`
- [ ] `int items`

## 2.2 Implement State Loader

- [ ] Open `ledger.state`
- [ ] Read file line-by-line
- [ ] Split line into `key=value`
- [ ] Convert value to integer
- [ ] Assign to corresponding variable

## 2.3 Implement State Writer

- [ ] Open state file for writing
- [ ] Write all fields in consistent order
- [ ] Ensure newline separation
- [ ] Close file

---

# 3. Status Command

## 3.1 Detect Initialization

- [ ] Check if state file exists
- [ ] If not:

  - [ ] Print "Ledger not initialized"
  - [ ] Suggest running `ledger init`
  - [ ] Exit

## 3.2 Load State

- [ ] Call state loader
- [ ] Populate rank/xp/focus/items variables

## 3.3 Print Status

- [ ] Print rank
- [ ] Print xp
- [ ] Print focus
- [ ] Print items

Example output:

```
Rank: 1
XP: 0
Focus: 0
Items: 0
```

---

# 4. Log Command

## 4.1 Command Argument Handling

- [ ] Ensure `argc >= 3`
- [ ] Extract log string from `argv[2]`

## 4.2 Parse Log Input

Input format:

```
ledger log "coding 25m"
```

Tasks:

- [ ] Separate activity text
- [ ] Extract minutes value
- [ ] Remove `m` suffix
- [ ] Convert minutes to integer
- [ ] Validate minutes > 0

## 4.3 Verify Initialization

- [ ] Ensure state file exists
- [ ] If not:

  - [ ] Print initialization message
  - [ ] Exit

## 4.4 Load Current State

- [ ] Load rank
- [ ] Load xp
- [ ] Load focus
- [ ] Load items

## 4.5 Update XP

- [ ] Define rule: `xp += minutes`

## 4.6 Update Rank

- [ ] Define rank formula

Example:

```
rank = 1 + xp / 100
```

- [ ] Recalculate rank

## 4.7 Update Focus

- [ ] Increase focus by minutes

```
focus += minutes
```

## 4.8 Save Updated State

- [ ] Write updated values to file

## 4.9 Confirm Log

Example output:

```
Logged: coding (25m)
```

---

# 5. Items Command

## 5.1 Verify Initialization

- [ ] Check state file exists
- [ ] Exit if missing

## 5.2 Load State

- [ ] Load `items` variable

## 5.3 Print Items

Example output:

```
Items: 0
```

---

# 6. Command Router

## 6.1 Handle Missing Command

- [ ] Detect `argc < 2`
- [ ] Print usage message

## 6.2 Add Command Handlers

- [ ] `init`
- [ ] `status`
- [ ] `log`
- [ ] `items`

## 6.3 Unknown Command Handling

- [ ] Print

```
ledger: unknown command
```

---

# 7. Error Handling

## 7.1 Config Directory Errors

- [ ] Detect empty config path
- [ ] Print error message

## 7.2 File Creation Errors

- [ ] Check `ofstream` success
- [ ] Print failure message if needed

## 7.3 Invalid Log Format

Examples:

```
ledger log "coding"
ledger log "coding xm"
ledger log ""
```

Tasks:

- [ ] Validate format
- [ ] Print usage message

---

# 8. Compilation

## 8.1 Compile Binary

```
g++ -std=c++20 main.cpp -o ledger
```

## 8.2 Verify Binary Runs

- [ ] Run `./ledger`
- [ ] Run `./ledger init`
- [ ] Run `./ledger status`

---

# 9. Install Binary on PATH

## 9.1 Move Binary

```
mv ledger ~/.local/bin/
```

## 9.2 Verify PATH

```
echo $PATH
```

## 9.3 Test Global Command

- [ ] Run `ledger init`
- [ ] Run `ledger status`

---

# 10. Persistence Testing

## 10.1 Initialization Test

- [ ] Run `ledger init`
- [ ] Confirm state file exists

## 10.2 Logging Test

- [ ] Run `ledger log "coding 25m"`
- [ ] Confirm xp increases

## 10.3 Restart Test

- [ ] Exit terminal
- [ ] Run `ledger status`
