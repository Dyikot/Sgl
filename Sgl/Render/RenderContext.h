#pragma once

#include <span>
#include <SDL3/SDL_render.h>

#include "Surface.h"
#include "Texture.h"
#include "../Base/Primitives.h"
#include "../Base/Media/Color.h"

namespace Sgl
{
    /// <summary>
    /// Representing a single vertex with position, color, and texture coordinate.
    /// </summary>
    using Vertex = SDL_Vertex;

    /// <summary>
    /// Provides a high-level interface for 2D rendering operations using an SDL_Renderer.
    /// Encapsulates common drawing primitives, texture rendering, clipping, and render target management.
    /// </summary>
    class RenderContext
    {
    public:
        /// <summary>
        /// Constructs a RenderContext instance with the specified SDL renderer.
        /// </summary>
        /// <param name="renderer"> - pointer to the SDL_Renderer to use for all drawing operations. Must not be null.</param>
        explicit RenderContext(SDL_Renderer* renderer);

        /// <summary>
        /// Gets the underlying SDL renderer used by this context.
        /// </summary>
        /// <returns>Pointer to the associated SDL_Renderer.</returns>
        SDL_Renderer* GetRenderer() const noexcept { return _renderer; }

        /// <summary>
        /// Sets the current render target to the specified texture.
        /// All subsequent drawing operations will render into this texture until ResetTarget() is called.
        /// </summary>
        /// <param name="texture"> - the texture to use as the new render target.</param>
        void SetTarget(const Texture& texture);

        /// <summary>
        /// Resets the render target back to the default (Window).
        /// </summary>
        void ResetTarget();

        /// <summary>
        /// Sets a clipping rectangle that restricts all subsequent drawing operations to the specified region.
        /// </summary>
        /// <param name="clip"> - the rectangle defining the clipping area in renderer coordinates.</param>
        void SetClip(Rect clip);

        /// <summary>
        /// Removes any active clipping rectangle, restoring full rendering access to the target.
        /// </summary>
        void ResetClip();

        /// <summary>
        /// Fills the entire current render target with a solid color.
        /// </summary>
        /// <param name="color"> - the color to use for the background fill.</param>
        void FillBackground(Color color);

        /// <summary>
        /// Draws a single point at the specified floating-point coordinates.
        /// </summary>
        /// <param name="point"> - the position (x, y) of the point to draw.</param>
        /// <param name="color"> - the color of the point.</param>
        void DrawPoint(FPoint point, Color color);

        /// <summary>
        /// Draws multiple points from a span of floating-point coordinates.
        /// </summary>
        /// <param name="points"> - a span of FPoint structures representing the points to draw.</param>
        /// <param name="color"> - the color used for all points.</param>
        void DrawPoints(std::span<const FPoint> points, Color color);

        /// <summary>
        /// Draws a line between two floating-point endpoints.
        /// </summary>
        /// <param name="start"> - the starting point of the line.</param>
        /// <param name="end"> - the ending point of the line.</param>
        /// <param name="color"> - the color of the line.</param>
        void DrawLine(FPoint start, FPoint end, Color color);

        /// <summary>
        /// Draws a series of connected lines (a polyline) through the given points.
        /// </summary>
        /// <param name="points"> - a span of FPoint structures defining the vertices of the polyline.</param>
        /// <param name="color"> - the color of the lines.</param>
        void DrawLines(std::span<const FPoint> points, Color color);

        /// <summary>
        /// Draws the outline of a rectangle with floating-point coordinates and dimensions.
        /// </summary>
        /// <param name="rect"> - the rectangle to draw (x, y, width, height).</param>
        /// <param name="color"> - the color of the rectangle outline.</param>
        void DrawRectangle(FRect rect, Color color);

        /// <summary>
        /// Draws outlines of multiple rectangles.
        /// </summary>
        /// <param name="rects"> - a span of FRect structures to draw as unfilled rectangles.</param>
        /// <param name="color"> - the color used for all rectangle outlines.</param>
        void DrawRectangles(std::span<const FRect> rects, Color color);

        /// <summary>
        /// Fills a rectangle with a solid color using floating-point coordinates and dimensions.
        /// </summary>
        /// <param name="rect"> - the rectangle to fill (x, y, width, height).</param>
        /// <param name="color"> - the fill color.</param>
        void DrawRectangleFill(FRect rect, Color color);

        /// <summary>
        /// Fills multiple rectangles with a solid color.
        /// </summary>
        /// <param name="rects"> - a span of FRect structures to fill.</param>
        /// <param name="color"> - the fill color for all rectangles.</param>
        void DrawRectanglesFill(std::span<const FRect> rects, Color color);

