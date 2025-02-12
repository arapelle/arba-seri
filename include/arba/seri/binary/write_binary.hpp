#pragma once

#include <arba/seri/binary/binary_serializable.hpp>
#include <arba/seri/polymorphism.hpp>

#include <arba/core/bit/htow.hpp>
#include <arba/uuid/uuid.hpp>

#include <array>
#include <cstdint>
#include <sstream>

inline namespace arba
{
namespace seri
{

std::ostream& write_bytes(std::ostream& stream, const void* data, std::size_t number_of_bytes_to_write);

//-----

// 4
template <typename OutputStream, typename FirstType, typename SecondType>
inline OutputStream& write_binary(OutputStream& stream, const std::pair<FirstType, SecondType>& value);

// 5
template <typename OutputStream, typename ValueType, std::size_t N>
inline OutputStream& write_binary(OutputStream& stream, const std::array<ValueType, N>& range);

// 6
template <typename OutputStream, std::ranges::sized_range RangeType>
inline OutputStream& write_binary(OutputStream& stream, const RangeType& range);

// 7
template <typename OutputStream>
OutputStream& write_binary(OutputStream& stream, const uuid::uuid& value);

// 8
template <typename OutputStream, typename Type>
    requires OutputBinarySerializable<OutputStream, Type>
OutputStream& write_binary(OutputStream& stream, const Type& value);

// 9
template <typename OutputStream, typename Type>
    requires(AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
            && OutputBinarySerializable<OutputStream, Type>
OutputStream& write_binary(OutputStream& stream, const std::unique_ptr<Type>& value);

// 10
template <typename OutputStream, typename Type>
    requires(!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
OutputStream& write_binary(OutputStream& stream, const std::unique_ptr<Type>& value);

// 11
template <typename OutputStream, typename Type>
    requires(AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
            && OutputBinarySerializable<OutputStream, Type>
OutputStream& write_binary(OutputStream& stream, const std::shared_ptr<Type>& value);

// 12
template <typename OutputStream, typename Type>
    requires(!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
OutputStream& write_binary(OutputStream& stream, const std::shared_ptr<Type>& value);

//-----

// 1
template <typename OutputStream, core::ByteSwappable Type>
inline OutputStream& write_binary(OutputStream& stream, Type value)
{
    value = core::htow(value);
    write_bytes(stream, reinterpret_cast<char*>(&value), sizeof(value));
    return stream;
}

// 2
template <typename OutputStream, typename Type>
    requires(sizeof(Type) == 1) && (std::is_convertible_v<Type, uint8_t> || std::is_enum_v<Type>)
inline OutputStream& write_binary(OutputStream& stream, const Type& value)
{
    write_bytes(stream, reinterpret_cast<const char*>(&value), 1);
    return stream;
}

// 3
template <typename OutputStream, typename Type>
    requires(sizeof(Type) == 1) && (std::is_convertible_v<uint8_t, Type> || std::is_enum_v<Type>)
inline OutputStream& write_binary(OutputStream& stream, const std::basic_string<Type>& value)
{
    uint64_t str_size = value.length();
    write_binary(stream, str_size);
    write_bytes(stream, value.data(), str_size);
    return stream;
}

// 4
template <typename OutputStream, typename FirstType, typename SecondType>
inline OutputStream& write_binary(OutputStream& stream, const std::pair<FirstType, SecondType>& value)
{
    write_binary(stream, value.first);
    write_binary(stream, value.second);
    return stream;
}

// 5
template <typename OutputStream, typename ValueType, std::size_t N>
inline OutputStream& write_binary(OutputStream& stream, const std::array<ValueType, N>& range)
{
    for (const auto& value : range)
        write_binary(stream, value);
    return stream;
}

// 6
template <typename OutputStream, std::ranges::sized_range RangeType>
inline OutputStream& write_binary(OutputStream& stream, const RangeType& range)
{
    uint64_t range_size = std::ranges::size(range);
    write_binary(stream, range_size);
    for (const auto& value : range)
        write_binary(stream, value);
    return stream;
}

// 7
template <typename OutputStream>
OutputStream& write_binary(OutputStream& stream, const uuid::uuid& value)
{
    write_binary(stream, value.data());
    return stream;
}

// Serializable object helper:

// 8
template <typename OutputStream, typename Type>
    requires OutputBinarySerializable<OutputStream, Type>
OutputStream& write_binary(OutputStream& stream, const Type& value)
{
    value.write_binary(stream);
    return stream;
}

// Smart pointers serialization:

// 9
template <typename OutputStream, typename Type>
    requires(AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
            && OutputBinarySerializable<OutputStream, Type>
OutputStream& write_binary(OutputStream& stream, const std::unique_ptr<Type>& value)
{
    if (value)
    {
        const Type& ref = *value;
        write_binary(stream, serializable_type_id(ref));
        ref.write_binary(stream);
    }
    else
        write_binary(stream, uutid());
    return stream;
}

// 10
template <typename OutputStream, typename Type>
    requires(!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
OutputStream& write_binary(OutputStream& stream, const std::unique_ptr<Type>& value)
{
    write_binary(stream, static_cast<bool>(value));
    if (value)
        write_binary(stream, *value);
    return stream;
}

// 11
template <typename OutputStream, typename Type>
    requires(AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>)
            && OutputBinarySerializable<OutputStream, Type>
OutputStream& write_binary(OutputStream& stream, const std::shared_ptr<Type>& value)
{
    if (value)
    {
        const Type& ref = *value;
        write_binary(stream, serializable_type_id(ref));
        ref.write_binary(stream);
    }
    else
        write_binary(stream, uutid());
    return stream;
}

// 12
template <typename OutputStream, typename Type>
    requires(!AbstractPolymorphicSerializable<Type> && !ConcretePolymorphicSerializable<Type>)
OutputStream& write_binary(OutputStream& stream, const std::shared_ptr<Type>& value)
{
    write_binary(stream, static_cast<bool>(value));
    if (value)
        write_binary(stream, *value);
    return stream;
}

} // namespace seri
} // namespace arba
