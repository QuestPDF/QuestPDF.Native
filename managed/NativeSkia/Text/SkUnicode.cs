using System;
using System.Runtime.InteropServices;

namespace QuestPDF.Skia.Text;

internal sealed class SkUnicode : IDisposable
{
    public IntPtr Instance { get; private set; }
    public static SkUnicode Global { get; } = new();

    private SkUnicode()
    {
        Instance = API.unicode_create();
    }
    
    ~SkUnicode()
    {
        Dispose();
    }
    
    public void Dispose()
    {
        if (Instance == IntPtr.Zero)
            return;
        
        API.unicode_unref(Instance);
        Instance = IntPtr.Zero;
    }
    
    private static class API
    {
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr unicode_create();
        
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr unicode_unref(IntPtr instance);
    }
}