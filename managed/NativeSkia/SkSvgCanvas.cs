using System.Runtime.InteropServices;

namespace QuestPDF.Skia;

internal sealed class SkSvgCanvas
{
    public static SkCanvas CreateSvg(float width, float height, SkWriteStream writeStream)
    {
        var bounds = new SkRect(0, 0, width, height);
        var instance = API.svg_create_canvas(bounds, writeStream.Instance);
        return new SkCanvas(instance);
    }
    
    private static class API
    {
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr svg_create_canvas(SkRect bounds, IntPtr writeStream);
    }
}