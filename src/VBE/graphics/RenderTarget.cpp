#include <VBE/graphics/RenderTarget.hpp>

RenderTarget::RenderTarget(unsigned int width, unsigned int height) : RenderTargetBase(width, height, 1) {
}

RenderTarget::RenderTarget(float mult) : RenderTargetBase(mult, 1) {
}

RenderTarget::~RenderTarget() {
}

void RenderTarget::setTexture(RenderTargetBase::Attachment a, Texture2D* tex) {
	if(entries.find(a) != entries.end()) entries.erase(a);
	entries.insert(std::pair<RenderTargetBase::Attachment, RenderTargetEntry>(a, RenderTargetEntry(tex)));
}

void RenderTarget::setBuffer(RenderTargetBase::Attachment a, RenderBuffer* buff) {
	if(entries.find(a) != entries.end()) entries.erase(a);
	entries.insert(std::pair<RenderTargetBase::Attachment, RenderTargetEntry>(a, RenderTargetEntry(buff)));
}

Texture2D* RenderTarget::getTexture(RenderTargetBase::Attachment a) {
	VBE_ASSERT(entries.find(a) != entries.end(), "No texture found for the provided attachment");
	VBE_ASSERT(entries.at(a).type == RenderTargetEntry::Texture2DEntry, "This attachment has a renderbuffer, not a texture");
	return entries.at(a).texture2D;
}
