#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "type_traits.h"
#include "construct.h"
#include <cstring> //memmove
#include "iterator.h"
#include <algorithm> //【之后改用自己的】

/*TODO【缺少异常处理  之后补上  commit or rollback】*/

namespace miniSTL{

//POD 
template<class ForwardIterator, class Size, class T>
inline ForwardIterator
_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _true_type) {
    return std::fill_n(first, n, x); //TODO【  改用 miniSTL::fill_n】
}
template<class ForwardIterator, class Size, class T>
inline ForwardIterator
_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _false_type) {
    ForwardIterator cur = first;
    for( ; n > 0; --n, ++cur)
        construct(&*cur, x);
    return cur;
}

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x,T1*){
    using is_POD = typename _type_traits<T1>::is_POD_type;  
    return _uninitialized_fill_n_aux(first, n ,x, is_POD());
}

template <class ForwardIterator, class Size, class T> 
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
    return _uninitialized_fill_n(first, n, x, value_type(first));  //【value_type 返回的是个指针 为啥设计成指针？】
}


template <class InputIterator, class ForwardIterator>
inline ForwardIterator
_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type){
    return std::copy(first, last, result);  //TODO  【改用 miniSTL::copy  (对两种类型的iterator所指进行拷贝 怎么实现）】
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator
_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type){
    ForwardIterator cur = result;
    for( ; first != last; ++first, ++ cur)
        construct(&*cur, *first);
    return cur;
}

template<class InputIterator , class ForwardIterator, class T>
inline ForwardIterator 
_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
    using is_POD = typename _type_traits<T>::is_POD_type;
    return _uninitialized_copy_aux(first, last, result, is_POD());
}
template<class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
    return _uninitialized_copy(first, last, result, value_type(result));//
}

inline char* uninitialized_copy(const char* first, const char* last,
                                char* result) {
  memmove(result, first, last - first); //【memmove 】
  return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last,
                                   wchar_t* result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}



//POD 
template<class ForwardIterator, class T>
inline void
_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _true_type) {
    std::fill(first, last, x); //TODO【  改用 miniSTL::fill】
}
template<class ForwardIterator , class T>
inline void
_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _false_type) {
    ForwardIterator cur = first;
    for ( ; cur != last; ++cur)
      construct(&*cur, x);
}

template <class ForwardIterator, class T, class T1>
inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,T1*){
    using is_POD = typename _type_traits<T1>::is_POD_type;  
    _uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator, class T> 
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x){
    _uninitialized_fill(first, last, x, value_type(first));  
}



// Copies [first1, last1) into [result, result + (last1 - first1)), and
//  copies [first2, last2) into
//  [result, result + (last1 - first1) + (last2 - first2)).

template <class InputIterator1, class InputIterator2, class ForwardIterator>
inline ForwardIterator
_uninitialized_copy_copy(InputIterator1 first1, InputIterator1 last1,
                          InputIterator2 first2, InputIterator2 last2,
                          ForwardIterator result) {
  ForwardIterator mid = uninitialized_copy(first1, last1, result);
  
    return uninitialized_copy(first2, last2, mid);
  
}

// Fills [result, mid) with x, and copies [first, last) into
//  [mid, mid + (last - first)).
template <class ForwardIterator, class T, class InputIterator>
inline ForwardIterator 
_uninitialized_fill_copy(ForwardIterator result, ForwardIterator mid,
                          const T& x,
                          InputIterator first, InputIterator last) {
  uninitialized_fill(result, mid, x);
  
    return uninitialized_copy(first, last, mid);
  
}

// Copies [first1, last1) into [first2, first2 + (last1 - first1)), and
//  fills [first2 + (last1 - first1), last2) with x.
template <class InputIterator, class ForwardIterator, class T>
inline void
_uninitialized_copy_fill(InputIterator first1, InputIterator last1,
                          ForwardIterator first2, ForwardIterator last2,
                          const T& x) {
  ForwardIterator mid2 = uninitialized_copy(first1, last1, first2);
  
    uninitialized_fill(mid2, last2, x);
  
  
}



}



#endif