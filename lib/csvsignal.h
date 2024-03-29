#ifndef CSV_SIGNAL_H
#define CSV_SIGNAL_H

#include <signal.h>
#include <errno.h>
#include "util/node.h"

/**
 * Initialize signal handlers
 */
void init_sig();

/**
 * This function simply removes any temporary files
 * in the event of an error or SIGxxx detected.
 */
void cleanexit();

/**
 * Add temp file
 */
struct node* tmp_push(void* tmp_file);

/**
 * Remove temp node only
 */
void tmp_remove_node(struct node* node);

/**
 * Remove temp file
 */
void tmp_remove_file(const char* tmp_file);

/**
 * Remove all temp files
 */
void tmp_removeall();

#endif /* CSV_SIGNAL_H */
