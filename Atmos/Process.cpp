#include <sdkddkver.h>
#include <boost/process.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/process/windows.hpp>

#include "Process.h"

#include <Chroma/StringUtility.h>

namespace Atmos::Process
{
	String Extract(std::future<std::vector<char>>& future)
	{
		auto characters = future.get();
		if (characters.empty())
			return "";

		std::vector<String> lines;
		std::string line;
		boost::iostreams::stream_buffer<boost::iostreams::array_source> streamBuffer(characters.data(), characters.size());
		std::istream inStream(&streamBuffer);

		while (std::getline(inStream, line) && !line.empty())
			lines.push_back(line);

		return Chroma::Join(String("\n"), lines.begin(), lines.end());
	}

	Output Run(const String& path)
	{
		std::future<std::vector<char>> output, error;

		boost::asio::io_service service;
		boost::process::child child(
			path,
			boost::process::std_out > output,
			boost::process::std_err > error,
			service,
			boost::process::windows::hide);
		service.run();

		return Output
		{
			Extract(output),
			Extract(error)
		};
	}
}