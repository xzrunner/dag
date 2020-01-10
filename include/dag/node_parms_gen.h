#ifndef PARM_FILEPATH
#error "You must define PARM_FILEPATH macro before include this file"
#endif

#define Bool   bool
#define Int    int
#define Int2   sm::ivec2
#define Int3   sm::ivec3
#define Int4   sm::ivec4
#define Float  float
#define Float2 sm::vec2
#define Float3 sm::vec3
#define Float4 sm::vec4
#define String std::string
#define Array  std::vector
#define Set    std::set

// names
public:
static const constexpr char* const ParmNames[] = {
#define PARAM_INFO(id, type, name, member, default_val) \
    #name,
#include PARM_FILEPATH
#undef  PARAM_INFO
    ""
};

// type enums
public:
enum class Parm
{
#define PARAM_INFO(id, type, name, member, default_val) \
    id,
#include PARM_FILEPATH
#undef  PARAM_INFO
};

// setter & getter
public:
#define PARAM_INFO(id, type, name, member, default_val) \
    void Set##id(const type& _##name)                   \
    {                                                   \
        if (member != _##name) {                        \
            member = _##name;                           \
            SetDirty(true);                             \
        }                                               \
    }                                                   \
    auto& Get##id() const                               \
    {                                                   \
        return member;                                  \
    }
#include PARM_FILEPATH
#undef  PARAM_INFO

// member variables
private:
#define PARAM_INFO(id, type, name, member, default_val) \
    type member = type##default_val;
#include PARM_FILEPATH
#undef  PARAM_INFO

#undef Bool
#undef Int
#undef Int2
#undef Int3
#undef Int4
#undef Float
#undef Float2
#undef Float3
#undef Float4
#undef String
#undef Array
#undef Set
