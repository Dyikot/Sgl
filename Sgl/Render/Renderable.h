#pragma once

#include "../Styling/StyleableElement.h"
#include "../Input/Cursor.h"
#include "../Base/Media/Brush.h"
#include "IVisualRoot.h"
#include "RenderContext.h"

namespace Sgl
{
    class Renderable : public StyleableElement
    {
    public:
        Renderable() = default;
        Renderable(Renderable&& other) noexcept;

        void SetCursor(Cursor value, ValueSource source = ValueSource::Local);
        Cursor GetCursor() const { return _cursor; }

        void SetBackground(const Brush& value, ValueSource source = ValueSource::Local);
        const Brush& GetBackground() const { return _background; }

        virtual void SetVisualRoot(IVisualRoot* value);
        IVisualRoot* GetVisualRoot() const { return _visualRoot; }
        
        void SetParent(IStyleHost* parent) override;

        virtual void Render(RenderContext context);
        virtual void InvalidateRender();

        static inline StyleableProperty CursorProperty { &SetCursor, &GetCursor };
        static inline StyleableProperty BackgroundProperty { &SetBackground, &GetBackground };
    protected:
        virtual void OnCursorChanged(Cursor cursor) {}
        virtual void OnBackgroundChanged(const Brush& background) {}
    protected:
        static inline PlatformCursor _platformCursor;
    private:
        IVisualRoot* _visualRoot = nullptr;

        Cursor _cursor = Cursors::Arrow;
        Brush _background = Colors::Transparent;

        ValueSource _cursorSource {};
        ValueSource _backgroundSource {};

        bool _isDirty = true;
    };

    template<>
    class ResourceSetter<Renderable, const Brush&> : public SetterBase
    {
    public:
        using BackgroundProperty = decltype(Renderable::BackgroundProperty);
    public:
        ResourceSetter(BackgroundProperty& property, ResourceKey key);

        void Apply(StyleableElement& target, ValueSource valueSource) const final;
    private:
        ResourceKey _key;
    };
}