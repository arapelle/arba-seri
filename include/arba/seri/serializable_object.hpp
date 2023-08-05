#pragma once

inline namespace arba
{
namespace seri
{

template <typename input_stream, typename type>
concept input_serializable_object = requires(input_stream& stream, type& ref)
{
    { ref.read_binary(stream) };
};

template <typename output_stream, typename type>
concept output_serializable_object = requires(output_stream& stream, const type& cref)
{
    { cref.write_binary(stream) };
};

}
}
