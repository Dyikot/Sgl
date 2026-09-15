#pragma once

#include <span>
#include <SDL3/SDL_render.h>

#include "Surface.h"
#include "Texture.h"
#include "../Base/Primitives.h"
#include "../Base/Media/Color.h"

namespace Sgl
{
    //! @brief Represents a single vertex with position, color, and texture coordinate
    using Vertex = SDL_Vertex;

    //! @brief Provides a high-level interface for 2D rendering operations using an SDL_Renderer.
    //! Encapsulates common drawing primitives, texture rendering, clipping, and render target management.
    class RenderContext
    {
    public:
        //! @brief Constructs a RenderContext instance with the specified SDL renderer
        //! @param renderer Pointer to the SDL_Renderer to use for all drawing operations. Must not be null.
        explicit RenderContext(SDL_Renderer* renderer);

        //! @brief Gets the underlying SDL renderer used by this context
        //! @return Pointer to the associated SDL_Renderer
        SDL_Renderer* GetRenderer() const noexcept { return _renderer; }

        //! @brief Sets the current render target to the specified texture.
        //! All subsequent drawing operations will render into this texture until ResetTarget() is called.
        //! @param texture The texture to use as the new render target
        void SetTarget(const Texture& texture);

        //! @brief Resets the render target back to the default (Window)
        void ResetTarget();

        //! @brief Sets a clipping rectangle that restricts all subsequent drawing operations to the specified region
        //! @param clip The rectangle defining the clipping area in renderer coordinates
        void SetClip(Rect clip);

        //! @brief Removes any active clipping rectangle, restoring full rendering access to the target
        void ResetClip();

        //! @brief Fills the entire current render target with a solid color
        //! @param color The color to use for the background fill
        void FillBackground(Color color);

        //! @brief Draws a single point at the specified floating-point coordinates
        //! @param point The position (x, y) of the point to draw
        //! @param color The color of the point
        void DrawPoint(FPoint point, Color color);

        //! @brief Draws multiple points from a span of floating-point coordinates
        //! @param points A span of FPoint structures representing the points to draw
        //! @param color The color used for all points
        void DrawPoints(std::span<const FPoint> points, Color color);

        //! @brief Draws a line between two floating-point endpoints
        //! @param start The starting point of the line
        //! @param end The ending point of the line
        //! @param color The color of the line
        void DrawLine(FPoint start, FPoint end, Color color);

        //! @brief Draws a series of connected lines (a polyline) through the given points
        //! @param points A span of FPoint structures defining the vertices of the polyline
        //! @param color The color of the lines
        void DrawLines(std::span<const FPoint> points, Color color);

        //! @brief Draws the outline of a rectangle
        //! @param rect The rectangle to draw (x, y, width, height)
        //! @param color The color of the rectangle outline
        void DrawRectangle(FRect rect, Color color);

        //! @brief Draws the outline of a rectangle with a specified border thickness
        //! @param rect The rectangle to draw (x, y, width, height)
        //! @param thickness The thickness of the rectangle outline
        //! @param color The color of the rectangle outline
        void DrawRectangle(FRect rect, float thickness, Color color);

        //! @brief Draws the outline of a rounded rectangle
        //! @param rect The rectangle to draw (x, y, width, height)
        //! @param cornersRadius The radius of the rectangle corners
        //! @param color The color of the rectangle outline
        void DrawRoundedRectangle(FRect rect, float cornersRadius, Color color);

        //! @brief Draws the outline of a rounded rectangle with a specific line thickness
        //! @param rect The rectangle to draw (x, y, width, height)
        //! @param cornersRadius The radius of the rectangle corners
        //! @param thickness The thickness of the rectangle outline
        //! @param color The color of the rectangle outline
        void DrawRoundedRectangle(FRect rect, float cornersRadius, float thickness, Color color);

        //! @brief Draws outlines of multiple rectangles
        //! @param rects A span of FRect structures to draw as unfilled rectangles
        //! @param color The color used for all rectangle outlines
        void DrawRectangles(std::span<const FRect> rects, Color color);

        //! @brief Fills a rectangle with a solid color
        //! @param rect The rectangle to fill (x, y, width, height)
        //! @param color The fill color
        void DrawRectangleFill(FRect rect, Color color);

