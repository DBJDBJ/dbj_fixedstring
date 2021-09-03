#ifndef VECTOR_H
#define VECTOR_H

/*
 *               RETURN CODES
 */

#define VECTOR_SUCCESS        0
#define VECTOR_MEMORY_ERROR  -1
#define VECTOR_EINVAL        -2
#define VECTOR_INTERN_ERROR  -3
#define VECTOR_GEN_ERROR     -4
#define VECTOR_IS_SUCCESS(arg) ((arg) >= 0)
#define VECTOR_IS_ERROR(arg) ((arg) < 0)
/*
 *
 */

#define VECTOR_API 
typedef int dbj_status;
typedef struct dbj_vector dbj_vector_t;


#define INVALID_VEC_INDEX ((size_t) -1)

VECTOR_API
dbj_vector_t *create_vector(size_t item_size, size_t capacity);

VECTOR_API
void destroy_vector(dbj_vector_t *);

VECTOR_API
size_t vector_size(const dbj_vector_t *);

VECTOR_API
size_t vector_capacity(const dbj_vector_t *);

VECTOR_API
int vector_push(dbj_vector_t *, const void *item);

VECTOR_API
int vector_insert(dbj_vector_t *, const void *item, size_t pos);

VECTOR_API
dbj_vector_t *vector_split(dbj_vector_t *, size_t pos);

VECTOR_API
const void *vector_at_c(const dbj_vector_t *vector, size_t index);

VECTOR_API
void *vector_at(dbj_vector_t *, size_t index);

VECTOR_API
dbj_status vector_swap(dbj_vector_t *cur_vec,dbj_vector_t *mv_vec, size_t pos);

VECTOR_API
void vector_clear(dbj_vector_t *);

VECTOR_API
int vector_erase(dbj_vector_t *, size_t index);

#ifdef VECTOR_TEST_BUILD
dbj_vector_t *copy_vector(dbj_vector_t *);
size_t vector_index_of(const dbj_vector_t *, const void *item);
#endif

#define VECTOR_AT(vector, pos, data_type) \
    *(data_type *)vector_at((vector), (pos))

#define VECTOR_AT_C(vector, pos, const_data_type) \
    *(const_data_type *)vector_at_c((vector), (pos))

#endif /* VECTOR_H */