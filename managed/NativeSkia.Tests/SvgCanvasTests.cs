using FluentAssertions;
using NUnit.Framework;
using QuestPDF.Skia;
using QuestPDF.Skia.Text;

namespace NativeSkia.Tests;

public class SvgCanvasTests
{
    [Test]
    public void Output()
    {
        // read bitmap
        using var imageData = SkData.FromFile("Input/image.jpg");
        using var image = SkImage.FromData(imageData);
        
        // load SVG
        var svgContent = File.ReadAllText("Input/icon.svg");
        using var svgImage = new SkSvgImage(svgContent, SkFontManager.Global);
        
        // draw svg in a pdf document
        using var svgOutputStream = new SkWriteStream();
        using var svgCanvas = SkSvgCanvas.CreateSvg(800, 600, svgOutputStream);
        
        svgCanvas.DrawImage(image, 800, 600);
        svgCanvas.DrawSvg(svgImage, 800, 600);
        
        svgCanvas.Dispose();
        
        // save
        using var svgData = svgOutputStream.DetachData();
        TestFixture.SaveOutput("canvas_svg.svg", svgData);
        svgData.ShouldHaveSize(366_086);
    }
}