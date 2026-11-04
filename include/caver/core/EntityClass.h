#ifndef CAVER_ENTITYCLASS_H
#define CAVER_ENTITYCLASS_H

#include <string>

namespace Caver {

namespace Proto { class EntityClass; }

class EntityClass {
public:
    EntityClass();
    void LoadFromProtobufMessage(const Proto::EntityClass& message);
    void SaveToProtobufMessage(Proto::EntityClass* message) const;

    std::string m_name;               // +0
    std::string m_title;              // +24
    bool m_levelHidden;               // +48
    bool m_freezable;                 // +49
    bool m_stunnable;                 // +50
    bool m_grabbable;                 // +51
    float m_magicResistance;          // +52
    float m_physicalResistance;       // +56
    int _pad60;                       // +60
};

#ifndef _MSC_VER
static_assert(sizeof(EntityClass) == 0x40, "EntityClass sizeof must be 0x40");
#endif

} // namespace Caver

#endif // CAVER_ENTITYCLASS_H
