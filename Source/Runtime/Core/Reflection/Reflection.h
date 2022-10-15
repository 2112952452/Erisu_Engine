//
// Created by Adarion on 2022/10/9.
//

#ifndef ERISU_ENGINE_REFLECTION_H
#define ERISU_ENGINE_REFLECTION_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Log/LogSystem.h"
#include "nlohmann/json.hpp"

namespace Erisu::Core::Reflection
{

struct ReflField
{
    std::string_view name;
    std::string_view type;
    int offset;
    int size;
};

template<typename T>
struct ReflInfo {};


class ReflectionManager
{
private:
    inline static std::unordered_map<std::string_view, std::vector<ReflField>> refl_info {};

    using json = nlohmann::json;
public:

    template<typename T>
    static constexpr void RegClass()
    {
        refl_info[ReflInfo<T>::GetName()] = ReflInfo<T>::GetFields();
    }

    template<typename T>
    static constexpr std::vector<ReflField> GetFields()
    {
        return refl_info[ReflInfo<T>::GetName()];
    }

    static std::vector<ReflField> GetFields(std::string_view name)
    {
        return refl_info.at(name);
    }

    static ReflField GetField(std::string_view name, std::string_view field_name)
    {
        auto fields = refl_info.at(name);
        for (auto& field : fields)
        {
            if (field.name == field_name)
            {
                return field;
            }
        }
        LOG_FATAL("Field {} not found in class {}", field_name, name);
    }
};

#define ReflClassBase(classname) \
namespace Erisu::Core::Reflection {                                 \
template<> \
    struct ReflInfo<classname> \
    { \
        inline static std::string_view name = #classname; \
        inline static std::vector<ReflField> fields = {}; \
        \
        static constexpr std::string_view GetName() { return name; } \
        static constexpr std::vector<ReflField> GetFields() { return fields; } \
        static const ReflField& GetField(std::string_view name_){ return *std::find_if(fields.begin(), fields.end(), [&name_](const ReflField& field) { return field.name == name_; }); }                    \
        \
        static constexpr void RegisterField(std::string_view name_, std::string_view type, int offset, int size) \
        { \
            fields.push_back({name_, type, offset, size}); \
        }                    \
        static constexpr void RegisterAllFields();        \
                             \
        template<class ...Args>                        \
        static classname* CreateInstance(Args &&...args) {         \
            return new classname(std::forward<Args>(args)...);    \
        }\
    }; }

#define ReflClass(classname) \
    class classname;    \
    ReflClassBase(classname) \
    class classname {        \
        friend struct Erisu::Core::Reflection::ReflInfo<classname>;

#define ReflNameSpaceClass(classname, Namespace) \
    namespace Namespace { \
    class classname; }\
    ReflClassBase(Namespace::classname)    \
    namespace Namespace { \
    class classname {        \
        friend struct Erisu::Core::Reflection::ReflInfo<classname>;

#define ReflDerivedClass(classname, Derive) \
    class classname;    \
    ReflClassBase(classname) \
    class classname : Derive {        \
        friend struct Erisu::Core::Reflection::ReflInfo<classname>;

#define ReflNameSpaceDerivedClass(classname, Derive, Namespace) \
    namespace Namespace { \
    class classname; }\
    ReflClassBase(Namespace::classname)    \
    namespace Namespace { \
    class classname : Derive {        \
        friend struct Erisu::Core::Reflection::ReflInfo<Namespace::classname>;

#define ReflTemplateClass(classname) \
    template<typename T> \
    class classname;    \
    ReflClassBase(classname<T>) \
    template<typename T>\
    class classname


#define ReflField(class_type, field_name, type) \
    Erisu::Core::Reflection::ReflInfo<class_type>::RegisterField(#field_name, #type, offsetof(class_type, field_name), sizeof(type));

#define ReflFieldBegin \
    private:            \
    static constexpr void RegisterFields() {

#define ReflFieldEnd } \


#define ReflClassEnd(classname) }; \
    constexpr void Erisu::Core::Reflection::ReflInfo<classname>::RegisterAllFields() { \
        classname::RegisterFields(); \
        Erisu::Core::Reflection::ReflectionManager::RegClass<classname>();   \
    }

#define ReflNamespaceClassEnd(classname, Namespace) }; } \
    constexpr void Erisu::Core::Reflection::ReflInfo<Namespace::classname>::RegisterAllFields() { \
        Namespace::classname::RegisterFields(); \
        Erisu::Core::Reflection::ReflectionManager::RegClass<Namespace::classname>();   \
    }

}
#endif //ERISU_ENGINE_REFLECTION_H
