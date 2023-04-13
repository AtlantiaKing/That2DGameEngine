#pragma once

#include <functional>

namespace that
{
    class Event
    {
    public:
        Event() = default;
        Event(const char* name) : m_Name{ name } {}
        virtual ~Event() = default;

        bool IsValid() const { return *m_Name != '\0'; }
        const char* GetName() const { return m_Name; }

        bool operator==(const Event& other) const {
            return m_Name == other.m_Name;
        }
    private:
        const char* m_Name{};
    };
}

namespace std {
    template <>
    struct hash<that::Event>
    {
        std::size_t operator()(const that::Event& event) const
        {
            return std::hash<const char*>()(event.GetName());
        }
    };
}
