#pragma once

#include <string>
#include "EventHandler.h"

namespace Sgl
{
	struct TextChangedEventArgs: EventArgs
	{
		std::string Text;
		size_t SelectionLength;
		int SelectionStart;
	};

	struct TextInputEventArgs: EventArgs
	{
		std::string Text;
	};

	class ITextEventsListener
	{
	public:
		virtual ~ITextEventsListener() = default;

		virtual void OnTextChanged(const TextChangedEventArgs& e) = 0;
		virtual void OnTextInput(const TextInputEventArgs& e) = 0;
	};

	using TextInputEventHandler = EventHandler<ITextEventsListener, TextInputEventArgs>;
	using TextChangedEventHandler = EventHandler<ITextEventsListener, TextChangedEventArgs>;
}