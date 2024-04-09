using System.Runtime.InteropServices;

namespace QuestPDF.Skia.Text;

internal sealed class SkFontCollection : IDisposable
{
    public IntPtr Instance { get; private set; }
    
    public SkFontCollection(IntPtr instance)
    {
        Instance = instance;
        SkiaAPI.EnsureNotNull(Instance);
    }

    public static SkFontCollection Create(SkTypefaceProvider typefaceProvider, SkFontManager fontManager)
    {
        var instance = API.font_collection_create(fontManager.Instance, typefaceProvider.Instance);
        return new SkFontCollection(instance);
    }
    
    ~SkFontCollection()
    {
        Dispose();
    }

    public void Dispose()
    {
        if (Instance == IntPtr.Zero)
            return;
        
        API.font_collection_unref(Instance);
        Instance = IntPtr.Zero;
    }
    
    private static class API
    {
        [DllImport(SkiaAPI.LibraryName)]
        public static extern IntPtr font_collection_create(IntPtr fontManager, IntPtr typefaceProvider);
        
        [DllImport(SkiaAPI.LibraryName)]
        public static extern void font_collection_unref(IntPtr fontCollection);
    }
}