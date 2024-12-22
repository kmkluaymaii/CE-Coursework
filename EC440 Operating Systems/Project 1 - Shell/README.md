## Project 1 - Shell


The task is to create a simple shell that can read input, parse commands, and execute them. Below is a detailed overview of the implementation in myshell.c.

### Implementation Details

### 1. **Main Loop**
The main loop in `myshell.c` performs these steps:
1. **Prompt Display**:
   - If `-n` is not passed as an argument, print the prompt `myshell$`.
2. **Input Reading**:
   - Read the user input from the terminal after displaying the prompt.
3. **Exit Condition**:
   - Exit the loop and terminate the shell if `Ctrl+D` (EOF) is pressed.
4. **Newline Removal**:
   - Remove the newline character from the input string.
5. **Command Parsing**:
   - Pass the cleaned input to the `parse_command` function for processing.

---

### 2. **Command Parsing**
The `parse_command` function splits input into individual commands:
1. **Tokenization**:
   - Use `strtok` to split the input string by the pipe character `|`.
2. **Command Storage**:
   - Store the split commands into the `command` array.
3. **Command Processing**:
   - Loop through commands and trim spaces using the `delete_space` function.

---

### 3. **delete_space Function**
The `delete_space` function removes unnecessary spaces:
1. **Leading Space Removal**:
   - Increment the pointer to skip leading spaces.
2. **Single Space Handling**:
   - Return the command if it has only one space.
3. **Trailing Space Removal**:
   - Decrement the pointer to remove trailing spaces.
4. **Null-Termination**:
   - Add a null character to mark the end of the string.
5. **Return Clean Command**:
   - Return the cleaned command for further processing.

---

### 4. **Command Execution**
The `execute_command` function executes the first command:
1. **Argument Tokenization**:
   - Use `strtok` to split the command into arguments.
2. **Argument Storage**:
   - Store the arguments in the `arg` array.
3. **Process Creation**:
   - Use `fork()` to create a child process.
4. **Child Process Execution**:
   - In the child process, use `execvp` to execute the command.
5. **Parent Process Waiting**:
   - Wait for the child process to finish using `waitpid`.
6. **Result Display**:
   - Print the result after execution.

---
