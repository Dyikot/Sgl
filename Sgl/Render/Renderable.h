#pragma once

#include "../Styling/Styleable.h"
#include "../Input/Cursor.h"
#include "../Base/Media/Brush.h"
#include "IVisualRoot.h"
#include "RenderContext.h"

namespace Sgl
{
    class Renderable : public Styleable
    {
    public:
        Renderable() = default;

        void SetCursor(Cursor value, ValueSource source = ValueSource::Local);
        Cursor GetCursor() const;

        void SetBackground(const Brush& value, ValueSource source = ValueSource::Local);
        const Brush& GetBackground() const;

        IVisualRoot* GetVisualRoot() const { return _visualRoot; }
        
        virtual void Render(RenderContext& context) {}
        void InvalidateRender();

        static inline StyleableProperty CursorProperty { &SetCursor, &GetCursor };
        static inline StyleableProperty BackgroundProperty { &SetBackground, &GetBackground };
    protected:
        ~Renderable() = default;
        void OnPropertyChanged(PropertyBase& property) override;
        void SetVisualRoot(IVisualRoot* visualRoot);
        void OnAttachedToLogicalTree() override;
        void OnDetachedFromLogicalTree() override;
        virtual void OnCursorChanged(Cursor cursor) {}
        virtual void OnBackgroundChanged(const Brush& background) {}
    private:
        IVisualRoot* _visualRoot = nullptr;
        Cursor _cursor = Cursors::Arrow;
        Brush _background = Colors::Transparent;
        
        ValueSource _cursorSource {};
        ValueSource _backgroundSource {};
    };

    using RenderFragment = Action<RenderContext, const FRect&>;
}