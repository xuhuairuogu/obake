// Copyright 2019 Francesco Biscani (bluescarni@gmail.com)
//
// This file is part of the obake library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OBAKE_TEX_STREAM_INSERT_HPP
#define OBAKE_TEX_STREAM_INSERT_HPP

#include <ostream>
#include <utility>

#include <obake/config.hpp>
#include <obake/detail/not_implemented.hpp>
#include <obake/detail/priority_tag.hpp>
#include <obake/detail/ss_func_forward.hpp>
#include <obake/type_traits.hpp>

namespace obake
{

namespace customisation
{

// External customisation point for obake::tex_stream_insert().
template <typename T
#if !defined(OBAKE_HAVE_CONCEPTS)
          ,
          typename = void
#endif
          >
inline constexpr auto tex_stream_insert = not_implemented;

namespace internal
{

// Internal customisation point for obake::tex_stream_insert().
template <typename T
#if !defined(OBAKE_HAVE_CONCEPTS)
          ,
          typename = void
#endif
          >
inline constexpr auto tex_stream_insert = not_implemented;

} // namespace internal

} // namespace customisation

namespace detail
{

// Highest priority: explicit user override in the external customisation namespace.
template <typename T>
constexpr auto tex_stream_insert_impl(::std::ostream &os, T &&x, priority_tag<3>)
    OBAKE_SS_FORWARD_FUNCTION((customisation::tex_stream_insert<T &&>)(os, ::std::forward<T>(x)));

// Unqualified function call implementation.
template <typename T>
constexpr auto tex_stream_insert_impl(::std::ostream &os, T &&x, priority_tag<2>)
    OBAKE_SS_FORWARD_FUNCTION(tex_stream_insert(os, ::std::forward<T>(x)));

// Explicit override in the internal customisation namespace.
template <typename T>
constexpr auto tex_stream_insert_impl(::std::ostream &os, T &&x, priority_tag<1>)
    OBAKE_SS_FORWARD_FUNCTION((customisation::internal::tex_stream_insert<T &&>)(os, ::std::forward<T>(x)));

// Default implementation: just do a plain stream insertion.
template <typename T>
constexpr auto tex_stream_insert_impl(::std::ostream &os, T &&x, priority_tag<0>)
    OBAKE_SS_FORWARD_FUNCTION(void(os << ::std::forward<T>(x)));

} // namespace detail

#if defined(_MSC_VER)

struct tex_stream_insert_msvc {
    template <typename T>
    constexpr auto operator()(::std::ostream &os, T &&x) const
        OBAKE_SS_FORWARD_MEMBER_FUNCTION(void(detail::tex_stream_insert_impl(os, ::std::forward<T>(x),
                                                                             detail::priority_tag<3>{})))
};

inline constexpr auto tex_stream_insert = tex_stream_insert_msvc{};

#else

// NOTE: perhaps here, and in the other stream insertion functions,
// it would be more ergonomic to return a reference to os. Let's keep
// it in mind for the future.
inline constexpr auto tex_stream_insert = [](::std::ostream & os, auto &&x) OBAKE_SS_FORWARD_LAMBDA(
    void(detail::tex_stream_insert_impl(os, ::std::forward<decltype(x)>(x), detail::priority_tag<3>{})));

#endif

namespace detail
{

template <typename T>
using tex_stream_insert_t
    = decltype(::obake::tex_stream_insert(::std::declval<::std::ostream &>(), ::std::declval<T>()));

}

template <typename T>
using is_tex_stream_insertable = is_detected<detail::tex_stream_insert_t, T>;

template <typename T>
inline constexpr bool is_tex_stream_insertable_v = is_tex_stream_insertable<T>::value;

#if defined(OBAKE_HAVE_CONCEPTS)

template <typename T>
OBAKE_CONCEPT_DECL TexStreamInsertable = requires(::std::ostream &os, T &&x)
{
    ::obake::tex_stream_insert(os, ::std::forward<T>(x));
};

#endif

} // namespace obake

#endif
