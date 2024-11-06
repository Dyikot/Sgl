#include "Geometry.h"

namespace Sgl
{
    Line::Line() noexcept: 
        Start(), End(), Color(Colors::White) {}

    Line::Line(const SDL_FPoint& start, const SDL_FPoint& end, ColorBrush color) noexcept:
        Start(start), End(end), Color(color) {}





    Lines::Lines() noexcept: 
        Points(), Color(Colors::White) {}

    Lines::Lines(const std::vector<SDL_FPoint>& points, ColorBrush color) noexcept:
        Points(points), Color(color) {}

    Lines::Lines(std::vector<SDL_FPoint> && points, ColorBrush color) noexcept:
        Points(std::move(points)), Color(color) {}





    Rectangle::Rectangle() noexcept: 
        Properties(), Color(Colors::White) {}

    Rectangle::Rectangle(const SDL_FRect & properties, ColorBrush color) noexcept:
        Properties(properties), Color(color) {}





    Rectangles::Rectangles() noexcept: 
        Properties(), Color(Colors::White) {}

    Rectangles::Rectangles(const std::vector<SDL_FRect>& properties,
                           ColorBrush color) noexcept:
        Properties(properties), Color(color) {}

    Rectangles::Rectangles(std::vector<SDL_FRect>&& properties, ColorBrush color) noexcept:
        Properties(std::move(properties)), Color(color) {}





    FillRectangle::FillRectangle() noexcept:
        Properties(), Background(Colors::White) {}

    FillRectangle::FillRectangle(const SDL_FRect& properties, ColorBrush background) noexcept:
        Properties(properties), Background(background) {}





    FillRectangles::FillRectangles() noexcept:
        Properties(), Color(Colors::White) {}

    FillRectangles::FillRectangles(const std::vector<SDL_FRect>&properties,
                                   ColorBrush color) noexcept:
        Properties(properties), Color(color) {}

    FillRectangles::FillRectangles(std::vector<SDL_FRect> && properties, 
                                   ColorBrush color) noexcept:
        Properties(std::move(properties)), Color(color) {}





    TextureRectangle::TextureRectangle() noexcept: 
        Properties(), Texture{} {}

    TextureRectangle::TextureRectangle(const SDL_FRect& properties, 
                                       SDL_Texture* texture) noexcept:
        Properties(properties), Texture(texture) {}

    TextureRectangle::TextureRectangle(const SDL_FRect& properties,
                                       const SDL_Rect& clip, 
                                       SDL_Texture* texture) noexcept:
        Properties(properties), Clip(clip), Texture(texture) {}





    Figure::Figure() noexcept:
        Vertices(), RenderOrder() {}

    Figure::Figure(const std::vector<SDL_Vertex>& vertices,
                   const std::vector<int>& renderOrder,
                   SDL_Texture* texture) noexcept:
        Vertices(vertices), RenderOrder(renderOrder), FigureTexture(texture) {}

    Figure::Figure(std::vector<SDL_Vertex>&& vertices,
                   const std::vector<int>& renderOrder,
                   SDL_Texture* texture) noexcept:
        Vertices(std::move(vertices)), RenderOrder(renderOrder), FigureTexture(texture) {}





    Ellipse::Ellipse() noexcept: 
        Ellipse(SDL_FPoint{}, 0, 0, Colors::White) {}

    Ellipse::Ellipse(const SDL_FPoint& center,
                     float width,
                     float height,
                     ColorBrush color) noexcept:
        _center(center), _width(width), _height(height), Color(color)
    {
        OnSizeChanged();
    }

    void Ellipse::SetCenterPoint(const SDL_FPoint& value)
    {
        _center = value;
        OnSizeChanged();
    }

    void Ellipse::SetWidth(float value)
    {
        _width = value;
        OnSizeChanged();
    }

    void Ellipse::SetHeight(float value)
    {
        _height = value;
        OnSizeChanged();
    }

    void Ellipse::OnSizeChanged()
    {
        for(size_t i = 0; i < PointNumber; i++)
        {
            _points[i].x = _center.x + _width * Cos_t[i];
            _points[i].y = _center.y + _height * Sin_t[i];
        }
    }
}