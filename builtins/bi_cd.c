#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// cd with only a relative or absolute path

// cd myFolder - relative path
// cd ../ - relative path (up one directory)
// cd /home/user - absolute path


#include <stdio.h>
#include <unistd.h>

int bi_cd(char *argv[]) 
{
    if (argv[1] == NULL) 
    {
        //custom message - projekt ma inne wymagania niz zwykly cd, not tied to any system call
        // telling the user they made a mistake (nasz wlasny logic check)
        fprintf(stderr, "cd: missing argument (relative or absolute path required)\n");
        return (1);
    }

    if (chdir(argv[1]) != 0) 
    {
        // last error from a system call (like chdir/open itd), stored in the global 
        // variable errno - perror sobie z tego czyta
        perror("cd");
        return (1);
    }

    return 0;
}



// test
int main(int argc, char *argv[])
{
    //before changing
    char *cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
        printf("Before cd: %s\n", cwd);
        free(cwd);
    }

    int result = bi_cd(argv);

    //afer
    cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
        printf("After cd: %s\n", cwd);
        free(cwd);
    }

    return (result);
}


// ///// stare - obsluguje brak argumentu, my nie powinnismy

// //jesli samo cd - go to the home directory
// int bi_cd(char *argv[]) 
// {
//     // If no arguments, change to the home directory
//     if (argv[1] == NULL) 
//     {
//         char *home = getenv("HOME");
//         if (home == NULL) 
//         {
//             // If HOME is not set, print an error
//             perror("cd");
//             return 1;
//         }
//         //jesli nie udalo sie mienic na home
//         if (chdir(home) != 0) 
//         {
//             perror("cd");
//             return (1);
//         }
//     }
//     // If there's an argument, attempt to change to the specified directory
//     else 
//     {
//         // Change to the specified directory (can be relative or absolute)
//         if (chdir(argv[1]) != 0) 
//         {
//             perror("cd");
//             return (1);
//         }
//     }
//     return (0);
// }