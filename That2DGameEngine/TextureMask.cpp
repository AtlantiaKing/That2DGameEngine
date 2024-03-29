#include "TextureMask.h"

#include "RenderComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include "Timer.h"

void that::TextureMask::SetPercentage(bool x, float percentage)
{
    if (x) m_XMask = percentage;
    else m_YMask = percentage;
}

glm::vec2 that::TextureMask::GetMask() const
{
    return glm::vec2{ m_XMask, m_YMask };
}

void that::TextureMask::Init()
{
    m_pTexture = GetOwner()->GetComponent<RenderComponent>();
    if (m_pTexture == nullptr) Logger::LogError("GameObject has TextureMask but no RenderComponent");
}

void that::TextureMask::Update()
{
    auto& srcRect{ m_pTexture->GetSource() };
    const auto& textureSize{ m_pTexture->GetTextureSize() };

    srcRect.w = static_cast<int>(m_XMask * textureSize.x);
    srcRect.h = static_cast<int>(m_YMask * textureSize.y);
}
