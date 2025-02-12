#pragma once

#include <arba/uuid/uuid.hpp>

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <typeindex>

inline namespace arba
{
namespace seri
{

/**
 * \brief Universal Unique Type ID
 */
using uutid = uuid::uuid;

// Constant to specialize for each polymorphic serializable type (cf. Helper macros):

template <typename Type>
inline constexpr bool has_serializable_type_id_v = false;

// Polymorphic serializable type concepts:

template <typename Type>
concept AbstractPolymorphicSerializable =
    std::has_virtual_destructor_v<Type> && std::is_abstract_v<Type> && (!has_serializable_type_id_v<Type>);

template <typename Type>
concept ConcretePolymorphicSerializable =
    std::has_virtual_destructor_v<Type> && (!std::is_abstract_v<Type>) && has_serializable_type_id_v<Type>;

// Function to specialize for each polymorphic serializable type (cf. Helper macros):

template <typename Type>
    requires ConcretePolymorphicSerializable<Type>
const uutid& serializable_type_id();

//-----

class ConcretePolymorphicSerializables final
{
private:
    ~ConcretePolymorphicSerializables() = delete;

    using types_register = std::unordered_map<std::type_index, uutid>;

    static types_register& global_types_register()
    {
        static types_register global_register;
        return global_register;
    }

    template <typename Type>
        requires ConcretePolymorphicSerializable<Type>
    static void register_type()
    {
        auto res = global_types_register().try_emplace(typeid(Type), serializable_type_id<Type>());
        if (!res.second)
        {
            std::ostringstream stream;
            stream << "Serializable type is already registered: " << typeid(Type).name() << ".";
            throw std::runtime_error(stream.str());
        }
    }

    template <typename Base, typename Derived>
        requires ConcretePolymorphicSerializable<Derived>
    friend class inheritance_relation;

    template <typename Type>
        requires AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>
    friend uutid serializable_type_id(const Type& value);
};

template <typename Type>
    requires AbstractPolymorphicSerializable<Type> || ConcretePolymorphicSerializable<Type>
inline uutid serializable_type_id(const Type& value)
{
    auto iter = ConcretePolymorphicSerializables::global_types_register().find(typeid(value));
    if (iter != ConcretePolymorphicSerializables::global_types_register().end())
        return iter->second;
    std::ostringstream stream;
    stream << "Type is not registered as a serializable type: " << typeid(Type).name() << ".";
    throw std::runtime_error(stream.str());
}

template <typename Base>
    requires AbstractPolymorphicSerializable<Base> || ConcretePolymorphicSerializable<Base>
class inheritance_relation_base
{
protected:
    using clone_function = Base* (*)();
    using clone_functions_register = std::unordered_map<uutid, clone_function>;

private:
    static Base* null_clone() { return nullptr; }

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
    static Base* make_instance(const uutid& id) { return global_clone_functions_register().at(id)(); }
};

template <typename Base, typename Derived>
    requires ConcretePolymorphicSerializable<Derived>
class inheritance_relation final : public inheritance_relation_base<Base>
{
    using clone_functions_register = typename inheritance_relation_base<Base>::clone_functions_register;

    struct dummy final
    {
    };
    ~inheritance_relation() = delete;

    static Base* clone() { return new Derived(); }

    inline static dummy register_base_derived_relation_()
    {
        if constexpr (std::is_same_v<Base, Derived>)
            ConcretePolymorphicSerializables::register_type<Derived>();

        clone_functions_register& functions_register =
            inheritance_relation_base<Base>::global_clone_functions_register();
        auto emplace_res = functions_register.try_emplace(serializable_type_id<Derived>(), &clone);
        if (!emplace_res.second)
        {
            std::ostringstream stream;
            stream << "Serializable type id is already associated with a type: " << serializable_type_id<Derived>()
                   << ".";
            throw std::runtime_error(stream.str());
        }

        return dummy{};
    }
    inline static dummy auto_init_ = register_base_derived_relation_();
};

// Helper macros:

#define ARBA_SERI_DEFINE_SERIALIZABLE_TYPE_ID(type_, id_)                                                              \
    template <>                                                                                                        \
    inline constexpr bool ::arba::seri::has_serializable_type_id_v<type_> = true;                                      \
    template <>                                                                                                        \
    const ::arba::seri::uutid& ::arba::seri::serializable_type_id<type_>()                                             \
    {                                                                                                                  \
        static const ::arba::seri::uutid uid(id_);                                                                     \
        return uid;                                                                                                    \
    }                                                                                                                  \
    template class ::arba::seri::inheritance_relation<type_, type_>

#define ARBA_SERI_REGISTER_INHERITANCE_RELATION(base, derived)                                                         \
    template class ::arba::seri::inheritance_relation<base, derived>

#ifndef SERI_DEFINE_SERIALIZABLE_TYPE_ID
#define SERI_DEFINE_SERIALIZABLE_TYPE_ID(type_, id_) ARBA_SERI_DEFINE_SERIALIZABLE_TYPE_ID(type_, id_)
#elif not defined(NDEBUG) && (defined(__GNUC__) || defined(__GNUG__) || defined(_MSC_VER) || defined(__clang__))
#pragma message "SERI_DEFINE_SERIALIZABLE_TYPE_ID already exists. You must use ARBA_SERI_DEFINE_SERIALIZABLE_TYPE_ID."
#endif

#ifndef SERI_REGISTER_INHERITANCE_RELATION
#define SERI_REGISTER_INHERITANCE_RELATION(base_, derived_) ARBA_SERI_REGISTER_INHERITANCE_RELATION(base_, derived_)
#elif not defined(NDEBUG) && (defined(__GNUC__) || defined(__GNUG__) || defined(_MSC_VER) || defined(__clang__))
#pragma message                                                                                                        \
    "SERI_REGISTER_INHERITANCE_RELATION already exists. You must use ARBA_SERI_REGISTER_INHERITANCE_RELATION."
#endif

// Helper make functions:

template <typename Base>
Base* make_instance(const uutid& id)
{
    return inheritance_relation_base<Base>::make_instance(id);
}

template <typename Base>
std::unique_ptr<Base> make_unique(const uutid& id)
{
    return std::unique_ptr<Base>(make_instance<Base>(id));
}

template <typename Base>
std::shared_ptr<Base> make_shared(const uutid& id)
{
    return std::shared_ptr<Base>(make_instance<Base>(id));
}

} // namespace seri
} // namespace arba
