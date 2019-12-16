//
// Copyright 2016 - 2018 (C). Alex Robenko. All rights reserved.
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

/// @file
/// @brief Contains definition of FloatValues message and its fields.

#pragma once

#include "comms/fields.h"
#include "comms/MessageBase.h"
#include "demo/MsgId.h"
#include "demo/FieldBase.h"
#include "demo/DefaultOptions.h"

namespace demo
{

namespace message
{

/// @brief Accumulates details of all the FloatValues message fields.
/// @tparam TOpt Extra options
/// @see FloatValues
template <typename TOpt = demo::DefaultOptions>
struct FloatValuesFields
{
    /// @brief Simple 4 byte IEEE 754 floating point value.
    using field1 =
        comms::field::FloatValue<
            demo::FieldBase,
            float,
            typename TOpt::message::FloatValuesFields::field1
    >;

    /// @brief Simple 8 byte IEEE 754 floating point value.
    using field2 =
        comms::field::FloatValue<
            demo::FieldBase,
            double,
            typename TOpt::message::FloatValuesFields::field2
    >;

    /// @brief Floating point value serialised as integer with (1e-2) scaling ratio.
    using field3 =
        comms::field::IntValue<
            demo::FieldBase,
            std::uint8_t,
            typename TOpt::message::FloatValuesFields::field3,
            comms::option::ScalingRatio<1, 100>
        >;

    /// @brief Floating point value serialised as 5 byte integer with (1e-11) scaling ratio.
    using field4 =
        comms::field::IntValue<
            demo::FieldBase,
            std::int64_t,
            typename TOpt::message::FloatValuesFields::field4,
            comms::option::ScalingRatio<1, 100000000000>,
            comms::option::FixedLength<5>
        >;

    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1,
        field2,
        field3,
        field4
    >;

};

/// @brief Accumulates various floating point values.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref FloatValuesFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class FloatValues : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::FloatValues,
        comms::option::StaticNumIdImpl<MsgId_FloatValues>,
        comms::option::FieldsImpl<typename FloatValuesFields<TOpt>::All>,
        comms::option::MsgType<FloatValues<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::FloatValues,
            comms::option::StaticNumIdImpl<MsgId_FloatValues>,
            comms::option::FieldsImpl<typename FloatValuesFields<TOpt>::All>,
            comms::option::MsgType<FloatValues<TMsgBase, TOpt> >,
            comms::option::HasName
        >;

    static const bool AreFieldsVersionDependent = Base::areFieldsVersionDependent();
    static_assert(!AreFieldsVersionDependent, "Fields mustn't be version dependent");
public:

    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    COMMS_MSG_FIELDS_NAMES(field1, field2, field3, field4);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when calls to doMinLength() and
    // doMaxLength() are performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 18, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 18, "Unexpected max serialisation length");

    /// @brief Default constructor
    FloatValues() = default;

    /// @brief Copy constructor
    FloatValues(const FloatValues&) = default;

    /// @brief Move constructor
    FloatValues(FloatValues&& other) = default;

    /// @brief Destructor
    ~FloatValues() noexcept = default;

    /// @brief Copy assignment
    FloatValues& operator=(const FloatValues&) = default;

    /// @brief Move assignment
    FloatValues& operator=(FloatValues&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "FloatValues";
    }
};

}  // namespace message

}  // namespace demo



