//
// Created by plaka on 20.04.2023.
//

#include "Physical.h"
#include "game/Timer.h"

void CPhysical::ApplyTurnSpeed()
{
    if (physicalFlags.bDontApplySpeed) {
        ResetTurnSpeed();
    }
    else
    {
        CVector vecTurnSpeedTimeStep = CTimer::GetTimeStep() * m_vecTurnSpeed;
        CVector vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetRight());
        GetRight() += vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetForward());
        GetForward() += vecCrossProduct;
        CrossProduct(&vecCrossProduct, &vecTurnSpeedTimeStep, &GetUp());
        GetUp() += vecCrossProduct;
        if (!physicalFlags.bInfiniteMass && !physicalFlags.bDisableMoveForce) {
            CVector vecNegativeCentreOfMass = m_vecCentreOfMass * -1.0f;
            CVector vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), vecNegativeCentreOfMass);
            GetPosition() += CrossProduct(vecTurnSpeedTimeStep, vecCentreOfMassMultiplied);
        }
    }
}

void CPhysical::ApplyTurnForce(CVector force, CVector point)
{
    if (!physicalFlags.bDisableTurnForce)
    {
        CVector vecCentreOfMassMultiplied{};
        if (!physicalFlags.bInfiniteMass)
            vecCentreOfMassMultiplied = Multiply3x3(GetMatrix(), m_vecCentreOfMass);

        if (physicalFlags.bDisableMoveForce) {
            point.z = 0.0f;
            force.z = 0.0f;
        }
        CVector vecDifference = point - vecCentreOfMassMultiplied;
        m_vecTurnSpeed += CrossProduct(vecDifference, force) / m_fTurnMass;
    }
}

void CPhysical::ApplyMoveSpeed()
{
    if (physicalFlags.bDontApplySpeed || physicalFlags.bDisableMoveForce)
        ResetMoveSpeed();
    else
        GetPosition() += CTimer::GetTimeStep() * m_vecMoveSpeed;
}