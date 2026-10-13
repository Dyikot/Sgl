#include "RadioButton.h"
#include "../../Styling/StyleContext.h"

namespace Sgl::UIElements
{
	class RadioButtonGroup
	{
	public:
		RadioButtonGroup() = default;

		void Add(RadioButton* button)
		{
			_buttons.push_back(button);
		}

		void Remove(RadioButton* button)
		{
			std::erase(_buttons, button);
		}
		
		void NotifyCheckedChanged(RadioButton* changedButton)
		{
			for(auto button : _buttons)
			{
				if(button != changedButton)
				{
					button->Uncheck();
				}
			}
		}

		bool Empty() const noexcept
		{
			return _buttons.empty();
		}
	private:
		std::vector<RadioButton*> _buttons;
	};

	class RadioButtonGroupRegistry
	{
	public:
		static RadioButtonGroupRegistry& Instance()
		{
			static RadioButtonGroupRegistry registry;
			return registry;
		}

		void Add(RadioButton* button)
		{
			auto& groupName = button->GetGroupName();

			if(groupName.empty())
			{
				return;
			}

			_groups[groupName].Add(button);
		}

		void Remove(RadioButton* button)
		{
			auto& groupName = button->GetGroupName();

			if(auto it = _groups.find(groupName); it != _groups.end())
			{
				auto& group = it->second;
				group.Remove(button);

				if(group.Empty())
				{
					_groups.erase(groupName);
				}
			}
		}

		void NotifyCheckedChanged(RadioButton* button)
		{
			auto& groupName = button->GetGroupName();
			_groups[groupName].NotifyCheckedChanged(button);
		}
	private:
		std::unordered_map<std::string, RadioButtonGroup> _groups;
	};

	RadioButton::RadioButton()
	{
		SetName("RadioButton");
	}

	void RadioButton::SetGroupName(const std::string& value)
	{
		auto source = GetValueSource();

		if(_groupNameValueSource > source)
		{
			return;
		}

		if(_groupName == value)
		{
			if(source < ValueSource::VisualState)
			{
				_groupNameValueSource = source;
			}

			return;
		}

		auto& registry = RadioButtonGroupRegistry::Instance();

		if(!_groupName.empty() && IsAttachedToLogicalTree())
		{
			registry.Remove(this);
		}

		_groupName = value;
		_groupNameValueSource = source;

		if(!_groupName.empty() && IsAttachedToLogicalTree())
		{
			registry.Add(this);
		}

		OnPropertyChanged(GroupNameProperty);
	}

	Ref<Style> RadioButton::CreateDefaultStyle() const
	{
		return New<RadioButtonStyle>();
	}

	void RadioButton::OnIsCheckedChanged()
	{
		ToggleButton::OnIsCheckedChanged();

		if(IsChecked())
		{
			RadioButtonGroupRegistry::Instance().NotifyCheckedChanged(this);
		}
	}

	void RadioButton::OnClick()
	{
		Button::OnClick();

		if(!IsChecked())
		{
			Check();
		}
	}

	void RadioButton::OnAttachedToLogicalTree()
	{
		ToggleButton::OnAttachedToLogicalTree();
		RadioButtonGroupRegistry::Instance().Add(this);
	}

	void RadioButton::OnDetachedFromLogicalTree()
	{
		ToggleButton::OnDetachedFromLogicalTree();
		RadioButtonGroupRegistry::Instance().Remove(this);
	}

	void RadioButtonStyle::OnBase(StyleContext& context)
	{
		ToggleButtonStyle::OnBase(context);
		context.Set(Button::WidthProperty, 24);
		context.Set(Button::HeightProperty, 24);
		context.Set(Button::BackgroundProperty, ImageSource(AssetId::RadioButtonOutline));
	}

	void RadioButtonStyle::OnChecked(StyleContext & context)
	{
		context.Set(Button::BackgroundProperty, ImageSource(AssetId::RadioButton));
	}
}