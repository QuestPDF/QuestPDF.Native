using FluentAssertions;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

internal static class TestHelpers
{
    public static void ShouldHaveSize(this SkData data, int sizeInBytes)
    {
        data.ToBytes().Length.Should().Be(sizeInBytes);
    }
}