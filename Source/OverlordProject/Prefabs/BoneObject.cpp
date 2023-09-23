#include "stdafx.h"
#include "BoneObject.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length) :
	m_Length{ length },
	m_pMaterial{ pMaterial }
{
}

void BoneObject::AddBone(BoneObject* pBone)
{
	// The incoming bone (pBone) must be translated along the X - axis so it is located next
	// to the parent(this) bone.The length of the parent bone is defined by m_Length
	pBone->GetTransform()->Translate(GetTransform()->GetWorldPosition().x + m_Length, .0f, .0f);

	// Add pBone as a child to the parent BoneObject(this).This creates the hierarchical relation between both bone,
	// pBone is now a child and ‘follows’ the transformation of its parent.
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	// Here we want to calculate the BindPose of the bone and all its children. The
	// BindPose is simply the inverse of its World Matrix, store this in
	// m_BindPose.This function will be called once, for every root bone – it’s important that the bone is
	// aligned correctly with the mesh before the BindPose is calculated and set.
	XMStoreFloat4x4(&m_BindPose, XMMatrixInverse(nullptr, XMLoadFloat4x4(&GetTransform()->GetWorld())));

	// Loop through all children of the BoneObject(this)
	for (const auto& pChild : GetChildren<BoneObject>())
	{
		pChild->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	// Create an empty GameObject
	const auto pEmpty{ new GameObject() };

	// Add pEmpty as a child to the BoneObject
	AddChild(pEmpty);

	// Add a ModelComponent to pEmpty (pModel), use Bone.ovm
	const auto pModel{ new ModelComponent(L"./Meshes/Bone.ovm") };
	pEmpty->AddComponent(pModel);

	// Assign the BoneObject’s material (m_pMaterial) to pModel
	pModel->SetMaterial(m_pMaterial);

	// Rotate pEmpty - 90 degrees around the Y - axis(This transform will orient the bone along the X - axis)
	pEmpty->GetTransform()->Rotate(.0f, -90.f, .0f);

	// Uniformly scale pEmpty to match the BoneObject’s length(m_Length)
	pEmpty->GetTransform()->Scale(m_Length);
}
