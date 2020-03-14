#ifndef UTI_MONADPIPE_HPP
#define UTI_MONADPIPE_HPP

#include <tuple>

template<typename Function, typename Tuple, size_t ... I>
auto call(Function f, Tuple t, std::index_sequence<I ...>)
{
     return f(std::get<I>(t).value() ...);
}

template<typename Function, typename Tuple>
auto call(Function f, Tuple t)
{
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return call(f, t, std::make_index_sequence<size>{});
}

template<typename Tuple, size_t ... I>
bool all_true(Tuple t, std::index_sequence<I ...>)
{
     return ((std::get<I>(t)) && ...);
}

template<typename Tuple, size_t index>
auto find_failing(Tuple t) {
	if constexpr(index == 0) {
		return !std::get<0>(t) ? std::get<0>(t).error() : std::errc::argument_out_of_domain;
	}
	else {
		return !std::get<index>(t) ? std::get<index>(t).error() : find_failing<Tuple, index - 1>(t);
	}
}


template<typename Tuple>
auto all_true(Tuple t)
{
    static constexpr auto size = std::tuple_size<Tuple>::value;
    return all_true(t, std::make_index_sequence<size>{});
}

template<typename Tuple>
auto find_failing(Tuple t) {
	static constexpr auto size = std::tuple_size<Tuple>::value;
	return find_failing<Tuple, size - 1>(t);
}
template<typename T, typename TtoOptionalU>
auto operator|(Expected<T> const& t, TtoOptionalU f) -> decltype(f(t.value()))
{
    if(t)
    {
        return f(t.value());
    }
    else
    {
        return t.error();
    }
}

template<typename ...T, typename TtoOptionalU>
auto operator|(std::tuple<T...> const& t, TtoOptionalU f) -> decltype(call(f,t))
{
    if(all_true(t))
    {
        return call(f, t);
    }
    else
    {
        return find_failing<std::tuple<T...>>(t);
    }
}

template<typename T, typename S>
auto operator,(Expected<T> const& t, Expected<S> const& s)
{
    return std::make_tuple(t, s);
}

template<typename ...T, typename S>
auto operator,(std::tuple<T...> const& t, Expected<S> const& s)
{
    return std::tuple_cat(t, std::tie(s));
}

#endif // UTI_MONADPIPE_HPP

