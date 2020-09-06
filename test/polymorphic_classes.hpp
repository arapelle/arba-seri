#pragma once

#include <seri/archive/binary_archive.hpp>
#include <seri/binary/binary_io.hpp>
#include <string_view>

class Object
{
public:
    virtual ~Object() {}
    virtual std::string_view name() const = 0;
    virtual void write_binary(seri::binary_oarchive& archive) const = 0;
    virtual void read_binary(seri::binary_iarchive& archive) = 0;
    auto operator<=>(const Object&) const = default;
};

class Polygon : public Object
{
public:
    Polygon(unsigned number_of_edges = 0) : number_of_edges(number_of_edges) {}
    virtual ~Polygon() override {}
    virtual std::string_view name() const override { return "Polygon"; }
    virtual void write_binary(seri::binary_oarchive& archive) const override
    {
        seri::write_binary(archive, number_of_edges);
    }
    virtual void read_binary(seri::binary_iarchive& archive) override
    {
        seri::read_binary(archive, number_of_edges);
    }
    auto operator<=>(const Polygon&) const = default;

    unsigned number_of_edges;
};

SERI_DEFINE_TYPE_ID(Polygon, "2fbfe890-eefe-4b38-babc-6ca79931e131");

class Colored_polygon : public Polygon
{
public:
    Colored_polygon(unsigned number_of_edges = 0, std::string color = "black")
        : Polygon(number_of_edges), color(color)
    {}
    virtual ~Colored_polygon() override {}
    virtual std::string_view name() const override { return "Colored_polygon"; }
    virtual void write_binary(seri::binary_oarchive& archive) const override
    {
        Polygon::write_binary(archive);
        seri::write_binary(archive, color);
    }
    virtual void read_binary(seri::binary_iarchive& archive) override
    {
        Polygon::read_binary(archive);
        seri::read_binary(archive, color);
    }
    auto operator<=>(const Colored_polygon&) const = default;

    std::string color;
};

//void write_binary(seri::oarchive& archive, const Colored_polygon& value)
//{
////    Shape::write_binary(stream);
//    seri::write_binary(archive, value.size);
//}

//void read_binary(seri::iarchive& archive, Colored_polygon& value)
//{
////    Shape::write_binary(stream);
//    seri::read_binary(archive, value.size);
//}

SERI_DEFINE_TYPE_ID(Colored_polygon, "733f99fe-3881-4862-813a-5e3aab4a6957");
SERI_REGISTER_INHERITANCE_RELATION(Object, Colored_polygon)
SERI_REGISTER_INHERITANCE_RELATION(Polygon, Colored_polygon)
