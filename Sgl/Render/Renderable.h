#pragma once

#include "../Styling/StyleableElement.h"
#include "../Input/Cursor.h"
#include "TextureSource.h"
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
    public:
        Renderable() = default;
        Renderable(const Renderable& other);
        Renderable(Renderable&& other) noexcept;
        virtual ~Renderable() = default;

        void SetCursor(const Cursor& value);
        const Cursor& GetCursor() const { return _cursor; }

        void SetBackground(const Brush& value);
        const Brush& GetBackground() const { return _background; }

        virtual void SetVisualRoot(IVisualRoot* value);
        IVisualRoot* GetVisualRoot() const { return _visualRoot; }
        
        void SetParent(IStyleHost* parent) override;

        virtual void Render(RenderContext context) {}
        virtual void InvalidateRender();
    protected:
        virtual void OnCursorChanged(const Cursor& cursor) {}
    public:
        static inline SglProperty CursorProperty { &SetCursor, &GetCursor };
        static inline SglProperty BackgroundProperty { &SetBackground, &GetBackground };
    };
}