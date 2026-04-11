#ifndef CSTRING_H_LIB_
#define CSTRING_H_LIB_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


#if defined(__clang__) || defined(__GNUC__)
    #define CSTR_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
    #define CSTR_NODISCARD _Check_return_
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define CSTR_NODISCARD [[nodiscard]]
#else
    #define CSTR_NODISCARD
#endif

#ifdef __unused
#undef __unused
#endif
#ifdef __always_inline
#undef __always_inline
#endif

#define __unused		    __attribute__((unused))
#define __always_inline		__attribute__((__always_inline__))



#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if		__has_builtin(__builtin_constant_p) || defined(__GNUC__)
#define pvt_builtin_constant_p(x)	__builtin_constant_p(x)
#else
#define pvt_builtin_constant_p(x)	0
#endif


// Types:

#define basic_cstring_type			char*
#define basic_const_cstring			const char*


typedef _Bool							bool_t;
typedef basic_cstring_type				cstring;
typedef basic_const_cstring				const_cstring;
typedef struct { cstring it; }			cstr_iterator;
typedef struct { const_cstring it; }	cstr_const_iterator;
typedef struct { cstring it; }			cstr_reverse_iterator;
typedef struct { const_cstring it; }	cstr_const_reverse_iterator;


#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#ifdef CSTRMAX
#undef CSTRMAX
#endif
#ifdef CSTRMIN
#undef CSTRMIN
#endif

#define CSTRMAX(A, B) \
	(A) > (B) ? (A) : (B)
#define CSTRMIN(A, B) \
	(A) < (B) ? (A) : (B)

#define CSTRING_DEFAULT_CAP		15
#define dummy_func				(void*)0


/* ----------------------------
        PRIVATE INTERFACE  
   ---------------------------- */

/* functions for allocation and deallocation need to correspond to each other,
   fall back to C library functions if not all are overridden */
#if !defined(pvt_cstr_malloc) || !defined(pvt_cstr_realloc) || \
	!defined(pvt_cstr_free)	  || !defined(pvt_cstr_memcpy) || \
	!defined(pvt_cstr_memmove)

#ifdef pvt_cstr_malloc
#undef pvt_cstr_malloc
#endif
#ifdef pvt_cstr_realloc
#undef pvt_cstr_realloc
#endif
#ifdef pvt_cstr_free
#undef pvt_cstr_free
#endif
#ifdef pvt_cstr_memcpy
#undef pvt_cstr_memcpy
#endif
#ifdef pvt_cstr_strlen
#undef pvt_cstr_strlen
#endif
#ifdef pvt_cstr_memmove
#undef pvt_cstr_memmove
#endif

#define pvt_cstr_malloc			malloc
#define pvt_cstr_realloc		realloc
#define pvt_cstr_free			free
#define pvt_cstr_memcpy		    memcpy
#define pvt_cstr_strlen			strlen
#define pvt_cstr_memmove		memmove
#endif

#ifdef pvt_cstr_assert_enabled
#undef pvt_cstr_assert_enabled
#endif
#ifdef pvt_cstr_assert
#undef pvt_cstr_assert
#endif
#ifdef pvt_cstr_static_assert
#undef pvt_cstr_static_assert
#endif

#define pvt_cstr_assert_enabled 1

