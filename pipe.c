#include "ms.h"

// pipe(int pipefd[2]) - pipefd[0]: Read end, pipefd[1]: Write end
// 1. use dup2() to redirect stdin/stdout to/from a pipe
// 2. fork a process for each command in the pipeline
// 3. The first command’s output goes into the pipe
// The last command reads from the last pipe, and outputs to stdout
// Any middle command reads from previous pipe and writes to next