#ifndef CAVER_MANABAR_H
#define CAVER_MANABAR_H

#include <boost/shared_ptr.hpp>

#include "GUIView.h"
#include "GUISlider.h"

namespace Caver {

class ManaBar : public GUIView {
public:
    ManaBar();
    ~ManaBar() override;

    void LayoutSubviews() override;

    void SetMaxMana(int maxMana);
    void SetCurrentMana(int currentMana);

    boost::shared_ptr<GUISlider> slider() const { return m_slider; }
    int maxMana() const { return m_maxMana; }
    int currentMana() const { return m_currentMana; }

private:
    boost::shared_ptr<GUISlider> m_slider; // +256
    int m_maxMana;                         // +272
    int m_currentMana;                     // +276
};

} // namespace Caver

#endif // CAVER_MANABAR_H
