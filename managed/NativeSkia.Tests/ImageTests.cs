using NUnit.Framework;
using FluentAssertions;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

public class ImageTests
{
    [Test]
    public void GeneratePlaceholder()
    {
        using var image = SkImage.GeneratePlaceholder(400, 300, 0xFF64B5F6, 0xFFBA68C8);
        image.Width.Should().Be(400);
        image.Height.Should().Be(300);
        image.EncodedDataSize.Should().Be(1659);
        
        var content = image.GetEncodedData().ToBytes();
        content.Length.Should().Be(1659);
        
        // blue (top left)
        // purple (bottom right)
        var path = Path.Combine(TestFixture.InputPath, "placeholder.jpg");
        File.WriteAllBytes(path, content);
    }
    
    [Test]
    public void Resize_Opaque()
    {
        using var sampleImageData = SkData.FromFile("Input/opaque-image.jpg");
        using var sampleImage = SkImage.FromData(sampleImageData);
        
        sampleImage.Width.Should().Be(800);
        sampleImage.Height.Should().Be(600);
        
        // don't scale images to if target resolution is bigger than original
        using var biggerImage = sampleImage.ResizeAndCompress(40_000, 30_000, 90);
        using var biggerImageData = biggerImage.GetEncodedData();
        
        biggerImage.Width.Should().Be(800);
        biggerImage.Height.Should().Be(600);
        biggerImageData.ToBytes().Length.Should().BeLessThan(100_000);
        
        // scale images to if target resolution is smaller than original
        using var smallerImage = sampleImage.ResizeAndCompress(80, 60, 90);
        using var smallerImageData = smallerImage.GetEncodedData();
        
        smallerImage.Width.Should().Be(80);
        smallerImage.Height.Should().Be(60);
        smallerImageData.ToBytes().Length.Should().BeLessThan(5_000);
    }
    
    [Test]
    public void ResizeAndCompress_Opaque()
    {
        using var sampleImageData = SkData.FromFile("Input/opaque-image.jpg");
        using var sampleImage = SkImage.FromData(sampleImageData);
        
        // high quality
        using var highQualityImage = sampleImage.ResizeAndCompress(400, 300, 90);
        using var highQualityImageData = highQualityImage.GetEncodedData();
        highQualityImageData.ToBytes().Length.Should().BeInRange(30_000, 32_000);
        TestFixture.SaveOutput("image_resize_high_quality.jpg", highQualityImageData);
        
        // low quality
        using var lowQualityImage = sampleImage.ResizeAndCompress(400, 300, 10);
        using var lowQualityImageData = lowQualityImage.GetEncodedData();
        lowQualityImageData.ToBytes().Length.Should().BeInRange(3_500, 4_500);
        TestFixture.SaveOutput("image_resize_low_quality.jpg", lowQualityImageData);
    }
    
    [Test]
    public void ResizeAndCompress_Transparent()
    {
        using var sampleImageData = SkData.FromFile("Input/transparent-image.png");
        using var sampleImage = SkImage.FromData(sampleImageData);
        
        sampleImage.Width.Should().Be(800);
        sampleImage.Height.Should().Be(600);
        
        using var resizedImage = sampleImage.ResizeAndCompress(200, 150, 90);
        using var resizedImageData = resizedImage.GetEncodedData();
        
        resizedImage.Width.Should().Be(200);
        resizedImage.Height.Should().Be(150);
        resizedImageData.ToBytes().Length.Should().BeInRange(6_000, 8_000);
        
        TestFixture.SaveOutput("image_transparent_resized.png", resizedImageData);
        // check if image is transparent
    }
}