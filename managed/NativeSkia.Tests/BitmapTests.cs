using NUnit.Framework;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

public class BitmapTests
{
    [Test]
    public void Encode()
    {
        using var bitmap = new SkBitmap(400, 300);
        using var canvas = SkCanvas.CreateFromBitmap(bitmap);
        
        using var imageData = SkData.FromFile("Input/image.jpg");
        using var image = SkImage.FromData(imageData);

        // blue filled rectangle
        canvas.DrawImage(image, 400, 300);
        canvas.DrawStrokeRectangle(new SkRect(50, 50, 350, 250), 20, 0xFF3F51B5);

        using var jpegData = bitmap.EncodeAsJpeg(10);
        TestFixture.SaveOutput("bitmap_encode.jpg", jpegData);
        
        using var pngData = bitmap.EncodeAsPng();
        TestFixture.SaveOutput("bitmap_encode.png", pngData);
        
        using var webpData = bitmap.EncodeAsWebp(10);
        TestFixture.SaveOutput("bitmap_encode.webp", webpData);
    }
}