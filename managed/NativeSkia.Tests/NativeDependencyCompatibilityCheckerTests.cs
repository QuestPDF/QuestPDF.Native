using NUnit.Framework;
using QuestPDF.Skia;

namespace NativeSkia.Tests;

public class NativeDependencyCompatibilityCheckerTests
{
    [Test]
    public void Perform()
    { 
        SkNativeDependencyCompatibilityChecker.Test();
    }
}