#if pvt_cstr_assert_enabled
#define pvt_cstr_assert(cond, fmt)										\
    do{                                                                 \
        if( !(cond) ){                                                  \
            fprintf(stderr, __FILE__ ":%d assertion %s failed: %s\n",   \
                    __LINE__, #cond, fmt);                              \
            exit(1);                                                    \
        }                                                               \
    } while(0)

#define pvt_cstr_static_assert(cond, fmt)								\
    _Static_assert( (cond), fmt );

#else

#define pvt_cstr_assert(cond, str)
#define pvt_cstr_static_assert(cond, str)

#endif



//  metadata for cstring
typedef struct
{
    size_t size;
    size_t capacity;
    bool_t   is_malloced;
} pvt_cstr_metadata_t;



#define pvt_dat_to_mdata_(ptr)											\
    (((pvt_cstr_metadata_t *)(ptr)) - 1)

#define pvt_dat_to_const_mdata_(ptr)									\
    (((const pvt_cstr_metadata_t *)(ptr)) - 1)

#define pvt_total_cap_(ptr)												\
     ((ptr) ? pvt_dat_to_const_mdata_(ptr)->capacity : 0)

#define pvt_total_size_(ptr)										    \
      (ptr) ? pvt_dat_to_const_mdata_(ptr)->size : 0

#define pvt_is_malloced_(ptr)											\
      (ptr) ? pvt_dat_to_const_mdata_(ptr)->is_malloced : false


#define pvt_set_total_cap_(ptr, cap)                                    \
    do {                                                                \
        pvt_cstr_assert(ptr, "The cstring instance is null.");			\
        pvt_dat_to_mdata_(ptr)->capacity = (cap);						\
    } while(0)

#define pvt_set_total_size_(ptr, count)                                 \
    do {                                                                \
        pvt_cstr_assert(ptr, "The cstring instance is null.");			\
        pvt_dat_to_mdata_(ptr)->size = (count);                         \
    } while(0)

#define pvt_set_buf_malloced_(ptr, boolean)                             \
    do{                                                                 \
        pvt_cstr_assert(ptr, "The cstring instance is null.");			\
        pvt_dat_to_mdata_(ptr)->is_malloced = (boolean);                \
    } while(0)

#define pvt_buf_grow_(ptr, count)                                               \
    do{                                                                         \
        size_t tmp_size = (size_t)(count) * (sizeof(*(ptr)))                    \
                        + sizeof(pvt_cstr_metadata_t);                          \
        pvt_cstr_metadata_t *base_ptr;                                          \
        base_ptr = (ptr) ? (pvt_cstr_metadata_t*)pvt_cstr_realloc(				\
											pvt_dat_to_mdata_(ptr), tmp_size)   \
                         : (pvt_cstr_metadata_t*)pvt_cstr_malloc(tmp_size);		\
        pvt_cstr_assert(base_ptr != NULL, "allocation failed!!");				\
        base_ptr->is_malloced = true;                                           \
        base_ptr->capacity    = (size_t)(count);                                \
        (ptr)                 = (typeof(ptr))(base_ptr + 1);                    \
    } while(0)


#define pvt_copy_(str, other, count)                                            \
    do {                                                                        \
        if(!(str) || !(other)) break;                                           \
        pvt_cstr_memcpy((str), (other), (size_t)(count) * sizeof(*(other)));	\
        (str)[(size_t)(count)] = 0;                                             \
        pvt_set_total_size_(str, count);                                        \
    } while(0)

#define pvt_fill_(str, idx, cnt, ch)        \
    do {                                    \
        for(size_t i = 0; i < (cnt); ++i){  \
            (str)[(idx) + i] = (char)(ch);  \
        }                                   \
    } while(0)

/**
 * Generic Helpers
 *****************
 */

#define PVT_GENERIC_SIZE_TYPES( arg_01 )                \
	short     : arg_01, unsigned short     : arg_01,    \
    int       : arg_01, unsigned int       : arg_01,    \
    long      : arg_01, unsigned long      : arg_01,    \
    long long : arg_01, unsigned long long : arg_01

#define PVT_GENERIC_CHAR_TYPES( arg_01 )				\
	char	  : arg_01,	const char		   : arg_01,    \
    int       : arg_01

#define PVT_GENERIC_STRING_TYPES( arg_01 )              \
    cstring						: arg_01,				\
	const_cstring				: arg_01 

#define PVT_GENERIC_NORMAL_ITER( arg_01 )               \
    cstr_iterator				: arg_01,				\
    cstr_const_iterator			: arg_01

#define PVT_GENERIC_REVERSE_ITER( arg_01 )              \
    cstr_reverse_iterator		: arg_01,				\
    cstr_const_reverse_iterator : arg_01

#define PVT_GENERIC_ITER_TYPES( arg_01 )                \
    PVT_GENERIC_NORMAL_ITER(arg_01),                    \
    PVT_GENERIC_REVERSE_ITER(arg_01)

#define PVT_MUTABLE_CSTRING( str )						\
    ( _Generic((str), cstring : 1, default : 0 ) &&		\
	  !pvt_builtin_constant_p(str) )

#define PVT_IS_STRING( str )                            \
    ( _Generic((str),									\
			PVT_GENERIC_STRING_TYPES(1),				\
			default : 0))

// ============================================================================
// PUBLIC API
// ============================================================================

/* Generic constructor for cstring.
 *
 * Use like:
 *   cstr_init(str)                          -> default init
 *   cstr_init(str, size | "text")           -> size or copy init
 *   cstr_init(str, n, ch | first, last)     -> fill or range init
 *   cstr_init(str, other, start, offset)    -> substring init
 * Dispatches automatically based on argument count and types.
 */
#define cstr_init(...) \
    __cstr_init_chooser(__VA_ARGS__)(__VA_ARGS__)


/**
 *  Free all memory associated with the cstring and set it
 *  to NULL.
 */
static inline void
cstr_free(cstring *ptr)
{
    if(*ptr){
        pvt_set_buf_malloced_(*ptr, false);
        pvt_cstr_free(pvt_dat_to_mdata_(*ptr));
        (*ptr) = NULL;
    }
}



/**
 * Functions related to the size & capacity
 ******************************************
 */

/**
 * returns the total size of the cstring as size_t
 */
__always_inline
inline size_t __unused
cstr_size(const_cstring str)
{
    return pvt_total_size_(str);
}


/**
 * returns the total capacity of the cstring as size_t
 */
__always_inline
static inline size_t __unused
cstr_capacity(const_cstring str)
{
    return pvt_total_cap_(str);
}

/**
 * returns the maximum number of elements the cstring is able 
 * to hold due to system or library implementation limitations.
 */
__always_inline
static inline size_t __unused
cstr_max_size()
{
	return (size_t)-1;
}

/**
 * requests the removal of unused capacity. It is a request 
 * to reduce capacity() to size().
 */
static inline void __unused
cstr_shrink_to_fit(cstring *str)
{
    size_t size_01_ = cstr_size(*str);
    pvt_buf_grow_(*str, size_01_);
    pvt_set_total_cap_(*str, size_01_);
    (*str)[size_01_] = (char)0;
}
/**
 * requests that the cstring capacity be adapted to a planned 
 * change of up to new_cap characters.
 */
static inline 
void __unused
cstr_reserve(cstring *str, size_t new_cap)
{
	if(!(*str) || new_cap > cstr_capacity(*str)) {
		pvt_buf_grow_(*str, new_cap);
	}	
}

/**
 * requests the number of increments needed to go from first to last.
 */
#define cstr_distance( first, last )						    \
	(size_t)_Generic((first),								    \
			PVT_GENERIC_NORMAL_ITER((last).it - (first).it),    \
            PVT_GENERIC_REVERSE_ITER((first).it - (last).it)    \
		)

/*
 *   Input and Output 
 ********************
 */

/**
 * read the string buffer from the stream
 */
static void __unused
cstr_getstr(cstring *str, size_t cnt, FILE* stream)
{
    cstring str_02_ = *str;
    pvt_cstr_assert(cnt < cstr_capacity(str_02_),
                "count is larger than capacity to read.");
    char l_str_array[cnt];
    fgets(l_str_array, (int)cnt, stream);
    pvt_copy_(str_02_, &l_str_array[0], cnt);
}


/**
 * print cstring to the stdout with a neswline at the end.
 */
static void __unused
cstr_println(const_cstring str)
{
    fputs(str, stdout);
    putc('\n', stdout);
}


/*
 *  Element access
 *****************
 */

/**
 * return the character at position `pos` in the cstring,
 * else -1 is returned.
 */
__always_inline
static inline char __unused
cstr_at(const_cstring str, size_t pos)
{
    return (pos < cstr_size(str)) ? str[pos] : -1;
}

/**
 * return the first character in the cstring, else -1 is returned.
 */
__always_inline
static inline char __unused
cstr_front(const_cstring str)
{
    return (str) ? str[0] : -1;
}

/**
 * return the last character in the cstring, else -1 is returned.
 */
__always_inline
static inline char __unused
cstr_back(const_cstring str)
{
    return (str) ? str[cstr_size(str) - 1] : -1;
}

/**
 * return true/1 if NULL or empty, false/0 if non-empty.
 */
__always_inline
static inline bool_t __unused
cstr_empty(const_cstring str)
{
    return (cstr_size(str) == (size_t)0);
}


/*
 * Iterators
 ***************
 */

/**
 * returns a mutable iterator to the first character of the cstring.
 */
__always_inline
static inline cstr_iterator __unused
cstr_begin(cstring str)
{
    return (cstr_iterator){ str };
}

/**
 * returns a constant iterator to the first character of the cstring.
 */
__always_inline
static inline cstr_const_iterator __unused
cstr_cbegin(const_cstring str)
{
    return (cstr_const_iterator){ str };
}

/**
 * returns a mutable iterator to the character following the last 
 * character of the string. This character acts as a placeholder, 
 * attempting to access it results in undefined behavior. 
 */
__always_inline
static inline cstr_iterator __unused
cstr_end(cstring str)
{
    return (cstr_iterator){ (str + cstr_size(str)) };
}

/**
 * returns a constant iterator to the character following the last
 * character of the string. This character acts as a placeholder,
 * attempting to access it results in undefined behavior.
 */
__always_inline
static inline cstr_const_iterator __unused
cstr_cend(const_cstring str)
{
    return (cstr_const_iterator){ (str + cstr_size(str)) };
}

/**
 * returns a mutable reverse iterator to the first character of the
 * reversed cstring. It corresponds to the last character of the
 * non-reversed cstring.
 */
__always_inline
static inline cstr_reverse_iterator __unused
cstr_rbegin(cstring str)
{
    return (cstr_reverse_iterator){ (str + cstr_size(str) - 1) };
}

/**
 * returns a constant reverse iterator to the first character of the
 * reversed cstring. It corresponds to the last character of the
 * non-reversed cstring.
 */
__always_inline
static inline cstr_const_reverse_iterator __unused
cstr_crbegin(const_cstring str)
{
    return (cstr_const_reverse_iterator){ (str + cstr_size(str) - 1) };
}

/**
 * returns a mutable reverse iterator to the character following the last
 * character of the reversed cstring. It corresponds to the character
 * preceding the first character of the non-reversed cstring.This character
 * acts as a placeholder, attempting to access it results in undefined behavior.
 */
__always_inline
static inline cstr_reverse_iterator __unused
cstr_rend(cstring str)
{
    return (cstr_reverse_iterator){ (str - 1) };
}

/**
 * returns a constant reverse iterator to the character following the last
 * character of the reversed cstring. It corresponds to the character
 * preceding the first character of the non-reversed cstring. This character
 * acts as a placeholder, attempting to access it results in undefined behavior.
 */
__always_inline
static inline cstr_const_reverse_iterator __unused
cstr_crend(const_cstring str)
{
    return (cstr_const_reverse_iterator){ (str - 1) };
}

/**
 * move the iterator to the next element.
 */
#define iter_next(it)                                   \
    _Generic((it),                                      \
        cstr_iterator*: intl_iter_next,                 \
        cstr_const_iterator*: intl_citer_next,          \
        cstr_reverse_iterator*: intl_riter_next,        \
        cstr_const_reverse_iterator*: intl_criter_next, \
        default : dummy_func                            \
    )(it)

/**
 * move the iterator to the previous element.
 */
#define iter_prev(it)                                   \
    _Generic((it),                                      \
        cstr_iterator*: intl_iter_prev,                 \
        cstr_const_iterator*: intl_citer_prev,          \
        cstr_reverse_iterator*: intl_riter_prev,        \
        cstr_const_reverse_iterator*: intl_criter_prev, \
        default : dummy_func                            \
    )(it)


/*********************************
 *	---- Operations ----
 */

static cstring __unused
cstr_substr(const_cstring str, const size_t pos, const size_t count)
{
	cstring substr_ = NULL;
	pvt_buf_grow_(substr_, count + 1);
	pvt_copy_(substr_, str + pos, count);
	return substr_;
}


/*
 * Modifiers
 ************
 */


/* Generic insert for cstring.
 *
 * Usage:
 *   cstr_insert(str, index, ch | "text")           -> insert char or string
 *   cstr_insert(str, index, count, ch | "text")    -> insert repeated char or string
 *   cstr_insert(str, index, other, pos, count)     -> insert substring
 */
#define cstr_insert(...) \
    __cstr_insert_chooser(__VA_ARGS__)(__VA_ARGS__)

/* Generic erase function for cstring.
 *
 * Usage:
 *   cstr_erase(str, index, count)        -> erase by index
 *   cstr_erase(str, pos)                 -> erase single element (iterator)
 *   cstr_erase(str, first, last)         -> erase range
 * Works with both iterator and const_iterator via _Generic dispatch.
 */
#define cstr_erase(str, a, ...)                         \
    _Generic((a),                                       \
        PVT_GENERIC_SIZE_TYPES(intl_erase_idx),         \
        cstr_iterator: _Generic((__VA_ARGS__),          \
            cstr_iterator: intl_erase_range_iter,       \
            default: intl_erase_ch_iter                 \
        ),                                              \
        cstr_const_iterator: _Generic((__VA_ARGS__),    \
            cstr_const_iterator: intl_erase_range_citer,\
            default: intl_erase_ch_citer                \
        )                                               \
    )(str, a, ##__VA_ARGS__)

/*
 * Assigns new content to the given cstring.
 *
 * Usage:
 *   intl_assign(str, other);               -> assign entire string
 *   intl_assign(str, other, count);        -> assign first 'count' characters
 *   intl_assign(str, other, pos, count);   -> assign substring [pos, pos+count)
 *   intl_assign(str, count, ch);           -> assign 'count' copies of character 'ch'
 */
#define cstr_assign(str, a, ...)                    \
    _Generic((a),                                   \
        PVT_GENERIC_STRING_TYPES(intl_assign_str),  \
        PVT_GENERIC_SIZE_TYPES(intl_assign_cnt_ch)  \
    )(str, a, ##__VA_ARGS__)

/**
 * Removes all characters from the string. All pointers, 
 * references, and iterators are invalidated.
 */
static void __unused
cstr_clear(cstring *str)
{
    pvt_fill_(*str, 0, cstr_size(*str), 0);
    pvt_set_total_size_(*str, 0);
}

/*
 * Appends the given character ch to the end of the cstring.
 */
void __unused
cstr_push_back(cstring *str);

/*
 * Removes the last character from the cstring.
 */
void __unused
cstr_pop_back(cstring *str);

// ============================================================================
// INTERNAL MACROS (DO NOT USE)
// ============================================================================

#define __cstr_init_get_4th(_1,_2,_3,NAME,...) NAME

#define __cstr_init_chooser(...)        \
    __cstr_init_get_4th(__VA_ARGS__,    \
        __cstr_init_3,                  \
        __cstr_init_2,                  \
        __cstr_init_1,                  \
        __cstr_init_0)

#define __cstr_insert_get_6th(_1,_2,_3,_4,_5,NAME,...) NAME

#define __cstr_insert_chooser(...)      \
    __cstr_insert_get_6th(__VA_ARGS__,  \
        __cstr_insert_5,                \
        __cstr_insert_4,                \
        __cstr_insert_3)

#define CSTR_GET_MACRO(_1,_2,_3,_4,NAME,...) NAME

#define __cstr_assign_str(str, other, ...)  \
    CSTR_GET_MACRO(__VA_ARGS__,             \
        intl_assign_str_range,              \
        intl_assign_str_offset,             \
        intl_assign_str                     \
    )(str, other, ##__VA_ARGS__)

// ============================================================================
//  WRAPPERS
// ============================================================================

/*  Constructor */
#define __cstr_init_0()                                 \
    intl_default_constructor()

#define __cstr_init_1(a)                                \
    _Generic((a),                                       \
        PVT_GENERIC_SIZE_TYPES(intl_init_w_size),       \
        PVT_GENERIC_STRING_TYPES(intl_copy_constructor) \
    )(a)

#define __cstr_init_2(a, b)                             \
    _Generic((a),                                       \
        PVT_GENERIC_SIZE_TYPES(intl_init_cpy_ch),       \
        PVT_GENERIC_ITER_TYPES(intl_init_cpy_str_w_iter)\
    )(a, b)

#define __cstr_init_3(other, start, offset)             \
    intl_init_cpy_str_w_off(other, start, offset)


/*  Insert */
#define __cstr_insert_3(str, index, x)                  \
    _Generic((x),                                       \
        PVT_GENERIC_CHAR_TYPES(intl_insert_ch),         \
        PVT_GENERIC_STRING_TYPES(intl_insert_str)       \
    )(str, index, x)

#define __cstr_insert_4(str, index, a, b)               \
    _Generic((b),                                       \
        PVT_GENERIC_CHAR_TYPES(intl_insert_cnt_ch),     \
        PVT_GENERIC_STRING_TYPES(intl_insert_cnt_str)   \
    )(str, index, a, b)

#define __cstr_insert_5(str, index, other, pos, count)  \
    intl_insert_substr(str, index, other, pos, count)


// ============================================================================
// INTERNAL FUNCTIONS (DO NOT USE)
// ============================================================================


/*
 *  Functions to move the iterators to forward or backward
 *  ------------------------------------------------------
 */

__always_inline
static inline void __unused
intl_iter_next(cstr_iterator *it) {
    it->it++;
}

__always_inline
static inline void __unused
intl_citer_next(cstr_const_iterator *it) {
    it->it++;
}

__always_inline
static inline void __unused
intl_riter_next(cstr_reverse_iterator *it) {
    it->it--;
}

__always_inline
static inline void __unused
intl_criter_next(cstr_const_reverse_iterator *it) {
    it->it--;
}

__always_inline
static inline void __unused
intl_iter_prev(cstr_iterator *it) {
    it->it--;
}

__always_inline
static inline void __unused
intl_citer_prev(cstr_const_iterator *it) {
    it->it--;
}

__always_inline
static inline void __unused
intl_riter_prev(cstr_reverse_iterator *it) {
    it->it++;
}

__always_inline
static inline void __unused
intl_criter_prev(cstr_const_reverse_iterator *it) {
    it->it++;
}


/*
 *	Functions to cstring overloaded Constructor
 *  -------------------------------------------
 */

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_init_cpy_ch(const size_t cnt, const char ch);

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_copy_constructor(const_cstring other);

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_init_cpy_str_w_iter(cstr_iterator begin, 
                         cstr_iterator end);

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_init_cpy_str_w_off(const_cstring other,
                        const size_t start, const size_t offset);

CSTR_NODISCARD
__attribute__((malloc))
static inline cstring __unused
intl_init_w_size(const size_t cnt)
{
    return intl_init_cpy_ch(cnt, ' ');
}

CSTR_NODISCARD
__attribute__((malloc))
static inline cstring __unused 
intl_default_constructor()
{
    return intl_init_cpy_ch(0, 0);
}

/*
 *  Functions to assign cstring
 *  ---------------------------
 */

cstring __unused
intl_assign_str_range(cstring *str, const_cstring other,
                      const size_t pos, const size_t count);

cstring __unused
intl_assign_cnt_ch(cstring *str, const size_t count, const char ch);

static inline cstring __unused
intl_assign_str_offset(cstring *str, const_cstring other, 
                       const size_t count)
{
    return intl_assign_str_range(str, other, 0, count);
}

static inline cstring __unused
intl_assign_str(cstring *str, const_cstring other)
{
    return intl_assign_str_range(str, other, 0, pvt_cstr_strlen(other));
}


//****************************
//	        Modifiers
//****************************


/*
 *  functions to erase
 *********************
 */

cstr_iterator __unused
intl_erase_range_iter(cstring *str, cstr_iterator first,
                      cstr_iterator last);

static inline cstr_const_iterator __unused
intl_erase_range_citer(cstring *str,
                       cstr_const_iterator first,
                       cstr_const_iterator last)
{
    return (cstr_const_iterator)
    {intl_erase_range_iter(str, 
            (cstr_iterator){(cstring)first.it}, 
            (cstr_iterator){(cstring)last.it}).it};
}

static inline cstring __unused
intl_erase_idx(cstring *str, const size_t index,
               const size_t count)
{
    cstring str_ = *str;
    size_t count_ = index + 
                (CSTRMIN(count, cstr_size(str_) - index));
    intl_erase_range_iter(str, cstr_begin(str_ + index), 
                            cstr_begin(str_ + count_));
    return str_;
}

static inline cstr_iterator __unused
intl_erase_ch_iter(cstring *str, cstr_iterator pos)
{
    return intl_erase_range_iter(str, pos, 
                (cstr_iterator){pos.it + 1});
}

static inline cstr_const_iterator __unused
intl_erase_ch_citer(cstring *str, cstr_const_iterator pos)
{
    return intl_erase_range_citer(str, pos, 
                (cstr_const_iterator){pos.it + 1});
}


/*
 *	functions to insert with multiple args
 *****************************************
 */

cstring __unused
intl_insert_cnt_ch(cstring *str, const size_t index, 
                   const size_t count, const char ch);

cstring __unused
intl_insert_cnt_str(cstring *str, const size_t index, 
                    const size_t count, const_cstring other);


static inline cstring __unused
intl_insert_ch(cstring *str, const size_t index, const char ch)
{
    return intl_insert_cnt_ch(str, index, 1, ch);
}

static inline cstring __unused
intl_insert_str(cstring *str, const size_t index, const_cstring other)
{
    return intl_insert_cnt_str(str, index, 1, other);
}

static inline cstring __unused
intl_insert_substr(cstring *str, const size_t index, 
		           const_cstring other, const size_t pos,
                   const size_t count)
{
	return intl_insert_str(str, index, cstr_substr(other, pos, count));
}


/*
 *	functions to replace with multiple args
 ******************************************
 */




#endif
