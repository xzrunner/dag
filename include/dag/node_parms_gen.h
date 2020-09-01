#ifndef PARM_FILEPATH
#error "You must define PARM_FILEPATH macro before include this file"
#endif

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
#ifndef NOT_GEN_SETTER_GETTER_FUNC
public:
#define PARAM_INFO(id, type, name, member, default_val) \
    void Set##id(const type& _##name)                   \
    {                                                   \
        if (member != _##name) {                        \
            member = _##name;                           \
            OnParmChanged(#name);                       \
            SetDirty(true);                             \
        }                                               \
    }                                                   \
    auto& Get##id() const                               \
    {                                                   \
        return member;                                  \
    }
#include PARM_FILEPATH
#undef  PARAM_INFO
#endif // NOT_GEN_SETTER_GETTER_FUNC

// member variables
private:
#define PARAM_INFO(id, type, name, member, default_val) \
    type member = type##default_val;
#include PARM_FILEPATH
#undef  PARAM_INFO
