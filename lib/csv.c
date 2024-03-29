#include "csverror.h"
#include "misc.h"
#include "csv.h"
#include "internal.h"
#include "util/stringy.h"
#include "util/util.h"

void csv_perror()
{
	err_printall();
}

void csv_perror_exit()
{
	csv_perror();
	exit(EXIT_FAILURE);
}

struct csv_record* csv_record_new()
{
	struct csv_record* new_rec = NULL;
	new_rec = malloc_(sizeof(*new_rec));
	return csv_record_construct(new_rec);
}

struct csv_record* csv_record_construct(struct csv_record* self)
{
	*self = (struct csv_record) {
	        ._in = NULL,
	        .fields = NULL,
	        .rec = NULL,
	        .reclen = 0,
	        .size = 0,
	};

	self->_in = malloc_(sizeof(*self->_in));
	*self->_in = (struct csv_record_internal) {
	        .field_data = new_t_(vec, string),
	        ._fields = new_t_(vec, struct csv_field),
	        .rec_alloc = 0,
	        .field_alloc = 0,
	};

	return self;
}

void csv_record_free(struct csv_record* self)
{
	csv_record_destroy(self);
	free_(self);
}

void csv_record_destroy(struct csv_record* self)
{
	delete_(vec, self->_in->_fields);
	string* it = vec_begin(self->_in->field_data);
	for (; it != vec_end(self->_in->field_data); ++it) {
		string_destroy(it);
	}
	delete_(vec, self->_in->field_data);
	if (self->_in->rec_alloc > 0) {
		free_(self->rec);
	}
	free_(self->_in);
}

struct csv_field* csv_record_release_data(struct csv_record* self)
{
	free_(self->_in->_fields);
	delete_(vec, self->_in->field_data);
	free_(self->_in);
	free_(self);

	return self->fields;
}

struct csv_record* csv_record_clone(const struct csv_record* src)
{
	struct csv_record* dest = NULL;
	dest = malloc_(sizeof(*dest));

	*dest = (struct csv_record) {
	        ._in = NULL,
	        .fields = NULL,
	        .rec = src->rec,
	        .reclen = src->reclen,
	        .size = src->size,
	};

	dest->_in = malloc_(sizeof(*dest->_in));
	*dest->_in = (struct csv_record_internal) {
	        .field_data = new_t_(vec, string),
	        ._fields = new_t_(vec, struct csv_field),
	        .rec_alloc = 0,
	        .field_alloc = src->size,
	};

	vec_resize(dest->_in->_fields, src->size);
	int i = 0;
	for (; i < src->size; ++i) {
		string* s_src = vec_at(src->_in->field_data, i);
		string* s_dest = vec_at(dest->_in->field_data, i);
		string_construct_from_string(s_dest, s_src);
	}

	vec_extend(dest->_in->_fields, src->_in->_fields);
	dest->fields = vec_begin(dest->_in->_fields);

	return dest;
}
