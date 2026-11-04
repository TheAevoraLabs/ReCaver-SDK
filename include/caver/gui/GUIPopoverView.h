// Recovered from libswordigo.so (1.4.13) evidence.
//
// GUIPopoverView: a popover bubble — a GUIRoundedRect border, a
// GUITexturedRect arrow, an optional dimmed backdrop and a content
// view positioned relative to an anchor rectangle.
// Vtable _ZTVN5Caver14GUIPopoverViewE @0x635970, 320 bytes = the 38
// GUIView slots with slot 2 (TouchEnded), slot 7/8 (~GUIPopoverView),
// slot 10 (DrawRect), slot 17 (ContainingPopoverView) and slot 18
// (DismissPopoverView) overridden (ABS64 relocation dump). RTTI
// @0x635AB0 is a __si_class_type_info whose single base is GUIView.
//
// Layout: GUIView (256 bytes) with two fields punched into the
// GUIView tail region (proven by the ctor @0x52D210 stores and by
// PresentFromRectangle @0x52DC34 / UpdateLayout @0x52D790 reads):
//   +249 bool presented (the GUIView colour dword's low byte; set by
//        PresentFromRectangle, gates DrawRect and the relayout in
//        SetPopoverContentView; member name UNRECOVERED)
//   +252 Rectangle anchorRect (16 bytes spanning +252..+267, written
//        as a single OWORD store by PresentFromRectangle)
// then the class's own members:
//   +272 boost::shared_ptr<GUIView> contentView
//   +288 unsigned int permittedArrowDirections (bit flags 1/2/4/8,
//        set by PresentFromRectangle)
//   +292 int arrowDirection (the direction UpdateLayout picked;
//        member name UNRECOVERED)
//   +296 Vector2 arrowPosition (member name UNRECOVERED)
//   +304 float arrowRotation (radians: 0, +pi/2, pi, -pi/2 by
//        direction — written by UpdateLayout as float bit patterns)
//   +312 boost::shared_ptr<GUIRoundedRect> roundedRect ("ui_popover_border")
//   +328 boost::shared_ptr<GUITexturedRect> arrowView ("ui_popover_arrow")
//   +344 bool field344 (gates the backdrop FillRect in DrawRect;
//        member name/purpose UNRECOVERED)
//   +345 Color backdropColor (member name UNRECOVERED)
// sizeof = 352.
#ifndef CAVER_GUIPOPOVER_H
#define CAVER_GUIPOPOVER_H

#include <boost/shared_ptr.hpp>

#include "../core/Color.h"
#include "../core/Rectangle.h"
#include "../core/Vector2.h"
#include "GUITexturedRect.h"
#include "GUIView.h"

namespace Caver {

class GUIRoundedRect;

class GUIPopoverView : public GUIView {
public:
    GUIPopoverView();
    ~GUIPopoverView();

    void TouchEnded(const FWTouch& touch) override;  // slot 2
    void DrawRect(RenderingContext* context, const Rectangle& rect,
                  const Matrix4& transform) override;  // slot 10
    GUIPopoverView* ContainingPopoverView() override;  // slot 17
    void DismissPopoverView() override;                // slot 18

    void SetPopoverContentView(const boost::shared_ptr<GUIView>& view);
    void UpdateLayout();
    void PresentFromRectangle(const Rectangle& rect,
                              unsigned int permittedArrowDirections);
    Rectangle AdjustContentFrameVertically(Rectangle rect);

    char _anchorTail[16];  // +256: anchorRect's y/width/height at
                           // +256/+260/+264 and 4 unrecovered bytes
                           // at +268 (see header note)
    boost::shared_ptr<GUIView> contentView;             // +272
    unsigned int permittedArrowDirections;              // +288
    int arrowDirection;                                 // +292 UNRECOVERED name
    Vector2 arrowPosition;                              // +296 UNRECOVERED name
    float arrowRotation;                                // +304
    char _pad308[4];                                    // +308
    boost::shared_ptr<GUIRoundedRect> roundedRect;      // +312
    boost::shared_ptr<GUITexturedRect> arrowView;       // +328
    bool field344;                                      // +344 UNRECOVERED
    Color backdropColor;                                // +345 UNRECOVERED name
};

} // namespace Caver

#endif
