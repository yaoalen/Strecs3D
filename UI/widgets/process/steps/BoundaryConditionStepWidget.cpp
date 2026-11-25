#include "BoundaryConditionStepWidget.h"
#include "../../Button.h"
#include <QVBoxLayout>

#include "../../../core/ui/UIState.h"

BoundaryConditionStepWidget::BoundaryConditionStepWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Add Buttons
    
    m_addConstraintButton = new Button("新增約束條件", this);
    connect(m_addConstraintButton, &Button::clicked, this, &BoundaryConditionStepWidget::addConstraintClicked);
    
    m_addLoadButton = new Button("新增載荷條件", this);
    connect(m_addLoadButton, &Button::clicked, this, &BoundaryConditionStepWidget::addLoadClicked);

    layout->addWidget(m_addConstraintButton);
    layout->addWidget(m_addLoadButton);
    
    // Select Bed Surface Button
    m_selectBedSurfaceButton = new Button("選取列印底面", this);
    connect(m_selectBedSurfaceButton, &Button::clicked, this, &BoundaryConditionStepWidget::selectBedSurfaceClicked);
    layout->addWidget(m_selectBedSurfaceButton);
    
    layout->addSpacing(20); // Small gap
    
    // Done Button
    m_doneButton = new Button("套用模擬設定", this);
    m_doneButton->setEmphasized(true);
    m_doneButton->setEnabled(false); // Initially disabled
    connect(m_doneButton, &Button::clicked, this, &BoundaryConditionStepWidget::doneClicked);
    
    layout->addWidget(m_doneButton);
    layout->addStretch();
}

void BoundaryConditionStepWidget::setUIState(UIState* uiState)
{
    m_uiState = uiState;
    if (m_uiState) {
        connect(m_uiState, &UIState::boundaryConditionChanged, 
                this, &BoundaryConditionStepWidget::updateDoneButtonState);
        updateDoneButtonState();
    }
}

void BoundaryConditionStepWidget::updateDoneButtonState()
{
    if (!m_uiState) return;

    BoundaryCondition bc = m_uiState->getBoundaryCondition();
    
    bool hasValidConstraint = false;
    for (const auto& constraint : bc.constraints) {
        if (constraint.surface_id != 0) {
            hasValidConstraint = true;
            break;
        }
    }

    bool hasValidLoad = false;
    for (const auto& load : bc.loads) {
        if (load.surface_id != 0) {
            hasValidLoad = true;
            break;
        }
    }

    m_doneButton->setEnabled(hasValidConstraint && hasValidLoad);
}
