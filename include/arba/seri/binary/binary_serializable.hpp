#pragma once

inline namespace arba
{
namespace seri
{

template <typename InputStream, typename Type>
concept InputBinarySerializable = requires(InputStream& stream, Type& ref) {
    { ref.read_binary(stream) };
};

template <typename OutputStream, typename Type>
concept OutputBinarySerializable = requires(OutputStream& stream, const Type& cref) {
    { cref.write_binary(stream) };
};

} // namespace seri
} // namespace arba
