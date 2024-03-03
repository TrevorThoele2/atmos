#pragma once

#include <string>

#include <Inscription/BinaryArchive.h>
#include <Inscription/JsonArchive.h>
#include <Inscription/Version.h>

namespace Atmos
{
    void SaveArchiveHeader(
        Inscription::Archive::Binary& archive,
        const std::string& signature,
        Inscription::Version inscriptionVersion,
        Inscription::Version clientVersion);
    std::tuple<Inscription::Version, Inscription::Version> LoadArchiveHeader(
        Inscription::Archive::Binary& archive,
        const std::string& expectedSignature);

    void SaveArchiveHeader(
        Inscription::Archive::Json& archive,
        Inscription::Version inscriptionVersion,
        Inscription::Version clientVersion);
    std::tuple<Inscription::Version, Inscription::Version> LoadArchiveHeader(
        Inscription::Archive::Json& archive);
}