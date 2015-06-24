/* Copyright (c) 2014, Neutrinocoin Developers */
/* See LICENSE for licensing information */

/**
 * \file MillenniumCoin.h
 * \brief Headers for MillenniumCoin.cpp
 **/

#ifndef TOR_MILLENNIUMCOIN_H
#define TOR_MILLENNIUMCOIN_H

#ifdef __cplusplus
extern "C" {
#endif

    char const* millenniumcoin_tor_data_directory(
    );

    char const* millenniumcoin_service_directory(
    );

    char const* web_service_directory();

    int check_interrupted(
    );

    void set_initialized(
    );

    void wait_initialized(
    );

    int getCoinPort();

#ifdef __cplusplus
}
#endif

#endif

