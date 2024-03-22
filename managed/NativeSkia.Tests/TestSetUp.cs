using NUnit.Framework;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

[SetUpFixture]
public class TestSetUp
{
    [OneTimeSetUp]
    public static void Setup()
    {
        SkNativeDependencyCompatibilityChecker.Test();
    }
}