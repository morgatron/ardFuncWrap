//From https://www.preney.ca/paul/archives/1099
#include <cstddef>
#include <type_traits>
#include <tuple>

//indices<0,1,2,3,4>
//START INDICES STUFF
template <std::size_t...>
struct indices { };

template <
  std::size_t Begin,
  std::size_t End,
  typename Indices = indices<>,
  typename Enable = void
>
struct make_seq_indices
{
  static_assert(Begin <= End, "Begin must be <= End");
};


template <
  std::size_t Begin,
  std::size_t End,
  template <std::size_t...> class I, 
  std::size_t... Indices
>
struct make_seq_indices<
  Begin, End,
  I<Indices...>,
  typename std::enable_if<Begin < End, void>::type
>
{
  using type = 
    typename make_seq_indices<
      Begin+1, End,
      I<Indices..., Begin>
    >::type
  ;
};

template <
  std::size_t Begin,
  std::size_t End,
  typename Indices
>
struct make_seq_indices<
  Begin, End,
  Indices,
  typename std::enable_if<Begin == End, void>::type
>
{
  using type = Indices;
};


template <std::size_t Begin, std::size_t End>
using make_seq_indices_T =
  typename make_seq_indices<Begin, End>::type
;


// END INDICES BITS======
//
// APPLY----------------------
template <typename Op, typename... Args>
inline constexpr auto apply(Op&& op, Args&&... args) ->
  decltype(std::forward<Op>(op)(std::forward<Args>(args)...))
{
  return std::forward<Op>(op)(std::forward<Args>(args)...);
}


// This function overload applies op to all tuple indices...
template <
  typename Op, 
  typename Tuple,
  template <std::size_t...> class I,
  std::size_t... Indices
>
inline constexpr auto apply_tuple(
  Op&& op, 
  Tuple&& t, 
  I<Indices...>&&
) -> 
  decltype(
    std::forward<Op>(op)(
      std::get<Indices>(std::forward<Tuple>(t))...
    )
  )
{
  return 
    std::forward<Op>(op)(
      std::get<Indices>(std::forward<Tuple>(t))...
    )
  ;
}


// This function overload forwards op and t along with the
// indices of the tuple generated by make_seq_indices...
template <
  typename Op, 
  typename Tuple,
  typename Indices = 
    make_seq_indices_T<
      0, 
      std::tuple_size<typename std::decay<Tuple>::type>::value
    >
>
inline constexpr auto apply_tuple(Op&& op, Tuple&& t) -> 
  decltype(
    apply_tuple(
      std::forward<Op>(op), 
      std::forward<Tuple>(t),
      Indices{}
    )
  )
{
  return
    apply_tuple(
      std::forward<Op>(op), 
      std::forward<Tuple>(t),
      Indices{}
    )
  ;
}

