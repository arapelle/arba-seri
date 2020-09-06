#pragma once

namespace seri
{
template <typename output_stream, core::byte_swappable T>
inline output_stream& write_binary(output_stream& stream, T value)
{
    value = core::htow(value);
    stream.write(reinterpret_cast<char*>(&value), sizeof(value));
    return stream;
}

template <typename output_stream, typename T>
requires (sizeof(T) == 1) && (std::is_convertible_v<T, uint8_t> || std::is_enum_v<T>)
inline output_stream& write_binary(output_stream& stream, const T& value)
{
    stream.write(reinterpret_cast<const char*>(&value), 1);
    return stream;
}

template <typename output_stream>
inline output_stream& write_binary(output_stream& stream, const std::string& value)
{
    uint64_t str_size = value.length();
    write_binary(stream, str_size);
    stream.write(value.data(), str_size);
    return stream;
}

template <typename output_stream, typename first_type, typename second_type>
inline output_stream& write_binary(output_stream& stream, const std::pair<first_type, second_type>& value)
{
    write_binary(stream, value.first);
    write_binary(stream, value.second);
    return stream;
}

template <typename output_stream, typename value_type, std::size_t N>
inline output_stream& write_binary(output_stream& stream, const std::array<value_type, N>& range)
{
    for (const auto& value : range)
        write_binary(stream, value);
    return stream;
}

template <typename output_stream, std::ranges::sized_range range_type>
inline output_stream& write_binary(output_stream& stream, const range_type& range)
{
    uint64_t range_size = std::ranges::size(range);
    write_binary(stream, range_size);
    for (const auto& value : range)
        write_binary(stream, value);
    return stream;
}

template <typename output_stream>
output_stream& write_binary(output_stream& stream, const core::uuid& value)
{
    write_binary(stream, value.data());
    return stream;
}

template <typename output_stream, typename type>
requires std::has_virtual_destructor_v<type>
&& (std::is_abstract_v<type> || has_serializable_type_id_v<type>)
&& output_serializable_functor<output_stream, type>
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

template <typename output_stream, typename type>
requires std::has_virtual_destructor_v<type>
&& (std::is_abstract_v<type> || has_serializable_type_id_v<type>)
&& (!output_serializable_functor<output_stream, type>)
output_stream& write_binary(output_stream& stream, const std::unique_ptr<type>& value)
{
    if (value)
    {
        const type& ref = *value;
        write_binary(stream, serializable_type_id(ref));
        io_functions_base_register<type>::write_instance(stream, *value, typeid(*value));
    }
    else
        write_binary(stream, uutid());
    return stream;
}

template <typename output_stream, typename type>
requires output_serializable_functor<output_stream, type>
output_stream& write_binary(output_stream& stream, const type& value)
{
    value.write_binary(stream);
    return stream;
}
}
