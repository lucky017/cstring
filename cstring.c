
#include "cstring.h"


// =========================================================
//                      PUBLIC API
// ======================================================== 


void __cstr__unused
cstr_shrink_to_fit(cstring *str)
{
    if (!(*str) || !pvt_is_malloced_(*str)) return;
    cstring str_ = *str;
    size_t size_ = cstr_size(str_);
    if (size_ < cstr_capacity(str_)) {
        size_t size_001_ = sizeof(pvt_cstr_metadata_t) + (size_ + 1) * sizeof(char);
        pvt_cstr_metadata_t *mdata = (pvt_cstr_metadata_t *)
                            pvt_cstr_realloc(pvt_dat_to_mdata_(str_), size_001_);
        if (mdata) {
            mdata->capacity = size_;
            str_ = (cstring)(mdata + 1);
        }
    }
}

/*
 *  Input & Output
 * ---------------
 */
void __cstr__unused
cstr_getstr(cstring *str, size_t cnt, FILE* stream)
{
    cstring str_02_ = *str;
    pvt_cstr_assert(cnt < cstr_capacity(str_02_),
                "count is larger than capacity to read.");

    size_t read_01_ = fread(str_02_, sizeof(*str_02_), cnt, stream);
    str_02_[read_01_] = (char)0;
    pvt_set_total_size_(str_02_, read_01_);
}


void __cstr__unused
cstr_println(const_cstring str)
{
    fputs(str, stdout);
    putc('\n', stdout);
}

/**
 *  Modifiers
 * ----------
 */

void __cstr__unused
cstr_push_back(cstring *str, const char ch)
{
    intl_append_cnt_ch(str, 1, ch);
}

void __cstr__unused
cstr_pop_back(cstring *str)
{
    intl_erase_ch_iter(str, (cstr_iterator){cstr_end(*str).it - 1});
}

size_t __cstr__unused 
cstr_copy(cstring *str, const_cstring other, const size_t count,
          const size_t pos)
{
    cstring str_ = *str;
    size_t count_ = CSTRMIN(cstr_size(str_), 
                        CSTRMIN(count, pvt_cstr_strlen(other) - pos)
                    );
    
    pvt_cstr_memmove(str_, other + pos, count_);
    return count_;
}

void __cstr__unused
cstr_resize(cstring *str, const size_t count, const char ch)
{
    cstring str_ = NULL;
    size_t size_ = cstr_size(*str);
    
    cstr_reserve(str, count + 1);
    str_ = *str;

    if(count > size_) {
        pvt_fill_(str_, size_, count - size_, ch);
    }

    str_[count] = (char)0;
    pvt_set_total_size_(str_, count);
}


void __cstr__unused
cstr_swap(cstring *str, cstring *other)
{
    cstring tmp = *str;
    *str = *other;
    *other = tmp;
}

/**
 *  Operations
 * -----------
 */

cstring __cstr__unused
cstr_substr(const_cstring str, const size_t pos, const size_t count)
{
    pvt_cstr_assert(pos < cstr_size(str), "Index out of Bounds");
	cstring substr_ = NULL;
    size_t count_ = CSTRMIN(count, cstr_size(str) - pos);

	pvt_buf_grow_(substr_, count_ + 1);
	pvt_copy_(substr_, str + pos, count_);
	return substr_;
}


// =========================================================
//                     PRIVATE FUNCTIONS
// ========================================================

/*
 * Functions to assign
 * -------------------
 */

cstring __cstr__unused
intl_assign_str_range(cstring *str, const_cstring other,
                      const size_t pos, const size_t count)
{
    size_t size_ = pvt_cstr_strlen(other);
    if(pos >= size_) return *str;
    size_t count_ = CSTRMIN(count, size_ - pos);

    cstr_reserve(str, count_ + 1);
    cstring str_ = *str;

    pvt_cstr_memmove(str_, other + pos, count_);

    str_[count_] = (char)0;
    pvt_set_total_size_(str_, count_);
    return str_;
}

cstring __cstr__unused
intl_assign_cnt_ch(cstring *str, const size_t count, const char ch)
{
    cstr_reserve(str, count + 1);
    cstring str_ = *str;

    pvt_fill_(str_, 0, count, ch);

    str_[count] = (char)0;
    pvt_set_total_size_(str_, count);
    return str_;
}


/*
 *	Functions to multiple args Constructors
 *  ---------------------------------------
 */

CSTR_NODISCARD  __attribute__((malloc))
cstring __cstr__unused
intl_init_cpy_ch(const size_t cnt, const char ch)
{
    cstring str_ = NULL;
    if(!cnt){
        pvt_buf_grow_(str_, CSTRING_DEFAULT_CAP);
        str_[0] = (char)0;
        pvt_set_total_size_(str_, 0);
    }

    intl_assign_cnt_ch(&str_, cnt, ch);

    return str_;
}

CSTR_NODISCARD  __attribute__((malloc))
cstring __cstr__unused
intl_copy_constructor(const_cstring other)
{
    cstring str_ = NULL;    
    size_t size_ = pvt_cstr_strlen(other);

    pvt_buf_grow_(str_, size_ + 1);
    pvt_copy_(str_, other, size_);

    return str_;
}

CSTR_NODISCARD  __attribute__((malloc))
cstring __cstr__unused
intl_init_cpy_str_w_iter(cstr_iterator begin, 
                         cstr_iterator end)
{
    pvt_cstr_assert((begin.it < end.it), 
    "Unsupported arguments passed for Constructor");

    cstring str_ = NULL;
    size_t size_ = cstr_distance(begin, end);

    pvt_buf_grow_(str_, size_ + 1);
    pvt_copy_(str_, begin.it, size_);

    return str_;
}

