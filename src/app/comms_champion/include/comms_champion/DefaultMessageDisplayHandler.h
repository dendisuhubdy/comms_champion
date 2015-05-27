//
// Copyright 2014 (C). Alex Robenko. All rights reserved.
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

#include <cassert>
#include <type_traits>
#include <QtWidgets/QVBoxLayout>

#include "comms/comms.h"

#include "comms_champion/Message.h"
#include "comms_champion/MessageDisplayHandler.h"
#include "comms_champion/DefaultMessageWidget.h"

#include "comms_champion/field_wrapper/IntValueWrapper.h"
#include "comms_champion/field_wrapper/LongIntValueWrapper.h"
#include "comms_champion/field_wrapper/BitmaskValueWrapper.h"
#include "comms_champion/field_wrapper/EnumValueWrapper.h"
#include "comms_champion/field_wrapper/StringWrapper.h"
#include "comms_champion/field_wrapper/BitfieldWrapper.h"
#include "comms_champion/field_wrapper/OptionalWrapper.h"
#include "comms_champion/field_wrapper/UnknownValueWrapper.h"

namespace comms_champion
{

namespace details
{

struct IntValueTag {};
struct BitmaskValueTag {};
struct EnumValueTag {};
struct StringTag {};
struct BitfieldTag {};
struct OptionalTag {};
struct UnknownValueTag {};

template <typename TField>
struct TagOf
{
    static_assert(!comms::field::isIntValue<TField>(),
        "IntValue is perceived as unknown type");
    static_assert(!comms::field::isBitmaskValue<TField>(),
        "BitmaskValue is perceived as unknown type");
    static_assert(!comms::field::isEnumValue<TField>(),
        "EnumValue is perceived as unknown type");
    static_assert(!comms::field::isString<TField>(),
        "String is perceived as unknown type");
    static_assert(!comms::field::isBitfield<TField>(),
        "Bitfield is perceived as unknown type");
    static_assert(!comms::field::isOptional<TField>(),
        "Optional is perceived as unknown type");
    typedef UnknownValueTag Type;
};

template <typename... TArgs>
struct TagOf<comms::field::IntValue<TArgs...> >
{
    static_assert(
        comms::field::isIntValue<comms::field::IntValue<TArgs...> >(),
        "isIntValue is supposed to return true");

    typedef IntValueTag Type;
};

template <typename... TArgs>
struct TagOf<comms::field::BitmaskValue<TArgs...> >
{
    static_assert(
        comms::field::isBitmaskValue<comms::field::BitmaskValue<TArgs...> >(),
        "isBitmaskValue is supposed to return true");

    typedef BitmaskValueTag Type;
};

template <typename... TArgs>
struct TagOf<comms::field::EnumValue<TArgs...> >
{
    static_assert(
        comms::field::isEnumValue<comms::field::EnumValue<TArgs...> >(),
        "isEnumValue is supposed to return true");

    typedef EnumValueTag Type;
};

template <typename... TArgs>
struct TagOf<comms::field::String<TArgs...> >
{
    static_assert(
        comms::field::isString<comms::field::String<TArgs...> >(),
        "isString is supposed to return true");

    typedef StringTag Type;
};

template <typename... TArgs>
struct TagOf<comms::field::Bitfield<TArgs...> >
{
    static_assert(
        comms::field::isBitfield<comms::field::Bitfield<TArgs...> >(),
        "isBitfield is supposed to return true");

    typedef BitfieldTag Type;
};

template <typename... TArgs>
struct TagOf<comms::field::Optional<TArgs...> >
{
    static_assert(
        comms::field::isOptional<comms::field::Optional<TArgs...> >(),
        "isOptional is supposed to return true");

    typedef OptionalTag Type;
};

template <typename TField>
using TagOfT = typename TagOf<TField>::Type;

}  // namespace details

class DefaultMessageDisplayHandler : public MessageDisplayHandler
{
public:

    template <typename TMessage>
    void handle(TMessage& msg)
    {
        auto& fields = msg.getFields();
        comms::util::tupleForEach(
            fields,
            makeFieldsDisplayDispatcher(
                *this,
                [this](FieldWidgetPtr&& fieldWidget)
                {
                    m_widget->addFieldWidget(fieldWidget.release());
                }));
    }

protected:

    using FieldWidgetPtr = std::unique_ptr<FieldWidget>;

    virtual MsgWidgetPtr createMsgWidgetImpl(Message& msg) override;

private:

    using IntValueTag = details::IntValueTag;
    using BitmaskValueTag = details::BitmaskValueTag;
    using EnumValueTag = details::EnumValueTag;
    using StringTag = details::StringTag;
    using BitfieldTag = details::BitfieldTag;
    using OptionalTag = details::OptionalTag;
    using UnknownValueTag = details::UnknownValueTag;

    struct IntValueWrapperTag {};
    struct LongIntValueWrapperTag {};

    template <typename TCreateWidgetHandler>
    class FieldsDisplayDispatcher
    {
    public:
        typedef std::function <void (FieldWidgetPtr)> WidgetDispatchFunc;
        template <typename TDispatchFunc>
        FieldsDisplayDispatcher(TCreateWidgetHandler& handler, TDispatchFunc&& dispatchOp)
          : m_handler(handler),
            m_dispatchOp(std::forward<TDispatchFunc>(dispatchOp))
        {
        }

