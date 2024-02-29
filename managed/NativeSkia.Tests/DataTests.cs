using FluentAssertions;
using NUnit.Framework;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

public class DataTests
{
    [Test]
    public void CreateFromFile()
    {
        var path = Path.Combine(TestFixture.InputPath, "image.jpg");
     
        // read file via C# API
        var expectedContent = File.ReadAllBytes(path);
        expectedContent.Length.Should().Be(30_167);
        
        // // read file via NativeSkia API
        using var data = SkData.FromFile(path);
        var actualContent = data.ToBytes();
        
        // compare
        actualContent.Should().BeEquivalentTo(expectedContent);
    }
    
    [Test]
    public void CreateFromBinary()
    {
        var path = Path.Combine(TestFixture.InputPath, "image.jpg");
     
        // read file via C# API
        var expectedContent = File.ReadAllBytes(path);
        expectedContent.Length.Should().Be(30_167);
        
        // read file via NativeSkia API
        using var data = SkData.FromBinary(expectedContent);
        var actualContent = data.ToBytes();
        
        // compare
        actualContent.Should().BeEquivalentTo(expectedContent);
    }
}