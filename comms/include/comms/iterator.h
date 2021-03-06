//
// Copyright 2019 - 2020 (C). Alex Robenko. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

/// @file
/// Provides auxiliary functions for retrieving best type of iterator for read/write operations

#include "comms/details/ReadIterator.h"
#include "comms/details/WriteIterator.h"

namespace comms
{

/// @brief Create and initialise iterator for message read
/// @tparam TMessage Type of message interface class or smart pointer to it.
/// @param[in] iter Iterator value.
/// @return The same @b iter value, but casted to appropriate type
template <typename TMessage, typename TIter>
auto readIteratorFor(TIter&& iter) -> decltype(details::ReadIterator<TMessage>::get(std::forward<TIter>(iter)))
{
    return details::ReadIterator<TMessage>::get(std::forward<TIter>(iter));
}

/// @brief Create and initialise iterator for message read
/// @tparam TMessage Type of message interface class or smart pointer to it.
/// @param[in] msg Unused message object helps detecting proper message type.
/// @param[in] iter Iterator value.
/// @return The same @b iter value, but casted to appropriate type
template <typename TMessage, typename TIter>
auto readIteratorFor(TMessage&& msg, TIter&& iter) ->
    decltype(details::ReadIterator<typename std::decay<decltype(msg)>::type>::get(std::forward<TIter>(iter)))
{
    return details::ReadIterator<typename std::decay<decltype(msg)>::type>::get(std::forward<TIter>(iter));
}

/// @brief Create and initialise iterator for message write
/// @tparam TMessage Type of message interface class or smart pointer to it.
/// @param[in] iter Iterator value.
/// @return The same @b iter value, but casted to appropriate type
template <typename TMessage, typename TIter>
auto writeIteratorFor(TIter&& iter) -> decltype(details::WriteIterator<TMessage>::get(std::forward<TIter>(iter)))
{
    return details::WriteIterator<TMessage>::get(std::forward<TIter>(iter));
}

/// @brief Create and initialise iterator for message write
/// @tparam TMessage Type of message interface class or smart pointer to it.
/// @param[in] msg Unused message object helps detecting proper message type.
/// @param[in] iter Iterator value.
/// @return The same @b iter value, but casted to appropriate type
template <typename TMessage, typename TIter>
auto writeIteratorFor(TMessage&& msg, TIter&& iter) ->
    decltype(details::WriteIterator<typename std::decay<decltype(msg)>::type>::get(std::forward<TIter>(iter)))
{
    return details::WriteIterator<typename std::decay<decltype(msg)>::type>::get(std::forward<TIter>(iter));
}

} // namespace comms
