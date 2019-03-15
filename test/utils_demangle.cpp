// Copyright 2019 Francesco Biscani (bluescarni@gmail.com)
//
// This file is part of the piranha library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <piranha/utils/demangle.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <mp++/integer.hpp>
#include <mp++/rational.hpp>

#include <piranha/config.hpp>

using namespace piranha;

TEST_CASE("demangle")
{

    std::cout << demangle<std::string>() << '\n';
    std::cout << demangle<int>() << '\n';
    std::cout << demangle<const int>() << '\n';
    std::cout << demangle<const volatile int>() << '\n';
    std::cout << demangle<volatile int>() << '\n';
    std::cout << demangle<long double>() << '\n';
    std::cout << demangle<std::vector<std::vector<float>>>() << '\n';
    std::cout << demangle<mppp::integer<1>>() << '\n';
    std::cout << demangle<mppp::rational<2>>() << '\n';
    std::cout << demangle<mppp::rational<2> &>() << '\n';
    std::cout << demangle<mppp::rational<2> const>() << '\n';
    std::cout << demangle<mppp::rational<2> const &>() << '\n';
    std::cout << demangle<mppp::rational<2> *>() << '\n';
    std::cout << demangle<const mppp::rational<2> *>() << '\n';
    std::cout << demangle<const mppp::rational<2> *const>() << '\n';
    std::cout << demangle<const mppp::rational<2> *const &>() << '\n';
    std::cout << demangle<void>() << '\n';
    std::cout << demangle<void const>() << '\n';
    std::cout << demangle<void volatile>() << '\n';
    std::cout << demangle<void volatile const>() << '\n';
#if defined(PIRANHA_HAVE_GCC_INT128)
    std::cout << demangle<__int128_t>() << '\n';
    std::cout << demangle<__int128_t *>() << '\n';
    std::cout << demangle<__int128_t const *>() << '\n';
    std::cout << demangle<__int128_t const *const>() << '\n';
    std::cout << demangle<__uint128_t>() << '\n';
    std::cout << demangle<__uint128_t *>() << '\n';
    std::cout << demangle<__uint128_t const *>() << '\n';
    std::cout << demangle<__uint128_t const *const>() << '\n';
    std::cout << demangle<const __int128_t>() << '\n';
    std::cout << demangle<const __uint128_t>() << '\n';
    std::cout << demangle<__int128_t &>() << '\n';
    std::cout << demangle<__uint128_t &>() << '\n';
    std::cout << demangle<__int128_t &&>() << '\n';
    std::cout << demangle<__uint128_t &&>() << '\n';
    std::cout << demangle<const __int128_t &>() << '\n';
    std::cout << demangle<const __uint128_t &>() << '\n';
    std::cout << demangle<std::vector<__int128_t>>() << '\n';
#endif

    // Couple of multithreaded tests.
    auto t_func = []() -> std::string {
        std::string tmp;
        for (auto i = 0; i < 100; ++i) {
            tmp += demangle<std::vector<std::vector<float>>>();
        }
        return tmp;
    };

    std::thread t1(t_func);
    std::thread t2(t_func);
    std::thread t3(t_func);
    std::thread t4(t_func);
    std::thread t5(t_func);
    std::thread t6(t_func);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}
