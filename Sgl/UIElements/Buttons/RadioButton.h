#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class RadioButton : public ToggleButton
	{
	public:
		RadioButton();
		RadioButton(const RadioButton&) = delete;
		RadioButton(RadioButton&& other) noexcept;

		void SetGroupName(const std::string& value, ValueSource source = ValueSource::Local);
		const std::string& GetGroupName() const { return _groupName; }

		static inline StyleableProperty GroupNameProperty { &SetGroupName, &GetGroupName };
	protected:
		void OnCheckedChanged() final;
		void OnClick() final;
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
	private:
		std::string _groupName;
		ValueSource _groupNameValueSource {};

		friend class RadioButtonGrouopRegistry;
	};
}