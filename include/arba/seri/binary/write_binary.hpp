#pragma once

#include <arba/seri/polymorphism.hpp>
#include <arba/seri/serializable_object.hpp>
#include <arba/core/uuid.hpp>
#include <arba/core/htow.hpp>
#include <array>
#include <sstream>
#include <cstdint>

namespace seri
{
std::ostream& write_bytes(std::ostream& stream, const void* data, std::size_t number_of_bytes_to_write);

//-----

// 4
template <typename output_stream, typename first_type, typename second_type>
inline output_stream& write_binary(output_stream& stream, const std::pair<first_type, second_type>& value);

// 5
template <typename output_stream, typename value_type, std::size_t N>
inline output_stream& write_binary(output_stream& stream, const std::array<value_type, N>& range);

// 6
template <typename output_stream, std::ranges::sized_range range_type>
inline output_stream& write_binary(output_stream& stream, const range_type& range);

// 7
template <typename output_stream>
output_stream& write_binary(output_stream& stream, const core::uuid& value);

// 8
template <typename output_stream, typename type>
requires output_serializable_object<output_stream, type>
output_stream& write_binary(output_stream& stream, const type& value);

// 9
template <typename output_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& output_serializable_object<output_stream, type>
output_stream& write_binary(output_stream& stream, const std::unique_ptr<type>& value);

// 10
template <typename output_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
output_stream& write_binary(output_stream& stream, const std::unique_ptr<type>& value);

// 11
template <typename output_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& output_serializable_object<output_stream, type>
output_stream& write_binary(output_stream& stream, const std::shared_ptr<type>& value);

// 12
template <typename output_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
output_stream& write_binary(output_stream& stream, const std::shared_ptr<type>& value);

//-----

// 1
template <typename output_stream, core::byte_swappable T>
inline output_stream& write_binary(output_stream& stream, T value)
{
    value = core::htow(value);
    write_bytes(stream, reinterpret_cast<char*>(&value), sizeof(value));
    return stream;
}

// 2
template <typename output_stream, typename T>
requires (sizeof(T) == 1) && (std::is_convertible_v<T, uint8_t> || std::is_enum_v<T>)
inline output_stream& write_binary(output_stream& stream, const T& value)
{
    write_bytes(stream, reinterpret_cast<const char*>(&value), 1);
    return stream;
}

// 3
template <typename output_stream, typename T>
requires (sizeof(T) == 1) && (std::is_convertible_v<uint8_t, T> || std::is_enum_v<T>)
inline output_stream& write_binary(output_stream& stream, const std::basic_string<T>& value)
{
    uint64_t str_size = value.length();
    write_binary(stream, str_size);
    write_bytes(stream, value.data(), str_size);
    return stream;
}

// 4
template <typename output_stream, typename first_type, typename second_type>
inline output_stream& write_binary(output_stream& stream, const std::pair<first_type, second_type>& value)
{
    write_binary(stream, value.first);
    write_binary(stream, value.second);
    return stream;
}

// 5
template <typename output_stream, typename value_type, std::size_t N>
inline output_stream& write_binary(output_stream& stream, const std::array<value_type, N>& range)
{
    for (const auto& value : range)
        write_binary(stream, value);
    return stream;
}

// 6
template <typename output_stream, std::ranges::sized_range range_type>
inline output_stream& write_binary(output_stream& stream, const range_type& range)
{
    uint64_t range_size = std::ranges::size(range);
    write_binary(stream, range_size);
    for (const auto& value : range)
        write_binary(stream, value);
    return stream;
}

// 7
template <typename output_stream>
output_stream& write_binary(output_stream& stream, const core::uuid& value)
{
    write_binary(stream, value.data());
    return stream;
}

// Serializable object helper:

// 8
template <typename output_stream, typename type>
requires output_serializable_object<output_stream, type>
output_stream& write_binary(output_stream& stream, const type& value)
{
    value.write_binary(stream);
    return stream;
}

// Smart pointers serialization:

// 9
template <typename output_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& output_serializable_object<output_stream, type>
output_stream& write_binary(output_stream& stream, const std::unique_ptr<type>& value)
{
    if (value)
    {
        const type& ref = *value;
        write_binary(stream, serializable_type_id(ref));
        ref.write_binary(stream);
    }
    else
        write_binary(stream, uutid());
    return stream;
}

// 10
template <typename output_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
output_stream& write_binary(output_stream& stream, const std::unique_ptr<type>& value)
{
    write_binary(stream, static_cast<bool>(value));
    if (value)
        write_binary(stream, *value);
    return stream;
}

// 11
template <typename output_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& output_serializable_object<output_stream, type>
output_stream& write_binary(output_stream& stream, const std::shared_ptr<type>& value)
{
    if (value)
    {
        const type& ref = *value;
        write_binary(stream, serializable_type_id(ref));
        ref.write_binary(stream);
    }
    else
        write_binary(stream, uutid());
    return stream;
}

// 12
template <typename output_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
output_stream& write_binary(output_stream& stream, const std::shared_ptr<type>& value)
{
    write_binary(stream, static_cast<bool>(value));
    if (value)
        write_binary(stream, *value);
    return stream;
}
}
