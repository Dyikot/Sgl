#pragma once

#include <unordered_map>
#include <string_view>
#include "../Data/Any.h"
#include "../Data/Delegate.h"
#include "../Graphic/RenderContext.h"

namespace Sgl
{
	class ResourceMap: public std::unordered_map<std::string_view, Any>
	{
	private:
		RenderDependenciesFactory _renderDependenciesFactory;
	public:
		ResourceMap(RenderDependenciesFactory renderDependenciesFactory):
			_renderDependenciesFactory(renderDependenciesFactory)
		{}

		void Configure(Action<ResourceMap&, RenderDependenciesFactory> configurer)
		{
			configurer(*this, _renderDependenciesFactory);
		}
	};
}