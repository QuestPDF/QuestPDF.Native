using System.Diagnostics;
using FluentAssertions;
using NUnit.Framework;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

public class PdfDocumentTests
{
    [Test]
    public void EmptyMetadata()
    {
        var metadata = new SkPdfDocumentMetadata();
        using var stream = new SkWriteStream();
        using var pdf = SkPdfDocument.Create(stream, metadata);
        
        using var canvas = pdf.BeginPage(400, 600);
        
        canvas.DrawFilledRectangle(new SkRect(50, 50, 350, 550), 0xFF673AB7);

        pdf.EndPage();
        pdf.Close();

        using var documentData = stream.DetachData();
        TestFixture.SaveOutput("document_without_metadata.pdf", documentData);
        documentData.ShouldHaveSize(709);
    }
    
    [Test]
    public void Metadata()
    {
        var metadata = new SkPdfDocumentMetadata
        {
            Title = new SkText("Native Skia Generation Test"),
            Author = new SkText("Marcin Ziąbek 🎉"),
            Subject = new SkText("Polski: ąśćźżółń"),
            Keywords = new SkText("keywords"),
            Creator = new SkText("Japanese creator: 日本語"),
            Producer = new SkText("Arabic producer: اللغة العربية"),
            
            CreationDate = new SkDateTime(DateTimeOffset.Now - TimeSpan.FromHours(1234)),
            ModificationDate = new SkDateTime(DateTimeOffset.Now - TimeSpan.FromHours(123)),
            
            SupportPDFA = true,
            CompressDocument = true,
            
            RasterDPI = 123,
            ImageEncodingQuality = 90
        };
        
        using var stream = new SkWriteStream();
        using var pdf = SkPdfDocument.Create(stream, metadata);
        
        using var canvas = pdf.BeginPage(400, 600);
        
        canvas.DrawFilledRectangle(new SkRect(50, 50, 350, 550), 0xFF673AB7);
        canvas.DrawFilledRectangle(new SkRect(100, 100, 300, 500), 0xFF3F51B5);
        canvas.DrawFilledRectangle(new SkRect(150, 150, 250, 450), 0xFF2196F3);
        
        pdf.EndPage();
        pdf.Close();

        using var documentData = stream.DetachData();
        TestFixture.SaveOutput("simple_document.pdf", documentData);
        documentData.ShouldHaveSize(6_020);
    }
    
    [Test]
    public void AnnotationUrl()
    {
        using var webpageImage = SkImage.FromData(SkData.FromFile("Input/webpage.jpg"));

        using var stream = new SkWriteStream();
        using var pdf = SkPdfDocument.Create(stream, new SkPdfDocumentMetadata());
        
        using var canvas = pdf.BeginPage(700, 340);
        
        canvas.Translate(50, 50);
        canvas.DrawImage(webpageImage, 600, 240);
        canvas.AnnotateUrl(600, 240, "https://www.questpdf.com");
        
        pdf.EndPage();
        pdf.Close();

        using var documentData = stream.DetachData();
        TestFixture.SaveOutput("document_with_url.pdf", documentData);
        documentData.ShouldHaveSize(249_678);
    }
    
    [Test]
    public void InternalAnnotationAndLink()
    {
        using var webpageImage = SkImage.FromData(SkData.FromFile("Input/webpage.jpg"));

        using var stream = new SkWriteStream();
        using var pdf = SkPdfDocument.Create(stream, new SkPdfDocumentMetadata());
        
        // first page
        using var firstPageCanvas = pdf.BeginPage(300, 500);
        
        firstPageCanvas.Translate(100, 200);
        firstPageCanvas.DrawFilledRectangle(new SkRect(0, 0, 100, 100), 0xFF673AB7);
        firstPageCanvas.AnnotateDestinationLink(100, 100, "page_2_destination");
        
        pdf.EndPage();
        
        // second page
        using var secondPageCanvas = pdf.BeginPage(300, 500);
        
        secondPageCanvas.Translate(100, 200);
        secondPageCanvas.DrawFilledRectangle(new SkRect(0, 0, 100, 100), 0xFF2196F3);
        secondPageCanvas.AnnotateDestination("page_2_destination");
        
        pdf.EndPage();
        pdf.Close();

        using var documentData = stream.DetachData();
        TestFixture.SaveOutput("document_with_internal_destination_and_link.pdf", documentData);
        documentData.ShouldHaveSize(1_375);
    }
}