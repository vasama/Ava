using Sharpmake;

namespace Extern
{
	[Export]
	public class Doctest : Project
	{
		public Doctest()
		{
			AddTargets(
				new Target(
					Platform.win32 | Platform.win64,
					DevEnv.vs2017,
					Optimization.Debug | Optimization.Release
				)
			);
		}

		[Configure]
		public void ConfigureAll(Configuration conf, Target target)
		{
			conf.IncludePaths.Add("ThirdParty/doctest");
			conf.ExportDefines.Add("DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS");
		}
	}
}

public abstract class Common : Project
{
	public Common()
	{
		AddTargets(
			new Target(
				Platform.win32 | Platform.win64,
				DevEnv.vs2017,
				Optimization.Debug | Optimization.Release
			)
		);

		IsFileNameToLower = false;
		IsTargetFileNameToLower = false;
		
		SourceFilesExtensions.Add(".ipp");
	}

	[Configure]
	public virtual void ConfigureAll(Configuration conf, Target target)
	{
		conf.ProjectFileName = "[project.Name].[target.DevEnv]";
		conf.TargetPath = "[conf.ProjectPath]/bin/[target.Platform]-[conf.Name]";
		conf.TargetLibraryPath = "[conf.ProjectPath]/lib/[target.Platform]-[conf.Name]";

		conf.Options.Add(Options.Vc.Compiler.Exceptions.Disable);
		conf.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP17);

		conf.Options.Add(Options.Vc.General.WarningLevel.Level4);
		conf.Options.Add(Options.Vc.General.TreatWarningsAsErrors.Enable);

		// Anonymous struct
		conf.Options.Add(new Options.Vc.Compiler.DisableSpecificWarnings("4201"));

		conf.Defines.Add("_HAS_EXCEPTIONS=0");
		conf.Defines.Add("_CRT_SECURE_NO_WARNINGS");
		conf.Defines.Add("_SCL_SECURE_NO_WARNINGS");
	}
}

[Generate]
public class Ava : Common
{
	public Ava()
	{
		Name = "Ava";

		SourceFilesBuildExcludeRegex.Add(@"\.test\.cpp$");
	}

	public override void ConfigureAll(Configuration conf, Target target)
	{
		base.ConfigureAll(conf, target);

		conf.IncludePaths.Add("Public");
		conf.IntermediatePath = "[conf.ProjectPath]/build/[target.Platform]-[conf.Name]";

		if (target.Optimization == Optimization.Release)
		{
			conf.ExportDefines.Add("Ava_CONFIG_ASSERT=0");
		}

		if (target.OutputType == OutputType.Dll)
		{
			conf.Output = Configuration.OutputType.Dll;
			conf.ExportDefines.Add("Ava_CONFIG_DLL");
			conf.Defines.Add("Ava_CONFIG_EXPORT");
		}
		else if (target.OutputType == OutputType.Lib)
		{
			conf.Output = Configuration.OutputType.Lib;
		}
	}
}

[Generate]
public class UnitTest : Common
{
	public UnitTest()
	{
		Name = "Ava-UnitTest";

		SourceFilesBuildFiltersRegex.Add(@"\.test\.cpp$");
	}

	public override void ConfigureAll(Configuration conf, Target target)
	{
		base.ConfigureAll(conf, target);

		conf.IntermediatePath = "[conf.ProjectPath]/build/test/[target.Platform]-[conf.Name]";

		conf.AddPrivateDependency<Ava>(target);
		conf.AddPrivateDependency<Extern.Doctest>(target);

		conf.IncludePaths.Add("Private");

		conf.Options.Add(Options.Vc.Compiler.Inline.Disable);
	//	conf.Options.Add(Options.Vc.Linker.GenerateDebugInformation.EnableFastLink);
	}
}

[Generate]
public class AvaSln : Solution
{
	public AvaSln()
	{
		Name = "Ava";

		AddTargets(
			new Target(
				Platform.win32 | Platform.win64,
				DevEnv.vs2017,
				Optimization.Debug | Optimization.Release
			)
		);

		IsFileNameToLower = false;
	}

	[Configure]
	public void ConfigureAll(Configuration conf, Target target)
	{
		conf.SolutionFileName = "[solution.Name].[target.DevEnv]";

		conf.AddProject<Ava>(target);
		conf.AddProject<UnitTest>(target);
	}

	[Main]
	public static void Main(Arguments args)
	{
		args.Generate<AvaSln>();
	}
}
