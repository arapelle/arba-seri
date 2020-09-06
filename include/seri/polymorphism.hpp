#pragma once 

#include <core/uuid.hpp>
#include "archive/archive_base.hpp"
#include <typeindex>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

namespace seri
{
using uutid = core::uuid;

template <typename type>
const uutid& serializable_type_id();

template <typename type>
inline constexpr bool has_serializable_type_id_v = false;

class serializable_types final
{
private:
    using types_register = std::unordered_map<std::type_index, uutid>;

    static types_register& serializable_types_register()
    {
        static types_register global_register;
        return global_register;
    }

public:
    template <typename type>
    requires std::has_virtual_destructor_v<type> && has_serializable_type_id_v<type> && (!std::is_abstract_v<type>)
    static void register_type()
    {
        auto res = serializable_types_register().try_emplace(typeid(type), serializable_type_id<type>());
        if (!res.second)
        {
            std::ostringstream stream;
            stream << "Serializable type is already registered: " << typeid(type).name() << ".";
            throw std::runtime_error(stream.str());
        }
    }

    template <typename type>
    friend uutid serializable_type_id(const type& value);
};

template <typename type>
inline uutid serializable_type_id(const type& value)
{
    auto iter = serializable_types::serializable_types_register().find(typeid(value));
    if (iter != serializable_types::serializable_types_register().end())
        return iter->second;
    std::ostringstream stream;
    stream << "Type is not registered as a serializable type: " << typeid(type).name() << ".";
    throw std::runtime_error(stream.str());
}

/**/
template <typename base>
class io_functions_base_register
{
public:
    using read_function = void(*)(iarchive&, base&);
    using write_function = void(*)(oarchive&, const base&);
    using read_functions_register = std::unordered_map<uutid, read_function>;
    using write_functions_register = std::unordered_map<std::type_index, write_function>;

    inline static read_functions_register& get_read_functions_register()
    {
        static read_functions_register global_register;
        return global_register;
    }

    inline static write_functions_register& get_write_functions_register()
    {
        static write_functions_register global_register;
        return global_register;
    }

public:
    template <typename derived>
    static void register_derived(read_function read_fn, write_function write_fn)
    {
        get_read_functions_register().emplace(serializable_type_id<derived>(), read_fn);
        get_write_functions_register().emplace(typeid(derived), write_fn);
    }

    static void read_instance(iarchive& archive, base& value, const uutid& id)
    {
        auto read_fn = get_read_functions_register().at(id);
        read_fn(archive, value);
    }

    static void write_instance(oarchive& archive, base& value, const std::type_index& id)
    {
        auto write_fn = get_write_functions_register().at(id);
        write_fn(archive, value);
    }
};
/**/

template <typename base>
requires std::has_virtual_destructor_v<base> && (std::is_abstract_v<base> || has_serializable_type_id_v<base>)
class inheritance_relation_base
{
protected:
    using clone_function = base*(*)();
    using base_register = std::unordered_map<uutid, clone_function>;

private:
    static base* null_clone() { return nullptr; }

    static auto create_base_register()
    {
        std::unordered_map<uutid, clone_function> base_register;
        base_register.emplace(uutid(), &null_clone);
        return base_register;
    }

protected:
    static base_register& get_base_register()
    {
        static std::unordered_map<uutid, clone_function> global_base_register = create_base_register();
        return global_base_register;
    }

public:
    static base* make_instance(const uutid& id)
    {
        return get_base_register().at(id)();
    }
};

template <typename base, typename derived>
requires (!std::is_abstract_v<derived>) && std::has_virtual_destructor_v<derived>
class inheritance_relation final : public inheritance_relation_base<base>
{
    using base_register = typename inheritance_relation_base<base>::base_register;

    struct dummy final {};
    ~inheritance_relation() = delete;

    static base* clone() { return new derived(); }

    static void read_binary_(iarchive& stream, base& value)
    {
//        if constexpr(!std::is_same_v<base, derived>)
//            read_binary(stream, dynamic_cast<derived&>(value));
    }

    static void write_binary_(oarchive& stream, const base& value)
    {
//        if constexpr(!std::is_same_v<base, derived>)
//            write_binary(stream, dynamic_cast<const derived&>(value));
    }

    inline static dummy register_base_derived_relation_()
    {
        if constexpr(std::is_same_v<base, derived>)
            serializable_types::register_type<derived>();
        base_register& b_register = inheritance_relation_base<base>::get_base_register();
        auto res = b_register.try_emplace(serializable_type_id<derived>(), &clone);
        if (!res.second)
        {
            std::ostringstream stream;
            stream << "Serializable type id is already associated with a type: " << serializable_type_id<derived>() << ".";
            throw std::runtime_error(stream.str());
        }
        if constexpr(!std::is_same_v<base, derived>)
            io_functions_base_register<base>::template register_derived<derived>(&read_binary_, &write_binary_);
        return dummy{};
    }
    inline static dummy auto_init_ = register_base_derived_relation_();
};

#define SERI_DEFINE_TYPE_ID(type, id) \
    namespace seri \
    { \
    template <> \
    inline constexpr bool has_serializable_type_id_v<type> = true; \
    template <> \
    const uutid& serializable_type_id<type>() { static const uutid uid(id); return uid; } \
    template class inheritance_relation<type, type>; \
    }

#define SERI_REGISTER_INHERITANCE_RELATION(base, derived) \
    namespace seri \
    { \
    template class inheritance_relation<base, derived>; \
    }

template <typename base>
base* make_instance(const uutid& id)
{
    return inheritance_relation_base<base>::make_instance(id);
}

template <typename base>
std::unique_ptr<base> make_unique(const uutid& id)
{
    return std::unique_ptr<base>(make_instance<base>(id));
}

template <typename base>
std::shared_ptr<base> make_shared(const uutid& id)
{
    return std::shared_ptr<base>(make_instance<base>(id));
}

template <typename input_stream, typename type>
concept input_serializable_functor = requires(input_stream& istream, type& ref)
{
    { ref.read_binary(istream) };
};

template <typename output_stream, typename type>
concept output_serializable_functor = requires(output_stream& ostream, const type& cref)
{
    { cref.write_binary(ostream) };
};
}
