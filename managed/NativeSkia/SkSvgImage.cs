using System.Runtime.InteropServices;
using QuestPDF.Skia.Text;

namespace QuestPDF.Skia;

internal sealed class SkSvgImage : IDisposable
{
    public IntPtr Instance { get; private set; }
    public SkRect ViewBox;
    
    public SkSvgImage(string svgString, SkFontManager fontManager)
    {
        using var data = SkData.FromBinary(System.Text.Encoding.UTF8.GetBytes(svgString));

        Instance = API.svg_create(data.Instance, fontManager.Instance);
        
        if (Instance == IntPtr.Zero)
            throw new Exception("Cannot decode the provided SVG image.");
        
        API.svg_get_viewbox(Instance, out ViewBox);
    }
    
    ~SkSvgImage()
    {
        Dispose();
    }
    
    public void Dispose()
    {
        if (Instance == IntPtr.Zero)
            return;
        
        API.svg_unref(Instance);
        Instance = IntPtr.Zero;
    }
    
    private static class API
    {
        [DllImport(SkiaAPI.LibraryName)]
        public static extern IntPtr svg_create(IntPtr data, IntPtr fontManager);
        
        [DllImport(SkiaAPI.LibraryName)]
        public static extern void svg_unref(IntPtr svg);
        
        [DllImport(SkiaAPI.LibraryName)]
        public static extern void svg_get_viewbox(IntPtr svg, out SkRect viewbox);
    }
}