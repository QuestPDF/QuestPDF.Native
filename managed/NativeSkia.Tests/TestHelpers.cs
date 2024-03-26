using FluentAssertions;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

internal static class TestHelpers
{
    public static void ShouldHaveSize(this SkData data, int sizeInBytes, int buffer = 0)
    {
        data.ToBytes().Length.Should().BeInRange(sizeInBytes - buffer, sizeInBytes + buffer);
    }
}