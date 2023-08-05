#pragma once

#include <arba/core/uuid.hpp>
#include <sstream>
#include <string>
#include <typeindex>
#include <functional>
#include <memory>

inline namespace arba
{
namespace seri
{

/**
 * \brief Universal Unique Type ID
 */
using uutid = core::uuid;

// Constant to specialize for each polymorphic serializable type (cf. Helper macros):

template <typename type>
inline constexpr bool has_serializable_type_id_v = false;

// Polymorphic serializable type concepts:

template <typename type>
concept abstract_polymorphic_serializable_type =
        std::has_virtual_destructor_v<type>
        && std::is_abstract_v<type>
        && (!has_serializable_type_id_v<type>);

template <typename type>
concept concrete_polymorphic_serializable_type =
        std::has_virtual_destructor_v<type>
        && (!std::is_abstract_v<type>)
        && has_serializable_type_id_v<type>;

// Function to specialize for each polymorphic serializable type (cf. Helper macros):

template <typename type>
requires concrete_polymorphic_serializable_type<type>
const uutid& serializable_type_id();

//-----

class concrete_polymorphic_serializable_types final
{
private:
    ~concrete_polymorphic_serializable_types() = delete;

    using types_register = std::unordered_map<std::type_index, uutid>;

    static types_register& global_types_register()
    {
        static types_register global_register;
        return global_register;
    }

    template <typename type>
    requires concrete_polymorphic_serializable_type<type>
    static void register_type()
    {
        auto res = global_types_register().try_emplace(typeid(type), serializable_type_id<type>());
        if (!res.second)
        {
            std::ostringstream stream;
            stream << "Serializable type is already registered: " << typeid(type).name() << ".";
            throw std::runtime_error(stream.str());
        }
    }

    template <typename base, typename derived>
    requires concrete_polymorphic_serializable_type<derived>
    friend class inheritance_relation;

    template <typename type>
    requires abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>
    friend uutid serializable_type_id(const type& value);
};

template <typename type>
requires abstract_polymorphic_serializable_type<type> || concrete_polymorphic_serializable_type<type>
inline uutid serializable_type_id(const type& value)
{
    auto iter = concrete_polymorphic_serializable_types::global_types_register().find(typeid(value));
    if (iter != concrete_polymorphic_serializable_types::global_types_register().end())
        return iter->second;
    std::ostringstream stream;
    stream << "Type is not registered as a serializable type: " << typeid(type).name() << ".";
    throw std::runtime_error(stream.str());
}

template <typename base>
requires abstract_polymorphic_serializable_type<base> || concrete_polymorphic_serializable_type<base>
class inheritance_relation_base
{
protected:
    using clone_function = base*(*)();
    using clone_functions_register = std::unordered_map<uutid, clone_function>;

private:
    static base* null_clone() { return nullptr; }

    static clone_functions_register create_clone_functions_register()
    {
        clone_functions_register functions_register;
        functions_register.emplace(uutid(), &null_clone);
        return functions_register;
    }

protected:
    static clone_functions_register& global_clone_functions_register()
    {
        static clone_functions_register functions_register = create_clone_functions_register();
        return functions_register;
    }

public:
    static base* make_instance(const uutid& id)
    {
        return global_clone_functions_register().at(id)();
    }
};

template <typename base, typename derived>
requires concrete_polymorphic_serializable_type<derived>
class inheritance_relation final : public inheritance_relation_base<base>
{
    using clone_functions_register = typename inheritance_relation_base<base>::clone_functions_register;

    struct dummy final {};
    ~inheritance_relation() = delete;

    static base* clone() { return new derived(); }

    inline static dummy register_base_derived_relation_()
    {
        if constexpr(std::is_same_v<base, derived>)
            concrete_polymorphic_serializable_types::register_type<derived>();

        clone_functions_register& functions_register = inheritance_relation_base<base>::global_clone_functions_register();
        auto emplace_res = functions_register.try_emplace(serializable_type_id<derived>(), &clone);
        if (!emplace_res.second)
        {
            std::ostringstream stream;
            stream << "Serializable type id is already associated with a type: " << serializable_type_id<derived>() << ".";
            throw std::runtime_error(stream.str());
        }

        return dummy{};
    }
    inline static dummy auto_init_ = register_base_derived_relation_();
};

// Helper macros:

#define SERI_DEFINE_SERIALIZABLE_TYPE_ID(type, id) \
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

// Helper make functions:

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

}
}
