using QuestPDF.Skia;

namespace NativeSkia.Tests;

internal static class Helpers
{
    public static void DrawFilledRectangle(this SkCanvas canvas, SkRect rectangle, uint color)
    {
        using var paint = new SkPaint();
        paint.SetSolidColor(color);
        canvas.DrawRectangle(rectangle, paint);
    }
    
    public static void DrawStrokeRectangle(this SkCanvas canvas, SkRect rectangle, float thickness, uint color)
    {
        using var paint = new SkPaint();
        paint.SetStroke(thickness);
        paint.SetSolidColor(color);
        canvas.DrawRectangle(rectangle, paint);
    }
}