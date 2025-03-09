// • Implement redirections:
// ◦ < should redirect input.
// ◦ > should redirect output.
// ◦ << should be given a delimiter, then read the input until a line containing the
// delimiter is seen. However, it doesn’t have to update the history!
// ◦ >> should redirect output in append mode


/*what it does:
<	Redirect stdin from file
>	Redirect stdout to file (truncate)
>>	Redirect stdout to file (append)
<<	Here-document (stdin from inline input until a delimiter line is found)*/

// execve(): Used when you want to replace the current process (the shell) 
//with an external program (e.g., cat, ls, etc.). 
//This is the typical way to run external programs in a shell.

//why to use fork?
/*fork() is used to create a new child process that is a copy of the current process 
(the parent process). After fork(), you will have two processes running: the parent process 
(which is the shell) and the child process (which will execute the command).*/