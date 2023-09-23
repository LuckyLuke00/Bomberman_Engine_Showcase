#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial() :
	Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{}

void SpikyMaterial::UpdateEffectVariables() const
{
	SetVariable_Vector(L"gColorDiffuse", m_ColorDiffuse);
	SetVariable_Scalar(L"gSpikeLength", m_SpikeLength);
}

void SpikyMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gColorDiffuse", m_ColorDiffuse);
	SetVariable_Scalar(L"gSpikeLength", m_SpikeLength);
}
