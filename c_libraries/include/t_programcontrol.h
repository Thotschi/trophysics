/* t_programcontrol.h
 * 
 * -------------------------------------------------------------------------------
 * Purpose:
 * This header mainly outsources functions and macros that are needed for the 
 * numerics library t_numerics but do not contain numerical methods into an own
 * header which is then dedicated for program control and output.
 * 
 * Usage:
 * Use the functions to color text output, print progress bars, and handle
 * pointers. Most things are self explanatory and, if not, have a brief
 * explanation.
 * 
 * Contributions and Feedback:
 * If you encounter bugs, have suggestions, or want to contribute, please reach out.
 * Contact: k.reinisch@stud.uni-goettingen.de
 * 
 * Date: 2025-08-14
 * Version: 1.0
 * 
 * License: CC BY-NC-SA 4.0
 * 
 */

#ifndef T_PROGRAMMCONTROL_H
#define T_PROGRAMMCONTROL_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//################################################################################

/*-----default settings for progressbar-----*/
#ifndef PBWIDTH
#define PBWIDTH 100
#endif

#ifndef PBSGN
#define PBSGN '#'
#endif

#ifndef EMPTYSTR
#define EMPTYSTR ""
#endif

//################################################################################
// progressbar in terminal

//--------------------------------------------------------------------------------
// usage:
//  printProgress(double p, char pbsgn, int pbwidth) takes arguments:
//  current progress (from 0 to 1), sign for progress bar (optional, dflt = '#'), 
//  width of progress bar (optional, dflt = 100)
//  possible calls: printProgress(0.3);          --> p=0.3, pbsgn='#', pbwidth=100
//                  printProgress(0.3, '=', 70); --> p=0.3, pbsgn='=', pbwidth=70
//                  printProgress(0.3, '|');     --> p=0.3, pbsgn='|', pbwidth=100
//                  f(0.3, .pbwidth = 70);       --> p=0.3, pbsgn='#', pbwidth=70
//                  f(.p = 0.3, .pbsgn = '|');   --> p=0.3, pbsgn='|', pbwidth=100
//  etc.
//--------------------------------------------------------------------------------

typedef struct {
  double p;
  char pbsgn;
  int pbwidth;
} printProgress_args;

#define printProgress(...) var_printProgress((printProgress_args){__VA_ARGS__})

void var_printProgress (printProgress_args in);
void base_printProgress (double p, char pbsgn, int pbwidth);

//################################################################################
// pointers

/* If from malloc returned pointer is NULL RAM might be full or we do not have
 * rights to request memory/storage, therefore we want feedback to decide how to 
 * proceed. */

bool is_NULL (void* pointer);
void exit_if_NULL (void* pointer);
void Null_exit_message (void* pointer, char message[]);
bool Null_message (void* pointer, char message[]);

//################################################################################
// color text output from printf()

unsigned int tp_decideColor (char color);

/* for functions below following arguments determine color:
 * 'd'=black,'r'=red, 'g'=green, 'y'=yellow, 'b'=blue, 'p'=purple, 'c'=cyan,
 * 'w'=white */
void tp_regularColor (char color);
void tp_boldColor (char color);
void tp_UnderlineColor (char color);
void tp_BackgroundColor (char color);
void tp_highIntensityColor (char color);
void tp_highIntensityBoldColor (char color);
void tp_highIntensityBackgroundColor (char color);

/*--resets text color options to default--*/
void tp_reset (void);

void tp_raiseError (char message[]);
void tp_raiseWarning (char message[]);

#endif
