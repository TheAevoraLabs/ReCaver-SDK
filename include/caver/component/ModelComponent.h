// Recovered from libswordigo.so (1.4.13) evidence.
// A Component (104 bytes) plus its own members (offsets proven from the
// ctor store map @0x332818, Load/Save @0x3330A8/0x333200, Draw
// @0x333E2C, WorldMatrix @0x334240 and the FlashModel writers in
// EntityComponent / BreakableObjectComponent):
//   +104 std::string model name (binding 0; proto extension field 1)
//   +128 boost::shared_ptr<const Model> (loaded lazily by name)
//   +144 ModelInstance* (raw owning pointer, manual new/delete)
//   +152 intrusive_ptr to the sibling TransformComponent (cached by
//        Prepare; stored through the Component base)
//   +160 float emission factor (binding 4)
//   +164 bool transparent (binding 6)
//   +168 FloatColor diffuse colour, ctor {1,1,1,1} (binding 7)
//   +184 FloatColor (name/writer UNRECOVERED — Draw multiplies it
//        component-wise with the diffuse colour; the ctor provably
//        does not initialise it)
//   +200 bool: origin (below) is non-zero (!IsClose(origin, 0, 0.001))
//   +204 Vector3 origin (binding 1; proto shatter_color field 5)
//   +216 float x rotation, radians (binding 2)
//   +220 float y rotation, radians (binding 3)
//   +224 Matrix4 (name/writer UNRECOVERED — WorldMatrix multiplies it
//        in when the flag at +288 is set; ctor does not initialise it)
//   +288 bool: the +224 matrix is set
//   +292 Rectangle local AABB (maintained by UpdateBounds)
//   +308 float min depth / +312 float max depth (UpdateBounds)
//   +316 bool (Draw early-out when set; name UNRECOVERED)
//   +317 bool flash active (set by FlashModel, cleared by Entity update)
//   +318 Color flash colour, ctor 0xFFFFFFFF
//   +322 Color main colour, ctor 0xFFFFFFFF (binding 5; proto origin)
//   +326 bool (name UNRECOVERED)
// Object size 0x148 (see Create()'s operator new(0x148)).
#ifndef CAVER_MODELCOMPONENT_H
#define CAVER_MODELCOMPONENT_H

#include "Component.h"
#include "../core/Color.h"
#include "../core/FloatColor.h"
#include "../core/Matrix4.h"
#include "../core/Rectangle.h"
#include "../core/Vector3.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace Caver {

class Model;
class ModelInstance;
class RenderingContext;

class ModelComponent : public Component {
public:
    ModelComponent();
    virtual ~ModelComponent();

    static ModelComponent* Create();
    static long Interface();

    Component* Clone() const override;
    bool ShouldSave() override;
    void Prepare() override;
    int componentCategories() const override;
    bool ImplementsInterface(long interfaceId) override;
    void RegisterInterfaces(ComponentManager* manager) override;
    bool HasBounds() const override;
    Rectangle localAABB() const override;
    // Golden vtable slots 29/30 return the float members at
    // +308/+312 in s0; Component.h now declares float too, so
    // these are plain member definitions (the asm-labelled float
    // stand-ins they replace trapped on wasm: a float-returning
    // body behind a double-declared slot fails wasm's indirect
    // call type check at the first bounded ModelComponent).
    float minDepth() const override;
    float maxDepth() const override;
    bool IsVisualComponent() const override;
    std::string className() const override;

    // NOT an override: the vtable slot 3 keeps the base function (the
    // parameter is narrowed to const ModelComponent*, cf. HealthComponent).
    void InitWithComponent(const ModelComponent* other);

    void LoadFromProtobufMessage(const Proto::Component& message) override;
    void SaveToProtobufMessage(Proto::Component* message) const override;

    void GetBindings(std::vector<Binding>* bindings) override;
    BindingValue ValueForBindedProperty(int property) override;
    void SetValueForBindedProperty(int property,
                                   const BindingValue& value) override;

    void InitWithModel(const boost::shared_ptr<const Model>& model);
    void setModelName(const std::string& name);
    void LoadModel();
    void UpdateBounds();
    void Draw(RenderingContext* context, const Matrix4& transform,
              bool flag) const;
    Matrix4 WorldMatrix() const;

    std::string m_modelName;                            // +104
    boost::shared_ptr<const Model> m_model;             // +128
    boost::scoped_ptr<ModelInstance> m_modelInstance;   // +144 (proven
        // by the null-then-delete reset dances in setModelName /
        // LoadModel / InitWithModel and the dtor teardown order)
    boost::intrusive_ptr<Component> m_transformComponent; // +152
    float m_emissionFactor;                             // +160
    bool m_transparent;                                 // +164
    unsigned char m_pad165[3];
    FloatColor m_diffuseColor;                          // +168
    FloatColor m_field184;                              // +184
    bool m_originNonZero;                               // +200
    unsigned char m_pad201[3];
    Vector3 m_origin;                                   // +204
    float m_xRotation;                                  // +216
    float m_yRotation;                                  // +220
    Matrix4 m_field224;                                 // +224
    bool m_hasCustomMatrix;                             // +288
    unsigned char m_pad289[3];
    Rectangle m_localAABB;                              // +292
    float m_minDepth;                                   // +308
    float m_maxDepth;                                   // +312
    bool m_field316;                                    // +316
    bool m_flashActive;                                 // +317
    Color m_flashColor;                                 // +318
    Color m_mainColor;                                  // +322
    bool m_field326;                                    // +326
    unsigned char m_pad327[1];
};

} // namespace Caver

#endif
