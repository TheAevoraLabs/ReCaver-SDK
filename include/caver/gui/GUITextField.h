// Recovered from libswordigo.so (1.4.13) evidence — IDA/Hex-Rays exports
// in ~/workspace/analysis/swordigo/functions/Caver/GUITextField/, the
// ABS64 relocations of the golden vtable, and the golden disassembly of
// the four own member functions.
//
// GUITextField: the abstract text-field interface of the GUI layer —
// a GUIControl plus a lazily created GUIFrameView backdrop and a raw
// delegate pointer, with 20 pure virtual text/font/colour accessors
// that GUITextFieldImpl implements.
//
// MERGE NOTE (file placement): the class declaration itself currently
// lives in GUITextFieldImpl.h (written by the GUITextFieldImpl slice,
// which owns that file): GUIControl base, the _goldenTreeTail pad, the
// m_frameView / m_delegate members, the inline destructor and the
// slot 39-58 pure virtuals are all declared there, and
// IGUITextFieldDelegate / ITextInputDelegate live there too. This
// header is the canonical standalone include for the class and
// forwards to that declaration rather than duplicating it (a second
// definition would be an ODR violation). At merge time the class
// declaration should move into this header verbatim and
// GUITextFieldImpl.h should include it instead. The out-of-line /
// header-inline own member functions are defined in GUITextField.cpp
// (New, frameView) and by the inline destructor in the declaration
// (D2/D0 below).
//
// Vtable _ZTVN5Caver12GUITextFieldE @0x6364F8, 488 bytes = 16-byte
// header + 59 slots. RTTI _ZTIN5Caver12GUITextFieldE @0x636498 is a
// __si_class_type_info whose single base is GUIControl. Slots (ABS64
// relocations, golden targets):
//   0-5   GUIResponder TouchBegan/TouchMoved/TouchEnded/TouchCancelled/
//         HandleMouseEvent/HandleKeyboardEvent (not overridden)
//   6     GUIView::nextResponder
//   7/8   ~GUITextField D2 @0x5405B4 / D0 @0x540638
//   9     GUIView::Update            10 GUIView::DrawRect
//   11    GUIView::LayoutSubviews    12 GUIView::AddSubview
//   13    GUIView::InsertSubviewBelowSubview
//   14    GUIView::InsertSubviewAtIndex
//   15    GUIView::RemoveFromSuperview
//   16    GUIView::DidMoveToWindow   17 GUIView::ContainingPopoverView
//   18    GUIView::DismissPopoverView
//   19    GUIView::DragAndDropItemWithTouch
//   20-22 GUIView::DraggableItemDid{Enter,Leave,Drop}IntoView
//   23    GUIView::UndoManager
//   24    GUIView::CanBecomeFirstResponder
//   25    GUIView::BecomeFirstResponder
//   26    GUIView::ResignFirstResponder
//   27    GUIView::ScrollViewCanCancelTouchInContentView
//   28    GUIView::PointInside       29 GUIView::HitTest
//   30    GUIView::Window            31 GUIView::SetBounds
//   32-36 GUIView::Convert{Point,Rect}{To,From}Subview (+UsingFrame)
//   37    GUIView::SetOrthoProjection
//   38    GUIControl::StateDidChange (not overridden)
//   39-58 the GUITextField interface, every slot __cxa_pure_virtual
//         in this class: font/setFont, textColor/setTextColor,
//         text/setText, placeholder/setPlaceHolder,
//         horizontalTextAlignment/setHorizontalTextAlignment,
//         verticalTextAlignment/setVerticalTextAlignment,
//         textMargins/setTextMargins, textBounds, TextSize,
//         isMultiline/setMultiline, immediateUpdates/setImmediateUpdates
//         (exact order in the class declaration in GUITextFieldImpl.h).
//
// Layout (sizeof 312 = 0x138): GUIControl (GUIView 256 + the multimap
// at +256, whose golden libc++ __tree spans +256..+287 — see the
// _goldenTreeTail note in the declaration), then
//   +288 boost::shared_ptr<GUIFrameView> m_frameView (name
//        UNRECOVERED) — proven by frameView() @0x462F68 loading the
//        counted pointer at +288/+296 and by ~GUITextField releasing
//        the counted pointer at +296
//   +304 IGUITextFieldDelegate* m_delegate (name UNRECOVERED; raw) —
//        proven by GUITextFieldImpl::TextInputDidFinish /
//        ResignFirstResponder calling its slots 1 / 0
// No out-of-line constructor exists in the golden (no C1/C2 symbol):
// the constructor was implicit/inline in the original as well.
//
// Own member functions (the complete set in the golden dynsym):
//   New()       @0x52611C, 4 bytes — a bare tail branch to
//               NewGUITextFieldImpl() (defined in GUITextField.cpp)
//   frameView() @0x462F68, 480 bytes, WEAK in the golden (defined
//               inline in the original header; defined out-of-line in
//               GUITextField.cpp here — see the merge note there)
//   ~GUITextField D2 @0x5405B4, 132 bytes, WEAK — inline in the
//               declaration: sets the GUITextField vptr, releases the
//               m_frameView counted pointer at +296, tail-calls
//               ~GUIControl. Grades 132=132, insn 100%.
//   ~GUITextField D0 @0x540638, 4 bytes, WEAK — a single `brk #1`
//               trap in the golden; clang reproduces it exactly for
//               this abstract class (grades 4=4, 100/100). Deleting
//               through a GUITextField* was therefore a trap in the
//               original as well; concrete deletion goes through
//               GUITextFieldImpl's normal D0.
#ifndef CAVER_GUITEXTFIELD_H
#define CAVER_GUITEXTFIELD_H

// The class declaration (see the merge note above).
#include "GUITextFieldImpl.h"

#endif
