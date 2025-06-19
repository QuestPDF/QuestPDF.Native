using System;
using System.Linq;
using System.Runtime.InteropServices;

namespace QuestPDF.Skia;

internal sealed class SkPaint : IDisposable
{
    public IntPtr Instance { get; private set; }
    
    public SkPaint()
    {
        Instance = API.paint_create();
        SkiaAPI.EnsureNotNull(Instance);
    }
    
    public void SetSolidColor(uint color)
    {
        API.paint_set_solid_color(Instance, color);
    }
    
    public void SetLinearGradient(SkPoint start, SkPoint end, uint[] colors)
    {
        API.paint_set_linear_gradient(Instance, start, end, colors.Length, colors);
    }
    
    public void SetStroke(float thickness)
    {
        API.paint_set_stroke(Instance, thickness);
    }
    
    public void SetDashedPathEffect(float[] intervals)
    {
        API.paint_set_dashed_path_effect(Instance, intervals.Length, intervals);
    }
    
    ~SkPaint()
    {
        Dispose();
    }
    
    public void Dispose()
    {
        if (Instance == IntPtr.Zero)
            return;
        
        API.paint_delete(Instance);
        Instance = IntPtr.Zero;
        GC.SuppressFinalize(this);
    }
    
    private static class API
    {
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr paint_create();
    
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void paint_delete(IntPtr paint);
        
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void paint_set_solid_color(IntPtr paint, uint color);
        
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void paint_set_linear_gradient(IntPtr paint, SkPoint start, SkPoint end, int colorsLength, uint[] colors);    
        
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void paint_set_stroke(IntPtr paint, float thickness);    
        
        [DllImport(SkiaAPI.LibraryName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void paint_set_dashed_path_effect(IntPtr paint, int arrayLength, float[] intervals); 
    }
}