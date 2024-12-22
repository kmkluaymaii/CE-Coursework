## Project 1 - Shell


The task is to create a simple shell that can read input, parse commands, and execute them. Below is a detailed overview of the implementation in myshell.c.

1. Main Loop
In myshell.c, I started by building a loop that performs the following steps:
    1. Prompt Display: Check if -n was passed as an argument. If not, print the prompt myshell$ .
    2. Input Reading: Read the user input from the terminal that follows the prompt.
    3. Exit Condition: Check if the Ctrl+D (EOF) is pressed. If it is, exit the loop and terminate the shell.
    4. Newline Removal: Remove the newline character from the input string.
    5. Command Parsing: Pass the cleaned input into the parse_command function for processing.

2. Command Parsing
Inside the parse_command function, I parse the input into separate commands by:
    1. Tokenization: Use the strtok function to split the input based on the pipe character |.
    2. Command Storage: Store the split commands into the command array.
    3. Command Processing Loop:
    Iterate through the commands and use the delete_space function to trim leading and trailing spaces from each command.

3. delete_space Function
The delete_space function works as follows:
    1. Leading Space Check: Use a while loop to check for leading spaces and increment the pointer accordingly.
    2. Single Space Handling: If there is only one space, return the command as is.
    3. Trailing Space Check: Identify the end of the command and decrement the pointer if there are trailing spaces.
    4. Null-Termination: Place a null character at the end of the command to terminate the string.
    5. Return Cleaned Command: Return the command without leading and trailing spaces.
    6. Command Output: After processing, print each command and flush the output.
    7. Command Execution: Pass the command to the execute_command function.

4. Command Execution
Inside the execute_command function, I handle the execution of the first command as follows:
    1. Argument Tokenization: Use strtok to split the first command into its constituent arguments.
    2. Argument Storage: Store these arguments into the arg array.
    3. Process Creation: Fork a new child process using fork().
    4. Child Process Execution: In the child process, use execvp to execute the command.
    5. Parent Process Waiting: In the parent process, wait for the child to finish using waitpid.
    6. Result Display: After the command execution, print the result on the last line.