
#include "cstring.h"


/*
 *      Assign
 */

cstring __unused
intl_assign_str_range(cstring *str, const_cstring other,
                      const size_t pos, const size_t count)
{
    cstring str_ = *str;
    size_t size_ = pvt_cstr_strlen(other),
           count_ = CSTRMIN(count, size_ - pos);
    if(pos >= size_) return str_;

    cstr_reserve(str, count_);
    pvt_cstr_memmove(str_, other + pos, count_);
    str_[count_] = (char)0;
    pvt_set_total_size_(str_, count_);
    return str_;
}

cstring __unused
intl_assign_cnt_ch(cstring *str, const size_t count, const char ch)
{
    cstring str_ = *str;
    
    cstr_reserve(str, count);
    pvt_fill_(str_, 0, count, ch);
    str_[count] = (char)0;
    pvt_set_total_size_(str_, count);
    return str_;
}


/*
 *	functions to multiple args Constructors
 ******************************************
 */

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
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

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_copy_constructor(const_cstring other)
{
    cstring str_ = NULL;    
    size_t size_ = pvt_cstr_strlen(other);

    pvt_buf_grow_(str_, size_ + 1);
    pvt_copy_(str_, other, size_);
    return str_;
}

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_init_cpy_str_w_iter(cstr_iterator begin, 
                         cstr_iterator end)
{
    pvt_cstr_assert((begin.it < end.it), 
    "Unsupported arguments passed for Constructor");

    cstring str_ = NULL;
    size_t size_ = cstr_distance(begin, end);

    pvt_buf_grow_(str_, size_);
    pvt_copy_(str_, begin.it, size_);
    return str_;
}

CSTR_NODISCARD
__attribute__((malloc))
cstring __unused
intl_init_cpy_str_w_off(const_cstring other,
                        const size_t start, const size_t offset) 
{
    cstring str_ = NULL; 
    size_t size_  = pvt_cstr_strlen(other) + 1,
           count_ = CSTRMIN(offset, size_ - start);
    pvt_cstr_assert(other != NULL && (start < size_), 
                    "Unsupported arguments passed for Constructor");
    
    pvt_buf_grow_(str_, count_);
    pvt_copy_(str_, other + start, count_);
    return str_;
}


/*********************************
 *	---- Modifiers ----
 */

/*
 *  functions to erase
 *********************
 */

cstr_iterator __unused
intl_erase_range_iter(cstring *str, cstr_iterator first,
                      cstr_iterator last)
{
    cstring str_ = *str;
    if((first.it < cstr_begin(str_).it) ||
       (last.it  > cstr_end(str_).it)   ||
       (first.it > last.it)) {
        return cstr_end(str_);
    }

    pvt_cstr_memmove(first.it, last.it, 
                     cstr_distance(last, cstr_end(str_)));
    
    size_t size_ = cstr_size(str_) - cstr_distance(first, last);
    str_[size_] = 0;
    pvt_set_total_size_(str_, size_);
    return last;
}



/*
 *	functions to insert with multiple args
 *****************************************
 */

cstring __unused
intl_insert_cnt_ch(cstring *str, const size_t index, 
                   const size_t count, const char ch)
{
    cstring str_ = *str;
    size_t size_ = cstr_size(str_) + count,
           count_ = count + index;
    pvt_cstr_assert(count_ <= size_, 
        "index for insert is out of bounds");
    
    cstr_reserve(str, size_);
    pvt_cstr_memmove(&str_[count_], &str_[index], size_ - index);
    pvt_fill_(str_, index, count, ch);
    
    pvt_set_total_size_(str_, size_);
    str_[size_] = (char)0;
    return str_;
}

cstring __unused
intl_insert_cnt_str(cstring *str, const size_t index, 
                   const size_t count, const_cstring other)
{
    cstring str_ = *str;
    size_t size_ = cstr_size(str_),
           other_size_ = pvt_cstr_strlen(other),
           count_      = count * other_size_;
    pvt_cstr_assert(index <= size_,
        "index for insert is out of bounds");
    
    cstr_reserve(str, size_ + count_);
    pvt_cstr_memmove(&str_[count_], &str_[index], size_ - index);
    
    for(size_t i = 0; i < count; ++i)
        pvt_cstr_memmove(&str_[index + (i * other_size_)], 
                         other, other_size_);
    
    pvt_set_total_size_(str_, size_ + count_);
    str_[size_ + count_] = (char)0;
    return str_;
}
