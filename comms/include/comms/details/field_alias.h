//
// Copyright 2019 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include "macro_common.h"

#define COMMS_ALIAS_MEM_ACC(prefix_, m_) COMMS_CONCATENATE(prefix_, m_)()

#define COMMS_ALIAS_MEM_ACC_1(prefix_, m_) COMMS_EXPAND(COMMS_ALIAS_MEM_ACC(prefix_, m_))
#define COMMS_ALIAS_MEM_ACC_2(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_1(field_, __VA_ARGS__)) // using field_ as next prefix on purpose
#define COMMS_ALIAS_MEM_ACC_3(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_2(field_, __VA_ARGS__)) // using field_ as next prefix on purpose
#define COMMS_ALIAS_MEM_ACC_4(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_3(field_, __VA_ARGS__)) // using field_ as next prefix on purpose
#define COMMS_ALIAS_MEM_ACC_5(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_4(field_, __VA_ARGS__)) // using field_ as next prefix on purpose
#define COMMS_ALIAS_MEM_ACC_6(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_5(field_, __VA_ARGS__)) // using field_ as next prefix on purpose
#define COMMS_ALIAS_MEM_ACC_7(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_6(field_, __VA_ARGS__)) // using field_ as next prefix on purpose
#define COMMS_ALIAS_MEM_ACC_8(prefix_, m_, ...) \
    COMMS_ALIAS_MEM_ACC(prefix_, m_) . COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_7(field_, __VA_ARGS__)) // using field_ as next prefix on purpose

#define COMMS_ALIAS_ALL_MEM_ACC_(N, prefix_, ...) COMMS_EXPAND(COMMS_ALIAS_MEM_ACC_ ## N(prefix_, __VA_ARGS__))
#define COMMS_ALIAS_ALL_MEM_ACC(N, prefix_, ...) COMMS_EXPAND(COMMS_ALIAS_ALL_MEM_ACC_(N, prefix_, __VA_ARGS__))
#define COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, ...) \
    COMMS_EXPAND(COMMS_ALIAS_ALL_MEM_ACC(COMMS_NUM_ARGS(__VA_ARGS__), prefix_, __VA_ARGS__))


#if __cplusplus < 201402L

#ifndef COMMS_DO_ALIAS_RET_TYPE_PREFIX
#define COMMS_DO_ALIAS_RET_TYPE_PREFIX
#endif // #ifndef COMMS_DO_ALIAS_RET_TYPE_PREFIX

#if defined(__GNUC__) && !defined(__clang__)
#if __GNUC__ < 5
#undef COMMS_DO_ALIAS_RET_TYPE_PREFIX
#define COMMS_DO_ALIAS_RET_TYPE_PREFIX this->
#endif // #if __GNUC__ < 5
#endif // #if defined(__GNUC__) && !defined(__clang__)

#define COMMS_DO_ALIAS(prefix_, f1_, ...) \
    auto COMMS_CONCATENATE(prefix_, f1_) () -> decltype(COMMS_DO_ALIAS_RET_TYPE_PREFIX COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__)) \
    { \
        return COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__); \
    }\
    auto COMMS_CONCATENATE(prefix_, f1_) () const -> decltype(COMMS_DO_ALIAS_RET_TYPE_PREFIX COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__)) \
    { \
        return COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__); \
    }

#define COMMS_DO_ALIAS_NOTEMPLATE(prefix_, f1_, ...) \
    auto COMMS_CONCATENATE(prefix_, f1_) () -> decltype(COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__)) \
    { \
        return COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__); \
    }\
    auto COMMS_CONCATENATE(prefix_, f1_) () const -> decltype(COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__)) \
    { \
        return COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__); \
    }

#else // #if __cplusplus < 201402L

#define COMMS_DO_ALIAS(prefix_, f1_, ...) \
    decltype(auto) COMMS_CONCATENATE(prefix_, f1_) () \
    { \
        return COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__); \
    }\
    decltype(auto) COMMS_CONCATENATE(prefix_, f1_) () const \
    { \
        return COMMS_DO_ALIAS_ALL_MEM_ACC(prefix_, __VA_ARGS__); \
    }

#define COMMS_DO_ALIAS_NOTEMPLATE(prefix_, f1_, ...) COMMS_EXPAND(COMMS_DO_ALIAS(prefix_, f1_, __VA_ARGS__))

#endif

