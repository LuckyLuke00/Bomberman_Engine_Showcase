#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active ? this : nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups) const
{
	const auto pScene{ GetScene() };
	const auto& sceneContext{ pScene->GetSceneContext() };

	const auto halfWidth{ sceneContext.windowWidth * .5f };
	const auto halfHeight{ sceneContext.windowHeight * .5f };

	const auto mousePos{ InputManager::GetMousePosition() };

	const auto ndcX{ (mousePos.x - halfWidth) / halfWidth };
	const auto ndcY{ (halfHeight - mousePos.y) / halfHeight };

	// Calculate near point and far point (and as such, the ray start and the direction)
	// The inverse of the ViewProjection matrix makes sure that the NearPoint and FarPoint are defined in world space
	const auto viewProjectionInv{ XMLoadFloat4x4(&GetViewProjectionInverse()) };

	const auto nearPoint{ XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, .0f, .0f), viewProjectionInv) };
	const auto farPoint{ XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 1.f, .0f), viewProjectionInv) };

	const PxVec3 rayStart{ XMVectorGetX(nearPoint), XMVectorGetY(nearPoint), XMVectorGetZ(nearPoint) };
	const PxVec3 rayDir{ PxVec3{ XMVectorGetX(farPoint - nearPoint), XMVectorGetY(farPoint - nearPoint), XMVectorGetZ(farPoint - nearPoint) }.getNormalized() };

	// Use PhysX to do a raycast
	// The PhysxProxy already contains code to do a raycast.
	PxQueryFilterData filterData{};
	filterData.data.word0 = ~UINT(ignoreGroups);

	if (PxRaycastBuffer hit{}; pScene->GetPhysxProxy()->Raycast(rayStart, rayDir, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		// If the raycast hits something, return the GameObject that was hit
		// return reinterpret_cast<BaseComponent*>(hit.block.actor->userData)->GetGameObject();
		return static_cast<BaseComponent*>(hit.getAnyHit(0).actor->userData)->GetGameObject();
	}

	return nullptr;
}
