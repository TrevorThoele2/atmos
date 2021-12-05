#include "JavaScriptCompiler.h"

#include "ScriptError.h"
#include "Process.h"

#include <Chroma/StringUtility.h>
#include <Inscription/Plaintext.h>

namespace Atmos::Scripting::JavaScript
{
	String TscProcessPath(const File::Path& outputDirectory, const std::vector<Scripting::Module>& modules)
	{
		const auto tscPath = std::filesystem::current_path() / "typescript" / "tsc.cmd";
		std::vector options =
		{
			tscPath.string()
		};

		std::vector<String> moduleNames;
		moduleNames.reserve(modules.size());
		for (auto& module : modules)
			moduleNames.push_back(module.name);

		options.insert(options.end(), moduleNames.begin(), moduleNames.end());

		options.emplace_back("atmos.d.ts");
		options.emplace_back("--target es2021");
		options.emplace_back("--strict");
		options.emplace_back("--module es2022");
		options.emplace_back("--allowJs");
		options.emplace_back("--outDir " + outputDirectory.string());
		options.emplace_back("--listEmittedFiles");
		options.emplace_back("--allowSyntheticDefaultImports");

		return Chroma::Join(String(" "), options.begin(), options.end());
	}

	std::vector<CompiledModule> ExtractCompiledModules(const String& tscOutput)
	{
		const auto emittedFiles = Chroma::Split(tscOutput, String("\r\n"));

		std::vector<CompiledModule> compiledModules;
		for (auto& emittedFile : emittedFiles)
		{
			const auto sterilizedString = Chroma::ReplaceString(Chroma::ReplaceString(emittedFile, "TSFILE: ", ""), "\r", "");
			const auto emittedFilePath = File::Path(sterilizedString);
			const auto compiledSource = Inscription::Plaintext::FromFile(emittedFilePath);

			Buffer compiledBuffer;
			for (auto& character : compiledSource)
				compiledBuffer.push_back(character);

			compiledModules.push_back(
				CompiledModule{ File::Path(emittedFilePath).replace_extension().filename().string(), compiledBuffer });
		}
		return compiledModules;
	}

	std::vector<Scripting::CompiledModule> Compile(
		const std::vector<Scripting::Module>& modules, Logging::Logger& logger)
	{
		try
		{
			const auto outputDirectory = std::filesystem::temp_directory_path() / "atmos" / "compiledScripts";
			const auto processPath = TscProcessPath(outputDirectory, modules);
			const auto output = Process::Run(processPath);

			const auto error = Chroma::Contains(output.standard, "error")
				? output.standard
				: output.error;

			if (!error.empty())
				throw Error(
					"Script compilation has encountered an error.",
					{ {"Details", error } });

			const auto compiledModules = ExtractCompiledModules(output.standard);
			std::filesystem::remove_all(outputDirectory);
			std::filesystem::remove(outputDirectory);
			return compiledModules;
		}
		catch (...)
		{
			logger.Log(
				"Compilation of script failed.",
				Logging::Severity::Information,
				{});
			throw;
		}
	}
}