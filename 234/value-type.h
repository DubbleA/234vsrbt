#ifndef __value_type_
#define __value_type_
#include <utility>
#include <memory>

/*
 These type aliases are from the LLVM C++ standard library implemention <map> header file: 
 
    //  __value_type alias
    using  __value_type    = std::__value_type<key_type, mapped_type>;                   

    // Its subsequent uses in the <map> header:
    using  __vc            = __map_value_compare<key_type, __value_type, key_compare>;  

    using __allocator_type = typename __rebind_alloc_helper<allocator_traits<allocator_type>, __value_type>::type; 

    // Its really critical use is, defining the map private member __base

    using __base           = __tree<__value_type, __vc, __allocator_type>;    
    // ... because __base is the type of the actual __tree type of the member variable used in map, __tree_
    __base __tree_;                                                           
*/


/*
 * Code from the libc++ <map> header
 */ 
template <class _Tp>
struct __uncvref  {
    using type = typename std::remove_cv<typename std::remove_reference<_Tp>::type>::type;
};

template <class _Tp, class _Up>
struct __is_same_uncvref : std::is_same<typename __uncvref<_Tp>::type,
                                        typename __uncvref<_Up>::type> {};

template <class _Key, class _Value> struct __value_type {

  public:   

     using    key_type           = _Key;
     using    mapped_type        = _Value;
     using    value_type         = std::pair<const key_type, mapped_type>;
    
     using    __nc_ref_pair_type  = std::pair<key_type&, mapped_type&>;
     using    __nc_rref_pair_type = std::pair<key_type&&, mapped_type&&>;
    
  private:
      value_type __cc; // std::pair<const key_type, mapped_type>

  public:

   /*
    * Returns: std::pair<const key_type, mapped_type>  
    */
    value_type& __get_value()
    {
        return __cc;
    }

   /*
    * Returns: const std::pair<const key_type, mapped_type>  
    */
    const value_type& __get_value() const
    {
        return __cc;
    }

   /*
    * Returns: std::pair<key_type&, mapped_type&> 
    */
    __nc_ref_pair_type __ref()   
    {
        value_type& __v = __get_value();
        return __nc_ref_pair_type(const_cast<key_type&>(__v.first), __v.second);
    }

   /*
    * Returns: std::pair<key_type&&, mapped_type&&>
    */
    __nc_rref_pair_type __move()          
    {
        value_type& __v = __get_value();

        return __nc_rref_pair_type(      // return a pair of rvalue references, after casting away const for 'first' 
                                    std::move(const_cast<key_type&>(__v.first)),
                                    std::move(__v.second)
                                  );
    }

    /* 
     * The next two methods below are the methods that use __ref() and __move()
     * which are defined above.
     */
    __value_type& operator=(const __value_type& __v)  // Assignment operator
    {
        __ref() = __v.__get_value();
        return *this;
    }

    __value_type& operator=(__value_type&& __v)    // Move assignment operator
    {
        __ref() = __v.__move();
        return *this;
    }

    
    /*
      The template assigment operator below is called for both lvalues or rvalues of type 'pair<const key_type, mapped_type>' or const/volatile or reference to these types.  

      To see this is the case, observe that

          __is_same_uncvref<typename T1, typename T2> 

      returns true if T1 and T2 are the same type after removing 'const', 'volatile' and any reference from them. Thus, __is_same_uncvref<_ValueTp, value_type> returns true if _ValueTp
      is of the same type as std::pair<const key_type, mapped_type>, after removing const/volatile/reference. And thus, __is_same_uncvref<_ValueTp, value_type> returns true if _ValueTp
      is of same type as __value_type::value_type, which is 'pair<const key_type, mapped_type>'. 
      
      Thus, the template assignment operator 

         template<_ValueTp> __value_type& operator=(_ValueTp&& __v)

      is called whenever an lvaule reference or rvalue reference of type pair<const key_type, mapped_type> is assigned to a __value_type.  

      It forwards the assigment to 'template<typename T1, typename T2> std::pair<Key, Value>::operator=(std::pair<T1, T2>&& lhs);

      It invokes the template move-assigment operator of std::pair.
    */
    template <class _ValueTp,                            
              class = typename std::enable_if<        
                    __is_same_uncvref<_ValueTp, value_type>::value  // where value_type = std::pair<const key_type, mapped_type> 
                 >::type
             >
    __value_type& operator=(_ValueTp&& __v)
    {
        __ref() = std::forward<_ValueTp>(__v); 
        return *this;
    }

    template<typename T1, typename T2> __value_type(T1&& first, T2&& second) : __cc(std::forward<T1>(first), std::forward<T2>(second))
    {
    } 

    explicit __value_type(const std::pair<_Key, _Value>& pr) : __cc(pr.first, pr.second)
    {
    } 

    explicit __value_type(std::pair<_Key, _Value>&& pr) : __cc(pr)
    {
    }

    __value_type()                     = default;
   ~__value_type()                     = default;

    __value_type(const __value_type&)  = default;
    __value_type(__value_type&&)       = default;

};
#endif