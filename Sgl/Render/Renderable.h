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
        Renderable(const Renderable&) = delete;
        Renderable(Renderable&& other) noexcept;
        virtual ~Renderable() = default;

        void SetCursor(const Cursor& value, ValueSource source = ValueSource::Local);
        const Cursor& GetCursor() const { return _cursor; }

        void SetBackground(const Brush& value, ValueSource source = ValueSource::Local);
        const Brush& GetBackground() const { return _background; }

        virtual void SetVisualRoot(IVisualRoot* value);
        IVisualRoot* GetVisualRoot() const { return _visualRoot; }
        
        void SetParent(IStyleHost* parent) override;

        virtual void Render(RenderContext& context) {}
        virtual void InvalidateRender();

        static inline StyleableProperty CursorProperty { &SetCursor, &GetCursor };
        static inline StyleableProperty BackgroundProperty { &SetBackground, &GetBackground };
    protected:
        virtual void OnCursorChanged(const Cursor& cursor) {}
        void RenderBackground(RenderContext& context);
        void RenderBackground(RenderContext& context, const FRect& rect);
    private:
        void InvalidateBackground();
    private:
        IVisualRoot* _visualRoot = nullptr;
        Texture _backgroundTexture;

        Cursor _cursor = Cursors::Arrow;
        Brush _background = Colors::Transparent;

        ValueSource _cursorSource {};
        ValueSource _backgroundSource {};
    };

    template<>
    class ResourceSetter<Renderable, const Brush&> : public SetterBase
    {
    public:
        ResourceSetter(StyleableProperty<Renderable, const Brush&>& property, ResourceKey key);

        void Apply(StyleableElement& target, ValueSource valueSource) const override;
    private:
        ResourceKey _key;
    };
}