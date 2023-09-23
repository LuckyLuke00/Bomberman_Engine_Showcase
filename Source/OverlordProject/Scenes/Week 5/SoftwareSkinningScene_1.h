#pragma once
class BoneObject;

class SoftwareSkinningScene_1 : public GameScene
{
public:
	SoftwareSkinningScene_1() :GameScene(L"SoftwareSkinningScene_1") {}
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

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
};
