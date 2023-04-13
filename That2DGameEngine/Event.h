#pragma once

#include <functional>

namespace that
{
    struct Event
    {
        Event() = default;
        Event(const char* name) : name{ name } {}
        virtual ~Event() = default;

        bool IsValid() const { return *name != '\0'; }

        bool operator==(const Event& other) const {
            return name == other.name;
        }

        const char* name{};
    };
}

namespace std {
    template <>
    struct hash<that::Event>
    {
        std::size_t operator()(const that::Event& event) const
        {
            return std::hash<const char*>()(event.name);
        }
    };
}
