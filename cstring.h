#ifndef CSTRING_H_LIB_
#define CSTRING_H_LIB_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


#define __cstring_ 2

#ifdef __unused
#undef __unused
#endif
#ifdef __always_inline
#undef __always_inline
#endif

#define __unused					__attribute__((unused))
#define __always_inline				__attribute__((__always_inline__))

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
	!defined(pvt_cstr_free)	  || !defined(pvt_cstr_strncpy) || \
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
#ifdef pvt_cstr_strncpy
#undef pvt_cstr_strncpy
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
#define pvt_cstr_strncpy		strncpy
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
        pvt_cstr_strncpy((str), (other), (size_t)(count) * sizeof(*(other)));	\
        (str)[(size_t)(count)] = 0;                                             \
        pvt_set_total_size_(str, count);                                        \
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

#define PVT_GENERIC_STRING_TYPES( arg_01 )              \
    cstring						: arg_01,				\
	const_cstring				: arg_01 

#define PVT_GENERIC_ITER_TYPES( arg_01 )                \
    cstr_iterator				: arg_01,				\
	cstr_const_iterator			: arg_01,				\
    cstr_reverse_iterator		: arg_01,				\
	cstr_const_reverse_iterator : arg_01

#define PVT_MUTABLE_CSTRING( str )						\
    ( _Generic((str), cstring : 1, default : 0 ) &&		\
	  !pvt_builtin_constant_p(str) )

#define PVT_IS_STRING( str )                            \
    ( _Generic((str),									\
			PVT_GENERIC_STRING_TYPES(1),				\
			default : 0))


/******************************
        PUBLIC INTERFACE      
 ******************************
 */

/**
 *	Constructs new string from a variety of data sources.
 */
#define cstring_init(...)                                   \
    PVT_CSTRING_GET_MACRO(__VA_ARGS__, pvt_init_4_param,    \
        pvt_init_3_param, pvt_init_2_param,                 \
        pvt_init_1_param, dummy_func                        \
        )(__VA_ARGS__)

/**
 *  Free all memory associated with the cstring and set it
 *  to NULL.
 */
static inline void
cstring_free(cstring *ptr)
{
    if(*ptr){
        pvt_set_buf_malloced_(*ptr, false);
        pvt_cstr_free(pvt_dat_to_mdata_(*ptr));
        (*ptr) = NULL;
    }
}

/*
 *   Private functional macros used for initialization
 *****************************************************
 */

#define PVT_CSTRING_GET_MACRO(_1,_2,_3,_4,NAME,...) NAME

#define pvt_init_1_param( _1 )                                          \
    do{                                                                 \
        pvt_cstr_static_assert(PVT_MUTABLE_CSTRING(*(_1)),              \
                                   "##_1 is not cstring");              \
        intl_default_constructor(_1);                                   \
    } while(0)

#define pvt_init_2_param( _1, _2 )                                      \
    do{                                                                 \
        pvt_cstr_static_assert(PVT_MUTABLE_CSTRING(*(_1)),              \
                                   "##_1 is not cstring");              \
        pvt_cstr_static_assert(_Generic((_2),                           \
            PVT_GENERIC_SIZE_TYPES(1),                                  \
            PVT_GENERIC_STRING_TYPES(1),                                \
            default : 0                                                 \
            ), "Unsupported arguments passed"                           \
        );                                                              \
        _Generic((_2),                                                  \
            PVT_GENERIC_SIZE_TYPES(intl_init_w_size),                   \
            PVT_GENERIC_STRING_TYPES(intl_copy_constructor),            \
            default : dummy_func                                        \
        )(_1, _2);                                                      \
    } while(0) 

