using System.Runtime.InteropServices;

namespace QuestPDF.Skia;

internal sealed class SkDrawable : IDisposable
{
    public IntPtr Instance { get; private set; }
    
    public SkDrawable(IntPtr instance)
    {
        Instance = instance;
        SkiaAPI.EnsureNotNull(Instance);
    }
    
    ~SkDrawable()
    {
        Dispose();
    }
    
    public void Dispose()
    {
        if (Instance == IntPtr.Zero)
            return;
        
        API.drawable_unref(Instance);
        Instance = IntPtr.Zero;
    }
    
    private static class API
    {
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void drawable_unref(IntPtr picture);
    }
}