#include "ArchiveHeader.h"

#include "InvalidSignature.h"

namespace Atmos
{
    void SaveArchiveHeader(
        Inscription::Archive::Binary& archive,
        const std::string& signature,
        Inscription::Version inscriptionVersion,
        Inscription::Version clientVersion)
    {
        for (auto& character : signature)
            archive(Inscription::RemoveConst(character));

        archive(inscriptionVersion);
        archive(clientVersion);
    }

    std::tuple<Inscription::Version, Inscription::Version> LoadArchiveHeader(
        Inscription::Archive::Binary& archive, const std::string& expectedSignature)
    {
        const auto size = expectedSignature.size();
        std::string loadedSignature(size, '\000');
        Inscription::ContainerSize address = 0;
        while (address < size)
        {
            char character = 0;
            archive(character);
            loadedSignature[address] = character;

            ++address;
        }

        if (loadedSignature != expectedSignature)
            throw InvalidSignature();

        Inscription::Version inscriptionVersion;
        Inscription::Version clientVersion;
        archive(inscriptionVersion);
        archive(clientVersion);

        return { inscriptionVersion, clientVersion };
    }

    void SaveArchiveHeader(
        Inscription::Archive::Json& archive,
        Inscription::Version inscriptionVersion,
        Inscription::Version clientVersion)
    {
        archive("inscriptionVersion", inscriptionVersion);
        archive("clientVersion", clientVersion);
    }

    std::tuple<Inscription::Version, Inscription::Version> LoadArchiveHeader(Inscription::Archive::Json& archive)
    {
        Inscription::Version inscriptionVersion;
        Inscription::Version clientVersion;
        archive("inscriptionVersion", inscriptionVersion);
        archive("clientVersion", clientVersion);

        return { inscriptionVersion, clientVersion };
    }
}