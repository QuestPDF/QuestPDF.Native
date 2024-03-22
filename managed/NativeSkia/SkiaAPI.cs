using System.Runtime.CompilerServices;

namespace QuestPDF.Skia;

internal static class SkiaAPI
{
    public const string LibraryName = "QuestPdfSkia";
    
    public static void EnsureNotNull(IntPtr instance)
    {
        if (instance == IntPtr.Zero)
            throw new InvalidOperationException($"Cannot instantiate native object.");
    }
}