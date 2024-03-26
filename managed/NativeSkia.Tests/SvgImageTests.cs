using FluentAssertions;
using NUnit.Framework;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

public class SvgImageTests
{
    [Test]
    public void Load()
    {
        var svgContent = File.ReadAllText("Input/icon.svg");
        using var svg = new SkSvgImage(svgContent);

        svg.Instance.Should().NotBe(0);
        svg.ViewBox.Should().Be(new SkRect(0, 0, 76f, 93f));
    }

    [Test]
    public void Svg()
    {
        // read SVG
        var svgContent = File.ReadAllText("Input/icon.svg");
        using var svg = new SkSvgImage(svgContent);
        
        // create document
        using var stream = new SkWriteStream();
        using var pdf = SkPdfDocument.Create(stream, new SkPdfDocumentMetadata() { CompressDocument = true });
        
        // draw svg in a pdf document
        using var pageCanvas = pdf.BeginPage(800, 600);
        pageCanvas.DrawSvg(svg, 400, 600);
        
        pdf.EndPage();
        pdf.Close();

        using var documentData = stream.DetachData();
        TestFixture.SaveOutput("document_svg.pdf", documentData);
        documentData.ShouldHaveSize(3_314);
    }
}