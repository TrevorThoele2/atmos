#include "FileHeader.h"

#include "InvalidSignature.h"

namespace Inscription
{
    void Scribe<Atmos::FileHeader>::Scriven(ObjectT& object, Inscription::Format::Binary& format)
    {
        if (format.IsOutput())
        {
            for (auto& character : object.signature)
                format(Inscription::RemoveConst(character));
        }
        else
        {
            const auto size = object.expectedSignature.size();
            std::string loadedSignature(size, '\000');
            Inscription::ContainerSize address = 0;
            while (address < size)
            {
                char character = 0;
                format(character);
                loadedSignature[address] = character;

                ++address;
            }

            if (loadedSignature != object.expectedSignature)
                throw Atmos::InvalidSignature();

            object.expectedSignature = loadedSignature;
        }

        format(object.inscriptionVersion);
        format(object.clientVersion);
    }

    void Scribe<Atmos::FileHeader>::Scriven(ObjectT& object, Inscription::Format::Json& format)
    {
        format("signature", object.signature);
        format("inscriptionVersion", object.inscriptionVersion);
        format("clientVersion", object.clientVersion);
    }
}