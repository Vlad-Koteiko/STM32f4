#pragma once
#include <cstddef>
#include <cstdint>

#ifndef BOOST_PP_VARIADICS
#    define BOOST_PP_VARIADICS
#endif
#include <boost/preprocessor.hpp>

template<typename... Ts>
struct Format
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n);
};

//////////////////////

template<std::size_t N>
constexpr bool checkValidFormats(const char (&fmt)[N], size_t n, char c)
{
  return fmt[n] == c ?
                     true
                     : checkValidFormats(fmt, n + 1, c);
}

template<class>
struct Type;

#define SUPPORTED_TYPE(T, Fmts) \
template<> \
struct Type<T> \
{ \
    template<std::size_t N> \
    constexpr static bool check(const char (&fmt)[N], std::size_t n) \
    { \
        return  checkValidFormats(Fmts, 0, fmt[n]); \
    } \
}

SUPPORTED_TYPE(char,              "c");
SUPPORTED_TYPE(int8_t,            "cd");
SUPPORTED_TYPE(uint8_t,           "cu");
SUPPORTED_TYPE(int16_t,           "d");
SUPPORTED_TYPE(uint16_t,          "u");
SUPPORTED_TYPE(int32_t,           "d");
SUPPORTED_TYPE(int,               "d");
SUPPORTED_TYPE(unsigned int,      "u");
SUPPORTED_TYPE(uint32_t,          "us");

// SUPPORTED_TYPE(char*,             "s");
// SUPPORTED_TYPE(unsigned char*,    "s");
// SUPPORTED_TYPE(const char*,       "s");
SUPPORTED_TYPE(double,            "f");
SUPPORTED_TYPE(float,             "f");

#define SUPPORTED_LL_TYPE(T, C) \
template<> \
struct Type<T> \
{ \
    template<std::size_t N> \
    static constexpr bool check(const char (&fmt)[N], std::size_t n) \
    { \
        return n < N && \
               n - 2 >= 0 && \
               fmt[n]     == C   && \
               fmt[n - 1] == 'l' && \
               fmt[n - 2] == 'l'; \
    } \
}

SUPPORTED_LL_TYPE(int64_t,  'd');
SUPPORTED_LL_TYPE(uint64_t, 'u');

template<typename... Ts>
struct Argument
{
  template<std::size_t N>
  static constexpr bool check(const char (&)[N], std::size_t)
  {
    return false;
  }
};

template<typename T, typename... Ts>
struct Argument<T, Ts...>
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    //    %[<flags>][<width>][.<precision>][<length>]<specifier>
    //        specifier := d|i|u|o|x|X|f|F|e|E|g|G|a|A|c|s|p|n
    return Type< typename std::decay<T>::type>::check(fmt, n) &&
           Format<Ts...>::check(fmt, n + 1);
  }
};

///////////////////////////

template<size_t N>
constexpr bool isDoubleLengthSpecifier(const char (&fmt)[N], std::size_t n)
{
  // hh | ll
  return n + 2 < N &&
         ((fmt[n] == 'h' && fmt[n + 1] == 'h') ||
          (fmt[n] == 'l' && fmt[n + 1] == 'l'));
}

template<size_t N>
constexpr bool isSingleLengthSpecifier(const char (&fmt)[N], std::size_t n)
{
  // h | l | j | z | t | L
  return n + 1 < N &&
         (fmt[n] == 'h' ||
          fmt[n] == 'l' ||
          fmt[n] == 'j' ||
          fmt[n] == 'z' ||
          fmt[n] == 't' ||
          fmt[n] == 'L');
}

template<size_t N>
constexpr size_t nextNonLengthSpecifier(const char (&fmt)[N], std::size_t n)
{
  return
      isDoubleLengthSpecifier(fmt, n) ? n + 2
      : isSingleLengthSpecifier(fmt, n) ? n + 1
                                        : n;
}

template<typename... Ts>
struct Length
{
  template<std::size_t N>
  static constexpr bool check(const char (&)[N], std::size_t)
  {
    return false;
  }
};

template<typename T, typename... Ts>
struct Length<T, Ts...>
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    //    %[<flags>][<width>][.<precision>][<length>]<specifier>
    //        length    := hh|h|l|ll|j|z|t|L
    return Argument<T, Ts...>::check(fmt, nextNonLengthSpecifier(fmt, n));
  }
};

///////////////////////////

template<std::size_t N>
constexpr size_t nextNonLiteralPrecision(const char (&fmt)[N], std::size_t n)
{
  return
      fmt[n] >= '0' && fmt[n] <= '9' ?
                                     nextNonLiteralPrecision(fmt, n + 1)
                                     : n;
}

