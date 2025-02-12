#pragma once

#include <arba/seri/polymorphism.hpp>
#include <arba/seri/binary/binary_serializable.hpp>
#include <arba/core/bit/htow.hpp>
#include <istream>
#include <ostream>
#include <ranges>

inline namespace arba
{
namespace seri
{

std::istream& read_bytes(std::istream& stream, void* buffer, std::size_t number_of_bytes_to_read);

//-----

// 4
template <typename InputStream, typename FirstType, typename SecondType>
requires (!std::is_const_v<FirstType>) && (!std::is_const_v<SecondType>)
inline InputStream& read_binary(InputStream& stream, std::pair<FirstType, SecondType>& value);

// 5
template <typename InputStream, typename ValueType, std::size_t N>
inline InputStream& read_binary(InputStream& stream, std::array<ValueType, N>& range);

// 6
template <typename InputStream, std::ranges::range RangeType>
requires requires(RangeType& range, std::ranges::range_value_t<RangeType>&& rvalue)
{
    { range.push_back(rvalue) };
}
inline InputStream& read_binary(InputStream& stream, RangeType& range);

// 7
template <typename InputStream, std::ranges::range RangeType>
requires requires(InputStream& stream, RangeType& range, std::ranges::range_value_t<RangeType> value)
{
    { read_binary(stream, value) };
    { range.insert(std::move(value)) };
}
inline InputStream& read_binary(InputStream& stream, RangeType& range);

// 8
template <typename InputStream, std::ranges::range RangeType>
requires (std::tuple_size<std::ranges::range_value_t<RangeType>>::value == 2)
&& requires(InputStream& stream, RangeType& range, std::ranges::range_value_t<RangeType> value)
{
    { range.insert(std::move(value)) };
    { value.first };
    { value.second };
    { std::ranges::range_value_t<RangeType>(value.first, value.second) };
}
&& (!requires(InputStream& stream, RangeType& range, std::ranges::range_value_t<RangeType> value)
{
    { read_binary(stream, value) };
})
inline InputStream& read_binary(InputStream& stream, RangeType& range);

// 9
template <typename InputStream>
inline InputStream& read_binary(InputStream& stream, uuid::uuid& value);

// 10
template <typename InputStream, typename Type>
requires InputBinarySerializable<InputStream, Type>
inline InputStream& read_binary(InputStream& stream, Type& value);

// 11
template <typename InputStream, typename Type>
requires (AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
&& InputBinarySerializable<InputStream, Type>
inline InputStream& read_binary(InputStream& stream, std::unique_ptr<Type>& value);

// 12
template <typename InputStream, typename Type>
requires (!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
inline InputStream& read_binary(InputStream& stream, std::unique_ptr<Type>& value);

// 13
template <typename InputStream, typename Type>
requires (AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
&& InputBinarySerializable<InputStream, Type>
inline InputStream& read_binary(InputStream& stream, std::shared_ptr<Type>& value);

// 14
template <typename InputStream, typename Type>
requires (!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
inline InputStream& read_binary(InputStream& stream, std::shared_ptr<Type>& value);

//-----

// 1
template <typename InputStream, core::ByteSwappable Type>
inline InputStream& read_binary(InputStream& stream, Type& value)
{
    read_bytes(stream, reinterpret_cast<char*>(&value), sizeof(value));
    value = core::wtoh(value);
    return stream;
}

// 2
template <typename InputStream, typename Type>
requires (sizeof(Type) == 1) && (std::is_convertible_v<uint8_t, Type> || std::is_enum_v<Type>)
inline InputStream& read_binary(InputStream& stream, Type& value)
{
    uint8_t byte;
    read_bytes(stream, reinterpret_cast<char*>(&byte), 1);
    value = static_cast<Type>(byte);
    return stream;
}

// 3
template <typename InputStream, typename Type>
requires (sizeof(Type) == 1) && (std::is_convertible_v<uint8_t, Type> || std::is_enum_v<Type>)
inline InputStream& read_binary(InputStream& stream, std::basic_string<Type>& value)
{
    uint64_t str_size = 0;
    read_binary(stream, str_size);
    value.resize(str_size);
    read_bytes(stream, value.data(), str_size);
    return stream;
}

// 4
template <typename InputStream, typename FirstType, typename SecondType>
requires (!std::is_const_v<FirstType>) && (!std::is_const_v<SecondType>)
inline InputStream& read_binary(InputStream& stream, std::pair<FirstType, SecondType>& value)
{
    read_binary(stream, value.first);
    read_binary(stream, value.second);
    return stream;
}

// 5
template <typename InputStream, typename ValueType, std::size_t N>
inline InputStream& read_binary(InputStream& stream, std::array<ValueType, N>& range)
{
    for (auto& value : range)
        read_binary(stream, value);
    return stream;
}

//-
template <typename Type>
inline constexpr bool is_reservable_range_v = false;

template <std::ranges::range RangeType>
requires requires(RangeType& range, uint64_t size)
{
    { range.reserve(size) };
}
inline constexpr bool is_reservable_range_v<RangeType> = true;
//-

// 6
template <typename InputStream, std::ranges::range RangeType>
requires requires(RangeType& range, std::ranges::range_value_t<RangeType>&& rvalue)
{
    { range.push_back(rvalue) };
}
inline InputStream& read_binary(InputStream& stream, RangeType& range)
{
    uint64_t range_size = 0;
    read_binary(stream, range_size);
    if constexpr (is_reservable_range_v<RangeType>)
        range.reserve(range_size);
    auto inserter = std::back_inserter(range);
    for (; range_size > 0; --range_size)
    {
        std::ranges::range_value_t<RangeType> value;
        read_binary(stream, value);
        inserter = std::move(value);
    }
    return stream;
}

// 7
template <typename InputStream, std::ranges::range RangeType>
requires requires(InputStream& stream, RangeType& range, std::ranges::range_value_t<RangeType> value)
{
    { read_binary(stream, value) };
    { range.insert(std::move(value)) };
}
inline InputStream& read_binary(InputStream& stream, RangeType& range)
{
    uint64_t range_size = 0;
    read_binary(stream, range_size);
    if constexpr (is_reservable_range_v<RangeType>)
        range.reserve(range_size);
    for (; range_size > 0; --range_size)
    {
        std::ranges::range_value_t<RangeType> value;
        read_binary(stream, value);
        range.insert(std::move(value));
    }
    return stream;
}

// 8
template <typename InputStream, std::ranges::range RangeType>
requires (std::tuple_size<std::ranges::range_value_t<RangeType>>::value == 2)
&& requires(InputStream& stream, RangeType& range, std::ranges::range_value_t<RangeType> value)
{
    { range.insert(std::move(value)) };
    { value.first };
    { value.second };
    { std::ranges::range_value_t<RangeType>(value.first, value.second) };
}
&& (!requires(InputStream& stream, RangeType& range, std::ranges::range_value_t<RangeType> value)
{
    { read_binary(stream, value) };
})
inline InputStream& read_binary(InputStream& stream, RangeType& range)
{
    uint64_t range_size = 0;
    read_binary(stream, range_size);
    if constexpr (is_reservable_range_v<RangeType>)
        range.reserve(range_size);
    for (; range_size > 0; --range_size)
    {
        std::remove_const_t<typename std::ranges::range_value_t<RangeType>::first_type> first;
        typename std::ranges::range_value_t<RangeType>::second_type second;
        read_binary(stream, first);
        read_binary(stream, second);
        range.insert(std::ranges::range_value_t<RangeType>(std::move(first), std::move(second)));
    }
    return stream;
}

// 9
template <typename InputStream>
inline InputStream& read_binary(InputStream& stream, uuid::uuid& value)
{
    read_binary(stream, value.data());
    return stream;
}

// Serializable object helper:

// 10
template <typename InputStream, typename Type>
requires InputBinarySerializable<InputStream, Type>
inline InputStream& read_binary(InputStream& stream, Type& value)
{
    value.read_binary(stream);
    return stream;
}

// Smart pointers serialization:

// 11
template <typename InputStream, typename Type>
requires (AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
&& InputBinarySerializable<InputStream, Type>
inline InputStream& read_binary(InputStream& stream, std::unique_ptr<Type>& value)
{
    uutid type_id;
    read_binary(stream, type_id);
    if (!type_id.is_nil())
    {
        value = make_unique<Type>(type_id);
        value->read_binary(stream);
    }
    else
        value = nullptr;
    return stream;
}

// 12
template <typename InputStream, typename Type>
requires (!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
inline InputStream& read_binary(InputStream& stream, std::unique_ptr<Type>& value)
{
    bool ptr_is_not_null;
    read_binary(stream, ptr_is_not_null);
    if (ptr_is_not_null > 0)
    {
        value = std::make_unique<Type>();
        read_binary(stream, *value);
    }
    else
        value = nullptr;
    return stream;
}

// 13
template <typename InputStream, typename Type>
requires (AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
&& InputBinarySerializable<InputStream, Type>
inline InputStream& read_binary(InputStream& stream, std::shared_ptr<Type>& value)
{
    uutid type_id;
    read_binary(stream, type_id);
    if (!type_id.is_nil())
    {
        value = make_shared<Type>(type_id);
        value->read_binary(stream);
    }
    else
        value = nullptr;
    return stream;
}

// 14
template <typename InputStream, typename Type>
requires (!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
inline InputStream& read_binary(InputStream& stream, std::shared_ptr<Type>& value)
{
    bool ptr_is_not_null;
    read_binary(stream, ptr_is_not_null);
    if (ptr_is_not_null > 0)
    {
        value = std::make_shared<Type>();
        read_binary(stream, *value);
    }
    else
        value = nullptr;
    return stream;
}

}
}
