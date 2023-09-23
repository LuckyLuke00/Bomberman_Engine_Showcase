#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1.
		//Calculate the passedTicks (see the lab document)
		auto passedTicks{ sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed };
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, m_CurrentClip.duration);

		//2.
		//IF m_Reversed is true
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;

			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		//ELSE
		else
		{
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;

			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		for (const auto& key : m_CurrentClip.keys)
		{
			//keyA > Closest Key with Tick before/smaller than m_TickCount
			if (key.tick < m_TickCount)
			{
				keyA = key;
			}
			//keyB > Closest Key with Tick after/bigger than m_TickCount
			else if (key.tick > m_TickCount)
			{
				keyB = key;
				break;
			}
		}

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		const float blendFactor{ (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick) };

		//Clear the m_Transforms vector
		m_Transforms.clear();

		//FOR every boneTransform in a key (So for every bone)
		for (size_t i{ 0 }; i < keyA.boneTransforms.size(); ++i)
		{
			//	Retrieve the transform from keyA (transformA)
			const auto transformA{ keyA.boneTransforms[i] };
			// 	Retrieve the transform from keyB (transformB)
			const auto transformB{ keyB.boneTransforms[i] };

			//	Decompose both transforms
			XMVECTOR scaleA, rotA, transA;
			XMMatrixDecompose(&scaleA, &rotA, &transA, XMLoadFloat4x4(&transformA));

			XMVECTOR scaleB, rotB, transB;
			XMMatrixDecompose(&scaleB, &rotB, &transB, XMLoadFloat4x4(&transformB));
			//	Lerp between all the transformations (Position, Scale, Rotation)
			const auto lerpScale{ XMVectorLerp(scaleA, scaleB, blendFactor) };
			const auto lerpRot{ XMQuaternionSlerp(rotA, rotB, blendFactor) };
			const auto lerpPos{ XMVectorLerp(transA, transB, blendFactor) };

			//	Compose a transformation matrix with the lerp-results
			XMMATRIX transform{ XMMatrixTransformation({}, {}, lerpScale, {}, lerpRot, lerpPos) };
			//	Add the resulting matrix to the m_Transforms vector
			XMFLOAT4X4 finalTransform;
			XMStoreFloat4x4(&finalTransform, transform);
			m_Transforms.push_back(finalTransform);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (UINT i{ 0 }; i < GetClipCount(); ++i)
	{
		if (GetClip(i).name == clipName)
		{
			SetAnimation(GetClip(i));
			return;
		}

		Reset(false);
		Logger::LogWarning(L"ModelAnimator::SetAnimation(UINT) > clipNumber is out of range!");
	}
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;
	if (clipNumber < GetClipCount())
	{
		SetAnimation(GetClip(clipNumber));
	}
	else
	{
		Reset(false);
		Logger::LogWarning(L"ModelAnimator::SetAnimation(UINT) > clipNumber is out of range!");
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause) m_IsPlaying = false;

	//Set m_TickCount to zero
	m_TickCount = .0f;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.f;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		const auto boneTransform{ m_CurrentClip.keys[0].boneTransforms };
		m_Transforms.assign(boneTransform.begin(), boneTransform.end());
	}
	else
	{
		//	Create an IdentityMatrix
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		const XMFLOAT4X4 identity{ XMFLOAT4X4{} };
		m_Transforms.assign(m_pMeshFilter->m_BoneCount, identity);
	}
}
