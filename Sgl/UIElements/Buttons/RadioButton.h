#pragma once

#include "ToggleButton.h"

namespace Sgl::UIElements
{
	class RadioButton : public ToggleButton
	{
	public:
		RadioButton();

		void SetGroupName(const std::string& value);
		const std::string& GetGroupName() const { return _groupName; }

		static inline StyleableProperty GroupNameProperty { &SetGroupName, &GetGroupName };
	protected:
		Ref<Style> CreateDefaultStyle() const override;
		void OnIsCheckedChanged() override;
		void OnClick() final;
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
	private:
		std::string _groupName;
		ValueSource _groupNameValueSource {};

		friend class RadioButtonGroup;
	};

	class RadioButtonStyle : public ToggleButtonStyle
	{
	public:
		void OnBase(StyleContext& context) override;
	protected:
		void OnChecked(StyleContext& context) override;
	};
}