#pragma once

#include <arba/seri/polymorphism.hpp>
#include <arba/seri/serializable_object.hpp>
#include <arba/core/htow.hpp>
#include <istream>
#include <ostream>
#include <ranges>

namespace seri
{
std::istream& read_bytes(std::istream& stream, void* buffer, std::size_t number_of_bytes_to_read);

//-----

// 4
template <typename input_stream, typename first_type, typename second_type>
requires (!std::is_const_v<first_type>) && (!std::is_const_v<second_type>)
inline input_stream& read_binary(input_stream& stream, std::pair<first_type, second_type>& value);

// 5
template <typename input_stream, typename value_type, std::size_t N>
inline input_stream& read_binary(input_stream& stream, std::array<value_type, N>& range);

// 6
template <typename input_stream, std::ranges::range range_type>
requires requires(range_type& range, std::ranges::range_value_t<range_type>&& rvalue)
{
    { range.push_back(rvalue) };
}
inline input_stream& read_binary(input_stream& stream, range_type& range);

// 7
template <typename input_stream, std::ranges::range range_type>
requires requires(input_stream& stream, range_type& range, std::ranges::range_value_t<range_type> value)
{
    { read_binary(stream, value) };
    { range.insert(std::move(value)) };
}
inline input_stream& read_binary(input_stream& stream, range_type& range);

// 8
template <typename input_stream, std::ranges::range range_type>
requires (std::tuple_size<std::ranges::range_value_t<range_type>>::value == 2)
&& requires(input_stream& stream, range_type& range, std::ranges::range_value_t<range_type> value)
{
    { range.insert(std::move(value)) };
    { value.first };
    { value.second };
    { std::ranges::range_value_t<range_type>(value.first, value.second) };
}
&& (!requires(input_stream& stream, range_type& range, std::ranges::range_value_t<range_type> value)
{
    { read_binary(stream, value) };
})
inline input_stream& read_binary(input_stream& stream, range_type& range);

// 9
template <typename input_stream>
inline input_stream& read_binary(input_stream& stream, core::uuid& value);

// 10
template <typename input_stream, typename type>
requires input_serializable_object<input_stream, type>
inline input_stream& read_binary(input_stream& stream, type& value);

// 11
template <typename input_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& input_serializable_object<input_stream, type>
inline input_stream& read_binary(input_stream& stream, std::unique_ptr<type>& value);

// 12
template <typename input_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
inline input_stream& read_binary(input_stream& stream, std::unique_ptr<type>& value);

// 13
template <typename input_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& input_serializable_object<input_stream, type>
inline input_stream& read_binary(input_stream& stream, std::shared_ptr<type>& value);

// 14
template <typename input_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
inline input_stream& read_binary(input_stream& stream, std::shared_ptr<type>& value);

//-----

// 1
template <typename input_stream, core::byte_swappable T>
inline input_stream& read_binary(input_stream& stream, T& value)
{
    read_bytes(stream, reinterpret_cast<char*>(&value), sizeof(value));
    value = core::wtoh(value);
    return stream;
}

// 2
template <typename input_stream, typename T>
requires (sizeof(T) == 1) && (std::is_convertible_v<uint8_t, T> || std::is_enum_v<T>)
inline input_stream& read_binary(input_stream& stream, T& value)
{
    uint8_t byte;
    read_bytes(stream, reinterpret_cast<char*>(&byte), 1);
    value = static_cast<T>(byte);
    return stream;
}

// 3
template <typename input_stream, typename T>
requires (sizeof(T) == 1) && (std::is_convertible_v<uint8_t, T> || std::is_enum_v<T>)
inline input_stream& read_binary(input_stream& stream, std::basic_string<T>& value)
{
    uint64_t str_size = 0;
    read_binary(stream, str_size);
    value.resize(str_size);
    read_bytes(stream, value.data(), str_size);
    return stream;
}

// 4
template <typename input_stream, typename first_type, typename second_type>
requires (!std::is_const_v<first_type>) && (!std::is_const_v<second_type>)
inline input_stream& read_binary(input_stream& stream, std::pair<first_type, second_type>& value)
{
    read_binary(stream, value.first);
    read_binary(stream, value.second);
    return stream;
}

// 5
template <typename input_stream, typename value_type, std::size_t N>
inline input_stream& read_binary(input_stream& stream, std::array<value_type, N>& range)
{
    uint64_t range_size = range.size();
    for (auto& value : range)
        read_binary(stream, value);
    return stream;
}

//-
template <typename T>
inline constexpr bool is_reservable_range_v = false;

template <std::ranges::range range_type>
requires requires(range_type& range, uint64_t size)
{
    { range.reserve(size) };
}
inline constexpr bool is_reservable_range_v<range_type> = true;
//-

// 6
template <typename input_stream, std::ranges::range range_type>
requires requires(range_type& range, std::ranges::range_value_t<range_type>&& rvalue)
{
    { range.push_back(rvalue) };
}
inline input_stream& read_binary(input_stream& stream, range_type& range)
{
    uint64_t range_size = 0;
    read_binary(stream, range_size);
    if constexpr (is_reservable_range_v<range_type>)
        range.reserve(range_size);
    auto inserter = std::back_inserter(range);
    for (; range_size > 0; --range_size)
    {
        std::ranges::range_value_t<range_type> value;
        read_binary(stream, value);
        inserter = std::move(value);
    }
    return stream;
}

// 7
template <typename input_stream, std::ranges::range range_type>
requires requires(input_stream& stream, range_type& range, std::ranges::range_value_t<range_type> value)
{
    { read_binary(stream, value) };
    { range.insert(std::move(value)) };
}
inline input_stream& read_binary(input_stream& stream, range_type& range)
{
    uint64_t range_size = 0;
    read_binary(stream, range_size);
    if constexpr (is_reservable_range_v<range_type>)
        range.reserve(range_size);
    for (; range_size > 0; --range_size)
    {
        std::ranges::range_value_t<range_type> value;
        read_binary(stream, value);
        range.insert(std::move(value));
    }
    return stream;
}

// 8
template <typename input_stream, std::ranges::range range_type>
requires (std::tuple_size<std::ranges::range_value_t<range_type>>::value == 2)
&& requires(input_stream& stream, range_type& range, std::ranges::range_value_t<range_type> value)
{
    { range.insert(std::move(value)) };
    { value.first };
    { value.second };
    { std::ranges::range_value_t<range_type>(value.first, value.second) };
}
&& (!requires(input_stream& stream, range_type& range, std::ranges::range_value_t<range_type> value)
{
    { read_binary(stream, value) };
})
inline input_stream& read_binary(input_stream& stream, range_type& range)
{
    uint64_t range_size = 0;
    read_binary(stream, range_size);
    if constexpr (is_reservable_range_v<range_type>)
        range.reserve(range_size);
    for (; range_size > 0; --range_size)
    {
        std::remove_const_t<typename std::ranges::range_value_t<range_type>::first_type> first;
        typename std::ranges::range_value_t<range_type>::second_type second;
        read_binary(stream, first);
        read_binary(stream, second);
        range.insert(std::ranges::range_value_t<range_type>(std::move(first), std::move(second)));
    }
    return stream;
}

// 9
template <typename input_stream>
inline input_stream& read_binary(input_stream& stream, core::uuid& value)
{
    read_binary(stream, value.data());
    return stream;
}

// Serializable object helper:

// 10
template <typename input_stream, typename type>
requires input_serializable_object<input_stream, type>
inline input_stream& read_binary(input_stream& stream, type& value)
{
    value.read_binary(stream);
    return stream;
}

// Smart pointers serialization:

// 11
template <typename input_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& input_serializable_object<input_stream, type>
inline input_stream& read_binary(input_stream& stream, std::unique_ptr<type>& value)
{
    uutid type_id;
    read_binary(stream, type_id);
    if (!type_id.is_nil())
    {
        value = make_unique<type>(type_id);
        value->read_binary(stream);
    }
    else
        value = nullptr;
    return stream;
}

// 12
template <typename input_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
inline input_stream& read_binary(input_stream& stream, std::unique_ptr<type>& value)
{
    bool ptr_is_not_null;
    read_binary(stream, ptr_is_not_null);
    if (ptr_is_not_null > 0)
    {
        value = std::make_unique<type>();
        read_binary(stream, *value);
    }
    else
        value = nullptr;
    return stream;
}

// 13
template <typename input_stream, typename type>
requires (abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>)
&& input_serializable_object<input_stream, type>
inline input_stream& read_binary(input_stream& stream, std::shared_ptr<type>& value)
{
    uutid type_id;
    read_binary(stream, type_id);
    if (!type_id.is_nil())
    {
        value = make_shared<type>(type_id);
        value->read_binary(stream);
    }
    else
        value = nullptr;
    return stream;
}

// 14
template <typename input_stream, typename type>
requires (!abstract_polymorphic_serializable_type<type> && !concrete_polymorphic_serializable_type<type>)
inline input_stream& read_binary(input_stream& stream, std::shared_ptr<type>& value)
{
    bool ptr_is_not_null;
    read_binary(stream, ptr_is_not_null);
    if (ptr_is_not_null > 0)
    {
        value = std::make_shared<type>();
        read_binary(stream, *value);
    }
    else
        value = nullptr;
    return stream;
}
}
