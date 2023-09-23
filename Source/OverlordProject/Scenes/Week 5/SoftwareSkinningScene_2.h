#pragma once
class BoneObject;

class SoftwareSkinningScene_2 : public GameScene
{
public:
	SoftwareSkinningScene_2() :GameScene(L"SoftwareSkinningScene_2") {}
	~SoftwareSkinningScene_2() override = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnGUI() override;

private:
	bool m_AutoRotate{ true };
	float m_BoneRotation{ .0f };
	int m_RotationSign{ 1 };

	BoneObject* m_pBone0{ nullptr };
	BoneObject* m_pBone1{ nullptr };

	XMFLOAT3 m_RotBone0{ .0f, .0f, .0f };
	XMFLOAT3 m_RotBone1{ .0f, .0f, .0f };

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color) :
			transformedVertex{ position, normal, color },
			originalVertex{ position, normal, color } {}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};