#define pvt_init_3_param( _1, _2, _3)	                                \
    do{                                                                 \
        pvt_cstr_static_assert(PVT_MUTABLE_CSTRING(*(_1)),              \
                                   "##_1 is not cstring");              \
        pvt_cstr_static_assert(_Generic((_2),                           \
            PVT_GENERIC_SIZE_TYPES(is_char_literal(#_3)),               \
            PVT_GENERIC_ITER_TYPES(_Generic(_3,                         \
                PVT_GENERIC_ITER_TYPES(1), default : 0                  \
                )),                                                     \
            default : 0                                                 \
            ), "Unsupported arguments passed"                           \
        );                                                              \
        _Generic( (_2),                                                 \
            PVT_GENERIC_SIZE_TYPES(intl_init_cpy_ch),                   \
            PVT_GENERIC_ITER_TYPES(intl_init_cpy_str_w_iter),           \
            default : dummy_func                                        \
        )(_1, _2, _3);                                                  \
    } while(0)

#define pvt_init_4_param( _1, _2, _3, _4)	                            \
    do{                                                                 \
        pvt_cstr_static_assert(PVT_MUTABLE_CSTRING(*(_1)),              \
                                   "##_1 is not cstring");              \
        pvt_cstr_static_assert(PVT_IS_STRING(_2),                       \
                                    "##_2 is not string");              \
        pvt_cstr_static_assert(_Generic((_3),                           \
            PVT_GENERIC_SIZE_TYPES(_Generic(_4,                         \
                PVT_GENERIC_SIZE_TYPES(1), default : 0                  \
                )),                                                     \
            default : 0                                                 \
            ), "Unsupported arguments passed"                           \
        );                                                              \
        _Generic( (_3),                                                 \
            PVT_GENERIC_SIZE_TYPES(intl_init_cpy_str_w_off),            \
            default : dummy_func                                        \
        )(_1, _2, _3, _4);                                              \
    } while(0)

/**
 * Functions related to the size & capacity
 ******************************************
 */

/**
 * returns the total size of the cstring as size_t
 */
__always_inline
inline size_t __unused
cstring_size(const_cstring str)
{
    return pvt_total_size_(str);
}


/**
 * returns the total capacity of the cstring as size_t
 */
__always_inline
static inline size_t __unused
cstring_capacity(const_cstring str)
{
    return pvt_total_cap_(str);
}

/**
 * returns the maximum number of elements the cstring is able 
 * to hold due to system or library implementation limitations.
 */
__always_inline
static inline size_t __unused
cstring_max_size()
{
	return (size_t)-1;
}

/**
 * requests the removal of unused capacity. It is a request 
 * to reduce capacity() to size().
 */
static void __unused
cstring_shrink_to_fit(cstring *str)
{
    size_t size_01_ = cstring_size(*str);
    if (size_01_ > 0 && cstring_capacity(*str) > size_01_) {
         cstring tmp = (cstring)realloc(pvt_dat_to_mdata_(*str),
				 size_01_ + sizeof(pvt_cstr_metadata_t));
        if (tmp) {
            *str = tmp + sizeof(pvt_cstr_metadata_t);
            pvt_set_total_cap_(*str, size_01_);
        }
    }
}

/**
 * requests that the cstring capacity be adapted to a planned 
 * change of up to new_cap characters.
 */
static void __unused
cstring_reserve(cstring *str, size_t new_cap)
{
	if(new_cap > cstring_capacity(*str) || 
	   ( new_cap > cstring_size(*str) && 
			(new_cap << 2) < cstring_capacity(*str))
	   )
	{
		pvt_buf_grow_(*str, new_cap);
	}	
}

/**
 * requests the number of increments needed to go from first to last.
 */
#define cstring_distance( first, last )						\
	(size_t)_Generic((first),								\
			PVT_GENERIC_ITER_TYPES(_Generic((last),			\
				PVT_GENERIC_ITER_TYPES(						\
					((last).it) - ((first).it)				\
					))),									\
			default : 0										\
		)

/*
 *   Input and Output 
 ********************
 */

/**
 * read the string buffer from the stream
 */
static void __unused
cstring_getstr(cstring *str, size_t cnt, FILE* stream)
{
    cstring str_02_ = *str;
    pvt_cstr_assert(cnt < cstring_capacity(str_02_),
                "count is larger than capacity to read.");
    char l_str_array[cnt];
    fgets(l_str_array, (int)cnt, stream);
    pvt_copy_(str_02_, &l_str_array[0], cnt);
}


/**
 * print cstring to the stdout with a neswline at the end.
 */
static void __unused
cstring_println(const_cstring str)
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
cstring_at(const_cstring str, size_t pos)
{
    return (pos < cstring_size(str)) ? str[pos] : -1;
}

/**
 * return the first character in the cstring, else -1 is returned.
 */
__always_inline
static inline char __unused
cstring_front(const_cstring str)
{
    return (str) ? str[0] : -1;
}

/**
 * return the last character in the cstring, else -1 is returned.
 */
__always_inline
static inline char __unused
cstring_back(const_cstring str)
{
    return (str) ? str[cstring_size(str) - 1] : -1;
}

/**
 * return true/1 if NULL or empty, false/0 if non-empty.
 */
__always_inline
static inline bool_t __unused
cstring_empty(const_cstring str)
{
    return (cstring_size(str) == (size_t)0);
}


/*
 * Iterators
 ***************
 * /

/**
 * returns a mutable iterator to the first character of the cstring.
 */
__always_inline
static inline cstr_iterator __unused
cstring_begin(cstring str)
{
    return (cstr_iterator){ str };
}

/**
 * returns a constant iterator to the first character of the cstring.
 */
__always_inline
static inline cstr_const_iterator __unused
cstring_cbegin(const_cstring str)
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
cstring_end(cstring str)
{
    return (cstr_iterator){ (str + pvt_cstr_strlen(str)) };
}

/**
 * returns a constant iterator to the character following the last
 * character of the string. This character acts as a placeholder,
 * attempting to access it results in undefined behavior.
 */
__always_inline
static inline cstr_const_iterator __unused
cstring_cend(const_cstring str)
{
    return (cstr_const_iterator){ (str + pvt_cstr_strlen(str)) };
}

/**
 * returns a mutable reverse iterator to the first character of the
 * reversed cstring. It corresponds to the last character of the
 * non-reversed cstring.
 */
__always_inline
static inline cstr_reverse_iterator __unused
cstring_rbegin(cstring str)
{
    return (cstr_reverse_iterator){ (str + pvt_cstr_strlen(str) - 1) };
}

/**
 * returns a constant reverse iterator to the first character of the
 * reversed cstring. It corresponds to the last character of the
 * non-reversed cstring.
 */
__always_inline
static inline cstr_const_reverse_iterator __unused
cstring_crbegin(const_cstring str)
{
    return (cstr_const_reverse_iterator){ (str + pvt_cstr_strlen(str) - 1) };
}

/**
 * returns a mutable reverse iterator to the character following the last
 * character of the reversed cstring. It corresponds to the character
 * preceding the first character of the non-reversed cstring.This character
 * acts as a placeholder, attempting to access it results in undefined behavior.
 */
__always_inline
static inline cstr_reverse_iterator __unused
cstring_rend(cstring str)
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
cstring_crend(const_cstring str)
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


/*
 * Modifiers
 ************
 * /

/**
 * assign a string to a cstring. The cstring, can be a NULL string.
 * If `str` refers to an existing cstring, the old content is
 * overwritten.
 */
static void __unused
cstring_assign(cstring *str, const_cstring other)
{
    cstring str_03_ = *str;
    size_t count = pvt_cstr_strlen(other);
    if ((!str_03_) || (count + 1) >= pvt_total_cap_(str_03_)) {
        pvt_buf_grow_(str_03_, (count + 1));        
    }
    if (other && count) {
        pvt_copy_(str_03_, other, count);
    } else {
        pvt_set_total_size_(str_03_, 1);
        str_03_[0] = 0;
    }
    *str = str_03_;
}


/*********************************
 *	---- Operations ----
 */

static cstring __unused
cstring_substr(const_cstring str, const size_t pos, const size_t count)
{
	cstring substr_ = NULL;
	pvt_buf_grow_(substr_, count + 1);
	pvt_copy_(substr_, str + pos, count);
	pvt_set_total_size_(substr_, count);
	substr_[count] = 0;
	return substr_;
}



/**
 *  private and helper functions....
 *  DO NOT USE THESE FUNCTIONS OUT OF THIS HEADER
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
 *	Constructors
 *****************
 */

static void __unused
intl_init_cpy_ch(cstring *str, size_t cnt, char ch)
{
    cstring str_01_ = *str;
    pvt_buf_grow_(str_01_, cnt);
    if(ch != -1 ){
        for(size_t i = 0; i < cnt; ++i)
			str_01_[i] = ch;
    } else cnt = 0;
    str_01_[cnt] = '\0';
	pvt_set_total_size_(str_01_, cnt); 
    *str = str_01_;
}

static void __unused
intl_init_w_size(cstring *str, size_t cnt)
{
    intl_init_cpy_ch(str, cnt, ' ');
}

static void __unused 
intl_default_constructor(cstring *str)
{
    intl_init_cpy_ch(str, CSTRING_DEFAULT_CAP, -1);
}

static void __unused
intl_copy_constructor(cstring *str, const_cstring other)
{
    cstring str_01_ = *str;
    pvt_cstr_assert( !(str_01_), 
            "cstring is not empty to initialize");
    size_t size = pvt_cstr_strlen(other);
    pvt_buf_grow_(str_01_, size + 1);
    pvt_copy_(str_01_, other, size);
    *str = str_01_;
}

static void __unused
intl_init_cpy_str_w_iter(cstring *str, cstr_iterator begin, 
        cstr_iterator end)
{
    if(begin.it >= end.it) return;
    cstring str_01_ = *str;
    size_t l_size = (size_t)(end.it - begin.it) + 1;
    pvt_buf_grow_(str_01_, l_size);
    for(size_t i = 0; i < l_size; ++i) {
        str_01_[i] = *(begin.it);
        iter_next(&begin);
    }
    str_01_[l_size] = 0;
    pvt_set_total_size_(str_01_, l_size);
    *str = str_01_;
}

static void __unused
intl_init_cpy_str_w_off(cstring *str, const_cstring other,
                        size_t start, size_t offset) 
{
    cstring str_01_ = *str;
    size_t othersize  = pvt_cstr_strlen(other) + 1;
    pvt_cstr_assert(other != NULL && (start < othersize) &&
                (start + offset <= othersize), 
                "Unsupported arguments passed");
    pvt_buf_grow_(str_01_, offset);
    pvt_cstr_strncpy(str_01_, other + start, offset);
    str_01_[offset] = '\0';
    pvt_set_total_size_(str_01_, offset);
    *str = str_01_;
}


/*********************************
 *	---- Modifiers ----
 */


static cstring __unused
intl_replace_cnt_offset_with_str(cstring *str, const size_t pos, 
						const size_t cnt, const_cstring other)
{
	cstring str_08_ = *str;
	size_t size = cstring_size(str_08_),
		   start = CSTRMIN(pos, size),
		   count = CSTRMIN(cnt, size - start);
	count = CSTRMIN(count, pvt_cstr_strlen(other));

	for(int i = 0; i < count; ++i)
		str_08_[start + i] = other[i];
	
	return str_08_;
}

static cstring __unused
intl_replace_str_with_iterator(cstring *str, const_cstring first, 
						const_cstring last, const_cstring other)
{
	cstring str_09_ = *str;

}


static cstring __unused
intl_insert_cnt_of_ch_at_idx(cstring str, const size_t index,
			const size_t count, const char ch)
{
	cstring str_04_ = str;
	const size_t size_02_ = cstring_size(str_04_),
		   cap_01_  = cstring_capacity(str_04_),
		   ttl_cap  = size_02_ + count;
	
	pvt_cstr_assert(index <= size_02_, "index for insert is out of bounds");
	if(cap_01_ < ttl_cap)
		pvt_buf_grow_(str_04_, ttl_cap);									// adjust the capacity
	
	pvt_cstr_memmove(&str_04_[index + count], &str_04_[index], size_02_ - index);
	for(size_t i = 0; i < count; ++i)
		str_04_[index + i] = ch;

	str_04_[ttl_cap] = 0;
	pvt_set_total_size_(str_04_, ttl_cap);
	return str_04_;
}

static cstring __unused
intl_insert_cnt_of_str_at_idx(cstring str, const size_t index,
			const size_t count, const_cstring other)
{
	cstring str_05_ = str;
	const size_t size_03_ = cstring_size(str_05_),
		   other_sz = pvt_cstr_strlen(other),
		   offset   = other_sz * count,
		   ttl_cap	= size_03_ + offset;
	
	pvt_cstr_assert(index <= size_03_, "index for insert is out of bounds");
	if(cstring_capacity(str_05_) < ttl_cap)
		pvt_buf_grow_(str_05_, ttl_cap);								// adjust the capacity

	pvt_cstr_memmove(&str_05_[index + offset], &str_05_[index], size_03_ - index);
	for(size_t i = 0; i < count; ++i)
		pvt_cstr_memmove(&str_05_[index + (i * other_sz)], other, other_sz);

	str_05_[ttl_cap] = 0;
	pvt_set_total_size_(str_05_, ttl_cap);
	return str_05_;
}

static cstring __unused
intl_insert_str_at_idx(cstring str, const size_t index, const_cstring other)
{
	return intl_insert_cnt_of_str_at_idx(str, index, 1, other);
}

static cstring __unused
intl_insert_cnt_of_substr_at_idx(cstring str, const size_t index, 
		const_cstring other, const size_t pos, const size_t count)
{
	cstring str_06_ = str,
			substr_01_ = cstring_substr(str, pos, count);
	str_06_ = intl_insert_str_at_idx(str_06_, index, substr_01_);
	return str_06_;
}

static cstring __unused
intl_insert_cnt_of_ch_at_iter(cstring str, cstr_iterator pos, 
		const size_t count, const char ch)
{
	cstring str_07_ = str;
	cstr_iterator end = cstring_end(str);
	const size_t size_03_ = cstring_size(str_07_),
		   cap_02_  = cstring_capacity(str_07_),
		   ttl_cap  = size_03_ + count;
	
	pvt_cstr_assert(pos.it != end.it, "index for insert is out of bounds");
	if(cap_02_ < ttl_cap)
		pvt_buf_grow_(str_07_, ttl_cap);									// adjust the capacity
	
	pvt_cstr_memmove(pos.it + count, pos.it, 
			size_03_ - cstring_distance(pos, end));
	
	return str_07_;
}

#endif
