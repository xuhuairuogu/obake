// Copyright 2019 Francesco Biscani (bluescarni@gmail.com)
//
// This file is part of the piranha library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <piranha/detail/to_string.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <random>
#include <string>
#include <tuple>
#include <type_traits>

#include <piranha/config.hpp>
#include <piranha/detail/limits.hpp>
#include <piranha/detail/tuple_for_each.hpp>
#include <piranha/type_traits.hpp>

using namespace piranha;

static std::mt19937 rng;

constexpr auto ntrials = 1000;

using int_types = std::tuple<char, signed char, unsigned char, short, unsigned short, int, unsigned, long,
                             unsigned long, long long, unsigned long long>;

TEST_CASE("to_string_test")
{
    // Small test for the generic overload.
    REQUIRE(std::to_string(1.2345) == detail::to_string(1.2345));

    // Test the integral types for which we supply our own implementation.
    detail::tuple_for_each(int_types{}, [](const auto &n) {
        using int_t = remove_cvref_t<decltype(n)>;

        REQUIRE(detail::to_string(int_t(0)) == "0");

        int_t min, max;
        std::tie(min, max) = detail::limits_minmax<int_t>;
        REQUIRE(detail::to_string(min) == std::to_string(min));
        REQUIRE(detail::to_string(max) == std::to_string(max));

        auto dist = [min, max]() {
            if constexpr (std::is_same_v<decltype(+n), int_t>) {
                return std::uniform_int_distribution<int_t>(min, max);
            } else {
                if constexpr (std::is_signed_v<int_t>) {
                    return std::uniform_int_distribution<int>(min, max);
                } else {
                    return std::uniform_int_distribution<unsigned>(min, max);
                }
            }
        }();

        for (auto i = 0; i < ntrials; ++i) {
            const auto tmp = static_cast<int_t>(dist(rng));
            REQUIRE(detail::to_string(tmp) == std::to_string(tmp));
            const auto tmp_2 = static_cast<int_t>(dist(rng, typename decltype(dist)::param_type(min / 2, max / 2)));
            REQUIRE(detail::to_string(tmp_2) == std::to_string(tmp_2));
            const auto tmp_2a = static_cast<int_t>(dist(rng, typename decltype(dist)::param_type(min / 64, max / 64)));
            REQUIRE(detail::to_string(tmp_2a) == std::to_string(tmp_2a));
            if constexpr (std::is_signed_v<int_t>) {
                const auto tmp_3 = static_cast<int_t>(dist(rng, typename decltype(dist)::param_type(-100, 100)));
                REQUIRE(detail::to_string(tmp_3) == std::to_string(tmp_3));
            } else {
                const auto tmp_3 = static_cast<int_t>(dist(rng, typename decltype(dist)::param_type(0, 200)));
                REQUIRE(detail::to_string(tmp_3) == std::to_string(tmp_3));
            }
        }
    });

#if defined(PIRANHA_HAVE_GCC_INT128)
    // Zeroes.
    REQUIRE(detail::to_string(__uint128_t(0)) == "0");
    REQUIRE(detail::to_string(__int128_t(0)) == "0");

    // Small values.
    REQUIRE(detail::to_string(__uint128_t(1)) == "1");
    REQUIRE(detail::to_string(__int128_t(1)) == "1");
    REQUIRE(detail::to_string(__int128_t(-1)) == "-1");
    REQUIRE(detail::to_string(__uint128_t(123)) == "123");
    REQUIRE(detail::to_string(__int128_t(123)) == "123");
    REQUIRE(detail::to_string(__int128_t(-123)) == "-123");

    // Larger values.
    REQUIRE(detail::to_string(__uint128_t(-1) / 100u) == "3402823669209384634633746074317682114");
    REQUIRE(detail::to_string(static_cast<__int128_t>(__uint128_t(-1) / 100u))
            == "3402823669209384634633746074317682114");
    REQUIRE(detail::to_string(-static_cast<__int128_t>(__uint128_t(-1) / 100u))
            == "-3402823669209384634633746074317682114");

    // Limit values.
    REQUIRE(detail::to_string(__uint128_t(-1)) == "340282366920938463463374607431768211455");
    constexpr auto max_int128_t = (((__int128_t(1) << 126) - 1) << 1) + 1;
    REQUIRE(detail::to_string(max_int128_t) == "170141183460469231731687303715884105727");
    REQUIRE(detail::to_string(-max_int128_t - 1) == "-170141183460469231731687303715884105728");
#endif
}