        /// <summary>
        /// Draws the outline of an ellipse inscribed within the given rectangle.
        /// </summary>
        /// <param name="rect"> - the bounding rectangle that defines the ellipse (x, y, width, height).</param>
        /// <param name="color"> - the color of the ellipse outline.</param>
        void DrawEllipse(FRect rect, Color color);

        /// <summary>
        /// Draws a smoothed (anti-aliased) outline of an ellipse inscribed within the given rectangle.
        /// </summary>
        /// <param name="rect"> - the bounding rectangle that defines the ellipse.</param>
        /// <param name="color"> - the color of the smoothed ellipse outline.</param>
        void DrawEllipseSmooth(FRect rect, Color color);

        /// <summary>
        /// Fills an ellipse inscribed within the given rectangle with a solid color.
        /// </summary>
        /// <param name="rect"> - the bounding rectangle that defines the ellipse.</param>
        /// <param name="color"> - the fill color.</param>
        void DrawEllipseFill(FRect rect, Color color);

        /// <summary>
        /// Fills an ellipse inscribed within the given rectangle using a texture.
        /// </summary>
        /// <param name="rect"> - the bounding rectangle that defines the ellipse.</param>
        /// <param name="texture"> - the texture used to fill the ellipse.</param>
        void DrawEllipseFill(FRect rect, const Texture& texture);

        /// <summary>
        /// Renders a custom textured geometry defined by vertices, optionally using a texture and an index order.
        /// </summary>
        /// <param name="vertices"> - a span of SDL_Vertex structures defining the geometry.</param>
        /// <param name="texture"> - the texture to apply to the geometry.</param>
        /// <param name="order"> - optional span of indices defining the drawing order. If empty, vertices are drawn in sequence.</param>
        void DrawGeometry(std::span<const Vertex> vertices, 
                          const Texture& texture = {},
                          std::span<const int> order = {});

        /// <summary>
        /// Draws a texture to the screen with optional both target and source clipping.
        /// </summary>
        /// <param name="texture"> - the texture to render.</param>
        /// <param name="target"> - the destination rectangle on the render target.</param>
        /// <param name="clip"> - the source rectangle within the texture to draw (in texture coordinates).</param>
        void DrawTexture(const Texture& texture, const FRect* target, const FRect* clip);

        /// <summary>
        /// Renders a 9-grid scaled texture with optional both target and source clipping.
        /// </summary>
        /// <param name="texture"> - the texture to render.</param>
        /// <param name="cornersLength"> - length of the corner regions.</param>
        /// <param name="scale"> - scaling factor.</param>
        /// <param name="target"> - destination rectangle.</param>
        /// <param name="clip"> - source rectangle within the texture (optional clipping of the original texture before 9-grid processing).</param>
        void DrawTexture9Grid(const Texture& texture, 
                              float cornersLength, 
                              float scale, 
                              const FRect* target, 
                              const FRect* clip);

        /// <summary>
        /// Draws a transformed and clipped texture into a target rectangle.
        /// </summary>
        /// <param name="texture"> - the texture to render.</param>
        /// <param name="angle"> - rotation angle in degrees.</param>
        /// <param name="center"> - optional rotation center.</param>
        /// <param name="flip"> - flipping mode.</param>
        /// <param name="target"> - destination rectangle.</param>
        /// <param name="clip"> - source rectangle within the texture to draw.</param>
        void DrawTextureTransformed(const Texture& texture, 
                                    double angle, 
                                    const FPoint* center, 
                                    FlipMode flip, 
                                    const FRect* target, 
                                    const FRect* clip);

        /// <summary>
        /// Renders a string of text at the specified position using the given font and color.
        /// </summary>
        /// <param name="position"> - the top-left position where text begins.</param>
        /// <param name="text"> - the UTF-8 encoded text to render.</param>
        /// <param name="size"> - font size in points.</param>
        /// <param name="color"> - text color.</param>
        /// <param name="fontFamily"> - font family to use; defaults to FontFamily::Default if not specified.</param>
        void DrawText(FPoint position, 
                      std::string_view text, 
                      float size, 
                      Color color, 
                      FontFamily fontFamily = FontFamily::Default);
    private:
        void SetColor(Color color) const noexcept
        {
            SDL_SetRenderDrawColor(_renderer, color.Red, color.Green, color.Blue, color.Alpha);
        }        
    private:
        SDL_Renderer* _renderer;
    };
}