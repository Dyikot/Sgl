#pragma once

#include "RenderContext.h"
#include "../Styling/StyleableElement.h"
#include "../Input/Cursor.h"
#include "../Base/Media/ImageBrush.h"

namespace Sgl
{
    class Renderable : public StyleableElement
    {
    private:
        Cursor _cursor = Cursors::Arrow;
        Brush _background = Colors::Transparent;

        bool _isRenderValid = false;
    public:
        Renderable() = default;
        Renderable(const Renderable& other);
        Renderable(Renderable&& other) noexcept;
        virtual ~Renderable() = default;

        void SetCursor(const Cursor& value);
        const Cursor& GetCursor() const { return _cursor; }

        void SetBackground(const Brush& value);
        const Brush& GetBackground() const { return _background; }
        
        bool NeedsRendering() const { return !_isRenderValid; }

        virtual void Render(RenderContext context);
        void InvalidateRender();
    protected:
        virtual void OnCursorChanged(const Cursor& cursor) {}
    public:
        static inline ObservableProperty CursorProperty { &SetCursor, &GetCursor };
        static inline ObservableProperty BackgroundProperty { &SetBackground, &GetBackground };
    };
}