        //! @brief Fills a rectangle with a solid color and rounded corners
        //! @param rect The rectangle to fill (x, y, width, height)
        //! @param cornersRadius The radius of the rectangle's rounded corners
        //! @param color The fill color
        void DrawRectangleFill(FRect rect, float cornersRadius, Color color);

        //! @brief Fills a rectangle with a texture and rounded corners
        //! @param rect The rectangle to fill (x, y, width, height)
        //! @param cornersRadius The radius of the rectangle's rounded corners
        //! @param texture The texture to apply as a fill
        void DrawRectangleFill(FRect rect, float cornersRadius, const Texture& texture);

        //! @brief Fills multiple rectangles with a solid color
        //! @param rects A span of FRect structures to fill
        //! @param color The fill color for all rectangles
        void DrawRectanglesFill(std::span<const FRect> rects, Color color);

        //! @brief Draws the outline of an ellipse inscribed within the given rectangle
        //! @param rect The bounding rectangle that defines the ellipse (x, y, width, height)
        //! @param color The color of the ellipse outline
        void DrawEllipse(FRect rect, Color color);

        //! @brief Fills an ellipse inscribed within the given rectangle with a solid color
        //! @param rect The bounding rectangle that defines the ellipse
        //! @param color The fill color
        void DrawEllipseFill(FRect rect, Color color);

        //! @brief Fills an ellipse inscribed within the given rectangle using a texture
        //! @param rect The bounding rectangle that defines the ellipse
        //! @param texture The texture used to fill the ellipse
        void DrawEllipseFill(FRect rect, const Texture& texture);

        //! @brief Renders a custom textured geometry defined by vertices, optionally using a texture and an index order
        //! @param vertices A span of SDL_Vertex structures defining the geometry
        //! @param texture The texture to apply to the geometry
        //! @param indices Optional span of indices defining the drawing order. If empty, vertices are drawn in sequence.
        void DrawGeometry(std::span<const Vertex> vertices, 
                          const Texture& texture = {}, 
                          std::span<const int> indices = {});

        //! @brief Draws a texture to the screen with optional target and source clipping
        //! @param texture The texture to render
        //! @param target The destination rectangle on the render target
        //! @param clip The source rectangle within the texture to draw (in texture coordinates)
        void DrawTexture(const Texture& texture, const FRect* target, const FRect* clip);

        //! @brief Renders a 9-grid scaled texture with optional target and source clipping
        //! @param texture The texture to render
        //! @param cornersLength Length of the corner regions
        //! @param scale Scaling factor
        //! @param target Destination rectangle
        //! @param clip Source rectangle within the texture (optional clipping of the original texture before 9-grid processing)
        void DrawTexture9Grid(const Texture& texture, 
                              float cornersLength, 
                              float scale, 
                              const FRect* target, 
                              const FRect* clip);

        //! @brief Draws a transformed and clipped texture into a target rectangle
        //! @param texture The texture to render
        //! @param angle Rotation angle in degrees
        //! @param center Optional rotation center
        //! @param flipMode Flipping mode
        //! @param target Destination rectangle
        //! @param clip Source rectangle within the texture to draw
        void DrawTextureTransformed(const Texture& texture, 
                                    double angle, 
                                    const FPoint* center, 
                                    FlipMode flipMode, 
                                    const FRect* target, 
                                    const FRect* clip);

        //! @brief Renders a string of text at the specified position using the given font and color
        //! @param position The top-left position where text begins
        //! @param text The UTF-8 encoded text to render
        //! @param size Font size in points
        //! @param color Text color
        //! @param fontFamily Font family to use; defaults to FontFamily::Default if not specified
        void DrawText(FPoint position, 
                      std::string_view text, 
                      float size, 
                      Color color, 
                      FontFamily fontFamily = FontFamily::GetDefault());

    private:
        void DrawEllipseCore(FRect rect, const Texture& texture, Color color);
        void DrawRectangleFillCore(FRect rect, float cornersRadius, const Texture& texture, Color color);
        void SetColor(Color color) const noexcept;

    private:
        SDL_Renderer* _renderer;
    };
}