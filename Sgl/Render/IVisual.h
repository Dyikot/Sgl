#pragma once

namespace Sgl
{
	class IVisual
	{
	public:
		virtual ~IVisual() = default;

		virtual void OnRender() const = 0;
	};
}