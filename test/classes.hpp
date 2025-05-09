#pragma once

#include <arba/seri/binary/io.hpp>

#include <string_view>

class Object
{
public:
    virtual ~Object() {}
    virtual std::string_view name() const = 0;
    virtual void write_binary(std::ostream& stream) const = 0;
    virtual void read_binary(std::istream& stream) = 0;
    auto operator<=>(const Object&) const = default;
};

class Polygon : public Object
{
public:
    Polygon(unsigned nb_edges = 0) : number_of_edges(nb_edges) {}
    virtual ~Polygon() override {}
    virtual std::string_view name() const override { return "Polygon"; }

    virtual void write_binary(std::ostream& stream) const override { seri::write_binary(stream, number_of_edges); }

    virtual void read_binary(std::istream& stream) override { seri::read_binary(stream, number_of_edges); }

    auto operator<=>(const Polygon&) const = default;

    unsigned number_of_edges;
};

ARBA_SERI_DEFINE_SERIALIZABLE_TYPE_ID(Polygon, "2fbfe890-eefe-4b38-babc-6ca79931e131");

class Colored_polygon : public Polygon
{
public:
    Colored_polygon(unsigned nb_edges = 0, std::string input_color = "black") : Polygon(nb_edges), color(input_color)
    {
    }
    virtual ~Colored_polygon() override {}
    virtual std::string_view name() const override { return "Colored_polygon"; }

    virtual void write_binary(std::ostream& stream) const override
    {
        Polygon::write_binary(stream);
        seri::write_binary(stream, color);
    }

    virtual void read_binary(std::istream& stream) override
    {
        Polygon::read_binary(stream);
        seri::read_binary(stream, color);
    }

    auto operator<=>(const Colored_polygon&) const = default;

    std::string color;
};

ARBA_SERI_DEFINE_SERIALIZABLE_TYPE_ID(Colored_polygon, "733f99fe-3881-4862-813a-5e3aab4a6957");
ARBA_SERI_REGISTER_INHERITANCE_RELATION(Object, Colored_polygon);
ARBA_SERI_REGISTER_INHERITANCE_RELATION(Polygon, Colored_polygon);

class Text
{
public:
    Text(const std::string& input_text = "") : text(input_text) {}

    void write_binary(std::ostream& stream) const { seri::write_binary(stream, text); }

    void read_binary(std::istream& stream) { seri::read_binary(stream, text); }

    auto operator<=>(const Text&) const = default;

    std::string text;
};