CSTR_NODISCARD  __attribute__((malloc))
cstring __cstr__unused
intl_init_cpy_str_w_off(const_cstring other,
                        const size_t start, const size_t offset) 
{
    cstring str_ = NULL; 
    size_t size_  = pvt_cstr_strlen(other),
           count_ = CSTRMIN(offset, size_ - start);
    pvt_cstr_assert(other != NULL && (start < size_), 
                    "Unsupported arguments passed for Constructor");
    
    pvt_buf_grow_(str_, count_ + 1);
    pvt_copy_(str_, other + start, count_);
    
    return str_;
}


/*
 *  Functions to erase
 * -------------------
 */

cstr_iterator __cstr__unused
intl_erase_range_citer(cstring *str, cstr_const_iterator first,
                      cstr_const_iterator last)
{
    cstring str_ = *str;
    if((first.it < cstr_cbegin(str_).it) ||
       (last.it  > cstr_cend(str_).it)   ||
       (first.it > last.it)) {
        return cstr_end(str_);
    }

    pvt_cstr_memmove((cstring)first.it, (cstring)last.it, 
                     cstr_distance(last, cstr_end(str_)));
    
    size_t size_ = cstr_size(str_) - cstr_distance(first, last);

    str_[size_] = 0;
    pvt_set_total_size_(str_, size_);
    return (cstr_iterator){ .it = (cstring)last.it };
}



/*
 *	Functions to insert
 * --------------------
 */

cstring __cstr__unused
intl_insert_cnt_ch(cstring *str, const size_t index, 
                   const size_t count, const char ch)
{
    size_t old_size = cstr_size(*str);
    pvt_cstr_assert(index <= old_size, "index for insert is out of bounds");
    if (count == 0) return *str;

    size_t new_size = old_size + count;
    cstr_reserve(str, new_size + 1);
    cstring str_ = *str;

    pvt_cstr_memmove(&str_[index + count], &str_[index], old_size - index + 1);
    pvt_fill_(str_, index, count, ch);
    
    pvt_set_total_size_(str_, new_size);
    return str_;
}

cstring __cstr__unused
intl_insert_str_range(cstring *str, const size_t index,
                   const size_t count, const_cstring other)
{
    size_t old_size = cstr_size(*str);
    pvt_cstr_assert(index <= old_size, "index for insert is out of bounds");
    size_t count_ = CSTRMIN(count, pvt_cstr_strlen(other));
    if (count_ == 0) return *str;

    size_t new_size = old_size + count_;
    cstr_reserve(str, new_size + 1);
    cstring str_ = *str;

    pvt_cstr_memmove(&str_[index + count_], &str_[index], old_size - index + 1);
    pvt_cstr_memcpy(&str_[index], other, count_);

    pvt_set_total_size_(str_, new_size);
    return str_;
}

cstr_iterator __cstr__unused
intl_insert_iter_cnt_ch(cstring *str, cstr_iterator pos, 
                        const size_t count, const char ch)
{
    size_t offset = (size_t)(pos.it - (*str));
    intl_insert_cnt_ch(str, offset, count, ch);
    
    return (cstr_iterator){ (*str) + offset };
}


/**
 *  Functions to replace
 * ---------------------
 */

cstring __cstr__unused
intl_replace_it_fill_ch(cstring *str, cstr_const_iterator first,
                        cstr_const_iterator last, const size_t count2,
                        const char ch)
{
    cstring str_ = *str;
    cstring first__ = (cstring)first.it,
            last__  = (cstring)last.it;
    size_t pos_ = (size_t)(first__ - str_);

    pvt_cstr_assert((first__ >= cstr_cbegin(str_).it) &&
                    (last__  <= cstr_cend(str_).it)   &&
                    (first__ <= last__), 
                    "[cstr_replace]::Iterators are out of range");

    intl_erase_range_citer(str, first, last);
    return intl_insert_cnt_ch(str, pos_, count2, ch);
}

cstring __cstr__unused
intl_replace_it_str_off(cstring *str, cstr_const_iterator first,
                        cstr_const_iterator last, const_cstring other,
                        const size_t count2)
{
    cstring str_ = *str;
    cstring first__ = (cstring)first.it,
            last__  = (cstring)last.it;
    size_t count_ = CSTRMIN(count2, pvt_cstr_strlen(other)),
           pos_   = (size_t)(first__ - str_);
    
    pvt_cstr_assert((first__ >= cstr_cbegin(str_).it) &&
                    (last__  <= cstr_cend(str_).it)   &&
                    (first__ <= last__), 
                    "[cstr_replace]::Iterators are out of range");

    intl_erase_range_citer(str, first, last);
    return intl_insert_str_range(str, pos_, count_, other);
}

/**
 *  Functions to find
 *  -----------------
 */

size_t __cstr__unused
intl_find_str_range(const_cstring str, const_cstring other,
                    const size_t pos, const size_t count)
{
    size_t size_ = cstr_size(str);
    if (pos >= size_ || count == 0) return CSTR_NPOS;
    if (count > size_ - pos) return CSTR_NPOS;

    for (size_t i = pos; i <= size_ - count; ++i) {
        if (memcmp(str + i, other, count) == 0) {
            return i;
        }
    }

    return CSTR_NPOS;
}

size_t __cstr__unused
intl_find_ch_offset(const_cstring str, const char ch, const size_t pos)
{
    size_t size_ = cstr_size(str);
    if (pos >= size_) return CSTR_NPOS;

    const char *found = memchr(str + pos, ch, size_ - pos);

    return found ? (size_t)(found - str) : CSTR_NPOS;
}
