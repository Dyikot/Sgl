#pragma once

#include "RenderContext.h"
#include "../Styling/StyleableElement.h"
#include "../Input/Cursor.h"
#include "../Base/Media/Brush.h"

namespace Sgl
{
    class Renderable: public StyleableElement
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

        void SetBackground(Brush value);
        Brush GetBackground() const { return _background; }
        
        bool NeedsRendering() const { return !_isRenderValid; }

        virtual void Render(RenderContext context);
        void InvalidateRender();
    protected:
        virtual void OnCursorChanged(const Cursor& cursor) {}
    public:
        static inline ObservableProperty<Renderable, const Cursor&> CursorProperty =
            ObservableProperty<Renderable, const Cursor&>(&SetCursor, &GetCursor);

        static inline ObservableProperty<Renderable, Brush> BackgroundProperty =
            ObservableProperty<Renderable, Brush>(&SetBackground, &GetBackground);        
    };
}