#ifndef INTERNAL_H
#define INTERNAL_H

#include <stdio.h>
#include <stdlib.h>
#include "util/node.h"
#include "util/vec.h"
#include "util/stringy.h"

/**
 * Internal Structures
 */

/* [csv_record].fields always points to the data
 * owned by [csv_record]._in->_fields. This way
 * we do not expose our internal vector type.
 */
struct csv_record_internal {
	vec* field_data; /* vec<string> */
	vec* _fields;    /* vec<struct csv_field> */
	size_t rec_alloc;
	unsigned field_alloc;
};

struct csv_read_internal {
	FILE* file;
	string delim;
	string weak_delim;
	string embedded_break;
	char* mmap_ptr;
	size_t offset;
	size_t file_size;
	int fd;

	/* Statistics */
	unsigned rows;
	unsigned embedded_breaks;

	/* Properties */
	int normorg;
	_Bool is_mmap;
};

struct csv_write_internal {
	FILE* file;
	node* tmp_node;
	string tempname;
	string filename;
	string buffer;
	string delim;
	string rec_terminator;
	int reclen;
	_Bool is_detached;
};

#endif