template<typename... Ts>
struct Precision
{
  template<std::size_t N>
  static constexpr bool check(const char (&)[N], std::size_t)
  {
    return false;
  }
};

template<typename T, typename... Ts>
struct Precision<T, Ts...>
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    //    %[<flags>][<width>][.<precision>][<length>]<specifier>
    //        precision := <number>|'*'        // A number or a '*'

    // if precision is a provided argument, validate it is integral
    return n + 1 < N && fmt[n] == '.' && fmt[n + 1] == '*' ?
                                                           std::is_integral<T>::value && Length<Ts...>::check(fmt, n + 2)

                                                           // otherwise skip over any literal precision
                                                           : n + 1 < N && fmt[n] == '.' ?
                                                                                        Length<T, Ts...>::check(fmt, nextNonLiteralPrecision(fmt, n + 1))

                                                                                        : Length<T, Ts...>::check(fmt, n);
  }
};

///////////////////////////

template<std::size_t N>
constexpr size_t nextNonLiteralWidth(const char (&fmt)[N], std::size_t n)
{
  return
      fmt[n] >= '0' && fmt[n] <= '9' ?
                                     nextNonLiteralWidth(fmt, n + 1)
                                     : n;
}

template<typename... Ts>
struct Width
{
  template<std::size_t N>
  static constexpr bool check(const char (&)[N], std::size_t)
  {
    return false;
  }
};

template<typename T, typename... Ts>
struct Width<T, Ts...>
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    //    %[<flags>][<width>][.<precision>][<length>]<specifier>
    //        width     := <number>|'*'        // A number or a '*'

    // if width is a provided argument, validate it is integral
    return fmt[n] == '*' ?
                         std::is_integral<T>::value && Precision<Ts...>::check(fmt, n + 1)

                         // otherwise skip over any literal width
                         : Precision<T, Ts...>::check(fmt, nextNonLiteralWidth(fmt, n));
  }
};

///////////////////////////

template<size_t N>
constexpr bool isFlag(const char (&fmt)[N], std::size_t n)
{
  return n + 1 < N &&
         (fmt[n] == '-' ||
          fmt[n] == '+' ||
          fmt[n] == ' ' ||
          fmt[n] == '#' ||
          fmt[n] == '0');
}

template<std::size_t N>
constexpr size_t nextNonFlag(const char (&fmt)[N], std::size_t n)
{
  return
      isFlag(fmt, n) ?
                     nextNonFlag(fmt, n + 1)
                     : n;
}

template<typename T, typename... Ts>
struct Flags
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    //    %[<flags>][<width>][.<precision>][<length>]<specifier>
    //        flags     := [-+ #0]*            // Zero or more
    return Width<T, Ts...>::check(fmt, nextNonFlag(fmt, n));
  }
};

///////////////////////////

template<size_t N>
constexpr bool isLiteralPercent(const char (&fmt)[N], std::size_t n)
{
  return n + 1 <= N && fmt[n] == '%' && fmt[n + 1] == '%';
}

template<typename T, typename... Ts>
struct Format<T, Ts...>
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    return

        // skip non-format specifiers (ie: not a % character)
        fmt[n] != '%' ?
                      Format<T, Ts...>::check(fmt, n + 1)

                      // %%
                      : isLiteralPercent(fmt, n) ?
                                                 Format<T, Ts...>::check(fmt, n + 2)

                                                 // we've found a format specifier
                                                 : Flags<T, Ts...>::check(fmt, n + 1);
  }
};

template<>
struct Format<>
{
  template<std::size_t N>
  static constexpr bool check(const char (&fmt)[N], std::size_t n)
  {
    return
        n>= N ?
               true
               : fmt[n] != '%' ?
                               check(fmt, n + 1)
                               : fmt[n + 1] == '%' ?
                                                   check(fmt, n + 2)
                                                   : false;
  }
};

////////////////

// printing...


////////////////

#define PP_PARENTHESISE_WITH_TOKEN(r, token, i, e) \
    BOOST_PP_COMMA_IF(i) token(e)

#define PP_CSV_SEQ_PARENTHESISE_WITH_TOKEN(...) \
    BOOST_PP_SEQ_FOR_EACH_I(PP_PARENTHESISE_WITH_TOKEN, decltype, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define PP_PERFORM_LOG_FORMAT_CHECK(fmt, ...) \
    Format<BOOST_PP_IF(BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE((,##__VA_ARGS__)), 1), \
            BOOST_PP_EXPAND, PP_CSV_SEQ_PARENTHESISE_WITH_TOKEN)(__VA_ARGS__)>::check(fmt, 0)

