#pragma once



#include <cstdint>
#include <tuple>


constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
  std::uint32_t hash = 2166136261u;
  for (std::size_t i = 0; i < count; i++)
  {
    hash = hash ^ s[i];
    hash = hash * 16777619u;
  }
  return hash;
}

constexpr std::uint32_t operator"" _hash(char const* s, std::size_t count)
{
  return fnv1a_32(s, count);
}

#include <algorithm>



template<class T>
struct type_id
{
  constexpr static std::uint8_t value = sizeof(T) - std::is_signed_v<T>;
};


template<>
struct type_id<float>
{
  constexpr static std::uint8_t value = 9;
};

template<>
struct type_id<double>
{
  constexpr static std::uint8_t value = 10;
};

template<class T>
constexpr std::uint8_t type_id_v = type_id<T>::value;

#pragma pack(1)
template<class T>
struct Holder
{
  std::uint8_t holder = type_id_v<T>;
  T value;

  constexpr explicit Holder(T value) : value(value) {}
};


#pragma pack()

#include <array>
#include <new>




template <std::size_t, typename, std::size_t = 4u>
struct get_offset;

template <std::size_t N, typename A0, typename ... As, std::size_t Off>
struct get_offset<N, std::tuple<A0, As...>, Off>
    : public get_offset<N-1u, std::tuple<As...>, Off+sizeof(A0)>
{ };

template <typename A0, typename ... As, std::size_t Off>
struct get_offset<0u, std::tuple<A0, As...>, Off>
    : public std::integral_constant<std::size_t, Off>
{ };


template<class Writer>
class LoggerImplementation
{

public:

  template<class ...Args>
  static constexpr std::size_t Size = (sizeof(Holder<Args>) + ... + 0 );


  template<class T, class ...Args,  std::size_t... I>
  constexpr void Log_impl( std::uint32_t hash, const std::tuple<Args...>& args,  std::integer_sequence<T, I...> int_seq )
  {
    std::array<std::uint8_t, 4 +  Size<Args...>> buffer;
    (
        (new(buffer.data()) std::uint32_t(hash)),
        ...,
        new(buffer.data() + get_offset<I, std::tuple<Holder<Args>...>>::value) Holder<Args>(std::get<I>(args))
    );
    Writer::Write(buffer.data(), buffer.size());
  }

  template<class ...Args>
  constexpr void Log( std::uint32_t hash, Args... args)
  {
    Log_impl(hash, std::make_tuple(args...), std::index_sequence_for<Args...>{});
  }

  static LoggerImplementation& GetInstance()
  {
    static LoggerImplementation logger;
    return logger;
  }
};

#include "format_check.hpp"

