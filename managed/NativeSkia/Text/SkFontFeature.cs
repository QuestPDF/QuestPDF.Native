using System.Runtime.InteropServices;

namespace QuestPDF.Skia.Text;

internal sealed class SkFontFeature
{
    [StructLayout(LayoutKind.Sequential)]
    public struct FontFeature
    {
        public int Tag; // Encoded as 32-bit integer
        public int Value;
    }

    public static IntPtr StructToIntPtr(FontFeature feature)
    {
        IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(FontFeature)));
        Marshal.StructureToPtr(feature, ptr, false);
        return ptr;
    }
}