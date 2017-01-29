///////////////////////////////////////////////////////////////////////////////
// USINGS
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;

///////////////////////////////////////////////////////////////////////////////
// ARGUMENTS
///////////////////////////////////////////////////////////////////////////////

var target = Argument("target", "Default");

///////////////////////////////////////////////////////////////////////////////
// SETTINGS
///////////////////////////////////////////////////////////////////////////////

var isAppVeyorBuild = AppVeyor.IsRunningOnAppVeyor;
var appveyorPlatform = EnvironmentVariable("AppveyorPlatform") ?? "";
var appveyorConfiguration = EnvironmentVariable("AppveyorConfiguration") ?? "";
var platforms = new [] { "Win32", "x64" }.ToList();
var configurations = new [] { "Debug", "Release" }.ToList();
var solution = "./BatchEncoder.sln";
var versionHeaderPath = (FilePath)File("./src/version.h");
var installerScript = MakeAbsolute((FilePath)File("./setup/setup.iss"));
var iscc = @"C:/Program Files (x86)/Inno Setup 5/ISCC.exe";
var artifactsDir = (DirectoryPath)Directory("./artifacts");
var binDir = (DirectoryPath)Directory("./src/bin");
var objDir = (DirectoryPath)Directory("./src/obj");

///////////////////////////////////////////////////////////////////////////////
// ACTIONS
///////////////////////////////////////////////////////////////////////////////

var buildSolutionAction = new Action<string,string> ((configuration, platform) => 
{
    Information("Building: {0}, {1} / {2}", solution, configuration, platform);
    MSBuild(solution, settings => {
        settings.SetConfiguration(configuration);
        settings.WithProperty("Platform", "\"" + platform + "\"");
        settings.SetVerbosity(Verbosity.Minimal); });
});

var packageBinariesAction = new Action<string,string> ((configuration, platform) => 
{
    var path = "./src/bin/" + configuration + "/" + platform + "/";
    var output = "BatchEncoder-" + version + "-" + platform + (configuration == "Release" ? "" : ("-(" + configuration + ")"));
    var outputDir = artifactsDir.Combine(output);
    var outputZip = artifactsDir.CombineWithFilePath(output + ".zip");
    var exeFile = File(path + "BatchEncoder.exe");
    CleanDirectory(outputDir);
    CopyFileToDirectory(File("README.md"), outputDir);
    CopyFileToDirectory(File("LICENSE.TXT"), outputDir);
    CopyFileToDirectory(exeFile, outputDir);
    CopyFiles(path + "*.progress", outputDir);
    CopyFiles("./config/*.options", outputDir);
    CopyFiles("./config/*.formats", outputDir);
    CopyFiles("./config/*.items", outputDir);
    CopyFiles("./scripts/*.ps1", outputDir);
    Zip(outputDir, outputZip);
});

var packageInstallersAction = new Action<string,string> ((configuration, platform) => 
{
    StartProcess(iscc, new ProcessSettings { 
        Arguments = 
            "\"" + installerScript.FullPath + "\""
            + " /DCONFIGURATION=" + configuration
            + " /DBUILD=" + platform
            + " /DVERSION=" + version, 
        WorkingDirectory = MakeAbsolute(artifactsDir) });
});

///////////////////////////////////////////////////////////////////////////////
// VERSION
///////////////////////////////////////////////////////////////////////////////

var isLocalBuild = BuildSystem.IsLocalBuild;
var isPullRequest = BuildSystem.AppVeyor.Environment.PullRequest.IsPullRequest;
var isMainRepo = StringComparer.OrdinalIgnoreCase.Equals("wieslawsoltes/BatchEncoder", BuildSystem.AppVeyor.Environment.Repository.Name);
var isMasterBranch = StringComparer.OrdinalIgnoreCase.Equals("master", BuildSystem.AppVeyor.Environment.Repository.Branch);
var isTagged = BuildSystem.AppVeyor.Environment.Repository.Tag.IsTag 
               && !string.IsNullOrWhiteSpace(BuildSystem.AppVeyor.Environment.Repository.Tag.Name);
var isRelease =  !isLocalBuild && !isPullRequest && isMainRepo && isMasterBranch && isTagged;

var text = System.IO.File.ReadAllText(versionHeaderPath.FullPath);
var split = text.Split(new char [] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
var major = split[0].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var minor = split[1].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var revision = split[2].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var build = split[3].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var version = major + "." + minor + (isRelease ? "" : "-alpha");

Information("Defined Version: {0}.{1}.{2}.{3}", major, minor, revision, build);
Information("Release Version: {0}", version);

///////////////////////////////////////////////////////////////////////////////
// TASKS
///////////////////////////////////////////////////////////////////////////////

Task("Clean")
    .Does(() =>
{
    CleanDirectory(artifactsDir);
    CleanDirectory(binDir);
    CleanDirectory(objDir);
});

Task("Build")
    .IsDependentOn("Clean")
    .Does(() =>
{
    if (isAppVeyorBuild)
        buildSolutionAction(appveyorConfiguration, appveyorPlatform);
    else
        configurations.ForEach(configuration => platforms.ForEach(platform => buildSolutionAction(configuration, platform)));
});

Task("Package-Binaries")
    .IsDependentOn("Build")
    .Does(() =>
{
    if (isAppVeyorBuild)
        packageBinariesAction(appveyorConfiguration, appveyorPlatform);
    else
        configurations.ForEach(configuration => platforms.ForEach(platform => packageBinariesAction(configuration, platform)));
});

Task("Package-Installers")
    .IsDependentOn("Build")
    .Does(() =>
{
    if (isAppVeyorBuild)
        packageInstallersAction(appveyorConfiguration, appveyorPlatform);
    else
        configurations.ForEach(configuration => platforms.ForEach(platform => packageInstallersAction(configuration, platform)));
});

///////////////////////////////////////////////////////////////////////////////
// TARGETS
///////////////////////////////////////////////////////////////////////////////

Task("Package")
  .IsDependentOn("Package-Binaries")
  .IsDependentOn("Package-Installers");

Task("Default")
  .IsDependentOn("Build");

///////////////////////////////////////////////////////////////////////////////
// EXECUTE
///////////////////////////////////////////////////////////////////////////////

RunTarget(target);
