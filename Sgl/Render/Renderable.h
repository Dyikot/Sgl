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
    private:
        IVisualRoot* _visualRoot = nullptr;
        Cursor _cursor = Cursors::Arrow;
        Brush _background = Colors::Transparent;

        ValueSource _cursorSource {};
        ValueSource _backgroundSource {};
    public:
        Renderable() = default;
        Renderable(const Renderable& other);
        Renderable(Renderable&& other) noexcept;
        virtual ~Renderable() = default;

        void SetCursor(const Cursor& value, ValueSource source = ValueSource::Local);
        const Cursor& GetCursor() const { return _cursor; }

        void SetBackground(const Brush& value, ValueSource source = ValueSource::Local);
        const Brush& GetBackground() const { return _background; }

        virtual void SetVisualRoot(IVisualRoot* value);
        IVisualRoot* GetVisualRoot() const { return _visualRoot; }
        
        void SetParent(IStyleHost* parent) override;

        virtual void Render(RenderContext context) {}
        virtual void InvalidateRender();
    protected:
        virtual void OnCursorChanged(const Cursor& cursor) {}
    public:
        static inline StyleableProperty CursorProperty { &SetCursor, &GetCursor };
        static inline StyleableProperty BackgroundProperty { &SetBackground, &GetBackground };
    };
}