#include "../include/t_programcontrol.h"

//================================================================================
//    progress bar
//================================================================================

void
var_printProgress (printProgress_args in)
{
    // if arguments are not already defined, they are set to default values
    double p_out = in.p ? in.p : 0;
    char pbsgn_out = in.pbsgn ? in.pbsgn : PBSGN;
    int pbwidth_out = in.pbwidth ? in.pbwidth : PBWIDTH;
    base_printProgress (p_out, pbsgn_out, pbwidth_out);
}

void
base_printProgress (double p, char pbsgn, int pbwidth)
{
    char* bar = (char *) malloc((pbwidth + 1) * sizeof(char));
    memset(bar, pbsgn, pbwidth);

    int val = (int) (p * 100);

    int lpad = (int) (p * pbwidth);

    /* calculates space from current progress bar to its end,
     * so that string always has correct length */
    int rpad = pbwidth - lpad;

    if (val == 100) {
        printf("\r  %3d %% [%.*s%*s]\n", val, lpad, bar, rpad, "");
    }
    /* %.*s is format specifier that allows to only print certain length
     * (1st argument) of given string (2nd argument),
     * whereas %*s prints one string a certain number repeatedly */
    else {
        printf("\r  %3d %% [%.*s%*s]", val, lpad, bar, rpad, "");
    }
    // flush stdout so that the progress bar is output directly without buffering
    fflush(stdout);
    free(bar);
}

//================================================================================
//    pointers
//================================================================================

/* if from malloc returned pointer is NULL RAM might be full or we do not have
 * rights to request memory/storage, therefore we want feedback so we can decide 
 * how to proceed */

bool
is_NULL (void* pointer)
{
    if (pointer == NULL)
        return true;
    return false;
}

void
exit_if_NULL (void* pointer)
{
    if (pointer == NULL) {
        printf("> Requested pointer is NULL! Programm is terminated.\n");
        exit(1);
    }
}

void
Null_exit_message (void* pointer, char message[])
{
    if (pointer == NULL) {
        printf("%s\n", message);
        exit(1);
    }
}

bool
Null_message (void* pointer, char message[])
{
    if (pointer == NULL) {
        printf("%s\n", message);
        return true;
    }
    return false;
}

//================================================================================
//    color text output from printf()
//================================================================================

unsigned int
tp_decideColor (char color)
{
    switch (color) {
        // dark (black. But b already belongs to blue)
        case 'd':
            return 30;
        // red
        case 'r':
            return 31;
        // green
        case 'g':
            return 32;
        // yellow
        case 'y':
            return 33;
        // blue
        case 'b':
            return 34;
        // purple
        case 'p':
            return 35;
        // cyan
        case 'c':
            return 36;
        // white
        case 'w':
            return 37;
        
        default:
            tp_raiseWarning ("Color not recognized! 0 is returned.\n");
            return 0;
    }
}

void
tp_regularColor (char color)
{
    printf("\033[0;%um", tp_decideColor (color));
}

void
tp_boldColor (char color)
{
    printf("\033[1;%um", tp_decideColor (color));
}

void
tp_UnderlineColor (char color)
{
    printf("\033[4;%um", tp_decideColor (color));
}

void
tp_BackgroundColor (char color)
{
    printf("\033[%um", tp_decideColor (color) + 10);
}

void
tp_highIntensityColor (char color)
{
    printf("\033[0;%um", tp_decideColor (color) + 60);
}

void
tp_highIntensityBoldColor (char color)
{
    printf("\033[1;%um", tp_decideColor (color) + 60);
}

void
tp_highIntensityBackgroundColor (char color)
{
    printf("\033[0;%um", tp_decideColor (color) + 70);
}

void
tp_reset (void)
{
    printf("\033[0m");
}

void
tp_raiseError(char message[])
{
    printf("> ");
    tp_boldColor ('r');
    printf("Error: ");
    tp_reset ();
    printf("%s\n", message);
    /* using exit function, the kernel automatically frees memory of programm.
     * So we do not need to worry about memory leaks */
    exit(1);
}

void
tp_raiseWarning (char message[])
{
    printf("> ");
    tp_boldColor ('p');
    printf("Warning: ");
    tp_reset ();
    printf("%s", message);
}
