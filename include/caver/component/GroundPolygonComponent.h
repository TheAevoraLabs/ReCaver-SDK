#ifndef CAVER_GROUNDPOLYGONCOMPONENT_H
#define CAVER_GROUNDPOLYGONCOMPONENT_H

#include "Component.h"
#include "../physics/CollisionShape.h"
#include "../scripting/Program.h"

namespace Caver {

class GroundPolygonComponent : public Component {
public:
    GroundPolygonComponent();
    virtual ~GroundPolygonComponent();
    static GroundPolygonComponent* Create();
    static long Interface();
    virtual GroundPolygonComponent* Clone() const;
    virtual void InitWithComponent(const Component* other);
    virtual void LoadFromProtobufMessage(const Proto::Component& message);
    virtual void SaveToProtobufMessage(Proto::Component* message) const;
    virtual bool ShouldSave();
    virtual bool Process(std::vector<boost::intrusive_ptr<Component> >* components);
    virtual void SetDefaultValues();
    virtual void GetBindings(std::vector<Binding>* bindings);
    virtual void GetEnumValuesForBindedProperty(int property, std::vector<BindingValue>* values);
    virtual BindingValue ValueForBindedProperty(int property);
    virtual void SetValueForBindedProperty(int property, const BindingValue& value);
    virtual bool ImplementsInterface(long interfaceId);
    virtual void RegisterInterfaces(ComponentManager* manager);
    virtual bool HasBounds() const;
    virtual Rectangle localAABB() const;
    virtual float minDepth() const;
    virtual float maxDepth() const;
    virtual std::string className() const;

    // 0x3A9EA4: allocate a 0xF0-byte block, run the ctor and call its
    // SetDefaultValues() virtual (vtable +80, Component slot 10).
    static GroundPolygonComponent* NewBlock();
    void SetPolygon(const Polygon& polygon);
    void UpdateBounds();

    Polygon m_polygon;       // +104
    Rectangle m_localAABB;   // +144
    float m_minDepth;        // +160
    float m_maxDepth;        // +164
    bool m_collides;         // +168
    char _pad169[3];
    float m_friction;        // +172
    bool m_unsafeGround;     // +176
    char _pad177[7];
    Program m_onCollide;     // +184
};

// Golden aarch64 NDK-libc++ layout anchor; MSVC STL member sizes differ,
// so this assert is Android/Linux-only (the Windows build is self-consistent
// under MSVC STL and does not claim golden byte-layout parity).
#ifndef _MSC_VER
static_assert(sizeof(GroundPolygonComponent) == 0xF0,
              "GroundPolygonComponent size must be 240 bytes");
#endif

} // namespace Caver

#endif