        template <typename TField>
        void operator()(TField&& field)
        {
            using FieldType = typename std::decay<TField>::type;
            using Tag = details::TagOfT<FieldType>;

            auto fieldWidget =
                m_handler.createFieldWidget(std::forward<TField>(field), Tag());
            m_dispatchOp(std::move(fieldWidget));
        }

    private:
        TCreateWidgetHandler& m_handler;
        WidgetDispatchFunc m_dispatchOp;
    };

    template <typename TCreateWidgetHandler>
    friend class FieldsDisplayDispatcher;

    template <typename TCreateWidgetHandler, typename TDispatchFunc>
    FieldsDisplayDispatcher<TCreateWidgetHandler>
    makeFieldsDisplayDispatcher(TCreateWidgetHandler& handler, TDispatchFunc&& dispatchOp)
    {
        return FieldsDisplayDispatcher<TCreateWidgetHandler>(handler, std::forward<TDispatchFunc>(dispatchOp));
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, IntValueTag)
    {
        typedef typename std::decay<decltype(field)>::type DecayedField;
        typedef typename std::conditional<
            field_wrapper::IntValueWrapper::canHandleField<DecayedField>(),
            IntValueWrapperTag,
            LongIntValueWrapperTag
        >::type Tag;

        return createIntValueFieldWidgetInternal(field, Tag());
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, BitmaskValueTag)
    {
        return createBitmaskValueFieldWidget(
            field_wrapper::makeBitmaskValueWrapper(field));
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, EnumValueTag)
    {
        return createEnumValueFieldWidget(
            field_wrapper::makeEnumValueWrapper(field));
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, StringTag)
    {
        return createStringFieldWidget(
            field_wrapper::makeStringWrapper(field));
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, BitfieldTag)
    {
        auto widget = createBitfieldFieldWidget(
            field_wrapper::makeBitfieldWrapper(field));

        auto& memberFields = field.members();
        comms::util::tupleForEach(
            memberFields,
            makeFieldsDisplayDispatcher(
                *this,
                [this, &widget](FieldWidgetPtr&& fieldWidget)
                {
                    bitfieldWidgetAddMember(*widget, std::move(fieldWidget));
                }));

        return std::move(widget);
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, OptionalTag)
    {
        auto widget = createOptionalFieldWidget(
            field_wrapper::makeOptionalWrapper(field));

        auto wrappedOptField = std::forward_as_tuple(field.field());
        comms::util::tupleForEach(
            wrappedOptField,
            makeFieldsDisplayDispatcher(
                *this,
                [this, &widget](FieldWidgetPtr&& fieldWidget)
                {
                    optionalWidgetSetField(*widget, std::move(fieldWidget));
                }));

        return std::move(widget);
    }

    template <typename TField>
    FieldWidgetPtr createFieldWidget(TField& field, UnknownValueTag)
    {
        return createUnknownValueFieldWidget(
            field_wrapper::makeUnknownValueWrapper(field));
    }

    FieldWidgetPtr createIntValueFieldWidget(
        field_wrapper::IntValueWrapperPtr fieldWrapper);

    FieldWidgetPtr createLongIntValueFieldWidget(
        field_wrapper::LongIntValueWrapperPtr fieldWrapper);

    FieldWidgetPtr createBitmaskValueFieldWidget(
        field_wrapper::BitmaskValueWrapperPtr&& fieldWrapper);

    FieldWidgetPtr createEnumValueFieldWidget(
        field_wrapper::EnumValueWrapperPtr&& fieldWrapper);

    FieldWidgetPtr createStringFieldWidget(
        field_wrapper::StringWrapperPtr&& fieldWrapper);

    FieldWidgetPtr createBitfieldFieldWidget(
        field_wrapper::BitfieldWrapperPtr&& fieldWrapper);

    FieldWidgetPtr createOptionalFieldWidget(
        field_wrapper::OptionalWrapperPtr fieldWrapper);

    FieldWidgetPtr createUnknownValueFieldWidget(
        field_wrapper::UnknownValueWrapperPtr&& fieldWrapper);

    void bitfieldWidgetAddMember(
        FieldWidget& optionalWidget,
        FieldWidgetPtr fieldWidget);

    void optionalWidgetSetField(
        FieldWidget& bitfieldWidget,
        FieldWidgetPtr memberFieldWidget);

    static void updateFieldIdxProperty(FieldWidget& field, std::size_t idx);

    template <typename TField>
    FieldWidgetPtr createIntValueFieldWidgetInternal(TField& field, IntValueWrapperTag)
    {
        return createIntValueFieldWidget(
            field_wrapper::makeIntValueWrapper(field));
    }

    template <typename TField>
    FieldWidgetPtr createIntValueFieldWidgetInternal(TField& field, LongIntValueWrapperTag)
    {
        return createLongIntValueFieldWidget(
            field_wrapper::makeLongIntValueWrapper(field));
    }


    using DefaultMsgWidgetPtr = std::unique_ptr<DefaultMessageWidget>;
    DefaultMsgWidgetPtr m_widget;
};

}  // namespace comms_champion


