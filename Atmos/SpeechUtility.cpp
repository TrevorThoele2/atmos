
/*
#include "SpeechUtility.h"

#include "SpeechBranch.h"
#include "SpeechInput.h"
#include "SpeechOutput.h"
#include "SpeechRandomSwitch.h"
#include "SpeechSwitch.h"
#include "SpeechFunction.h"
#include "SpeechReset.h"
#include "SpeechWaitFunction.h"
#include "SpeechSimultaneous.h"

#include "NPC.h"

namespace Atmos
{
    namespace Speech
    {
        template<class Part>
        Atmos::speech::Part* SetupPrototypes(Part &part, const std::vector<Output::Prototype> &prototypes)
        {
            if (prototypes.empty())
                return nullptr;
            else if (prototypes.size() == 1)
                return part.CreatePart<Output>(*prototypes.begin());
            else if (prototypes.size() > 1)
            {
                auto newSwitch = part.CreatePart<RandomSwitch<unsigned char>>();
                for (auto loop = prototypes.begin(); loop != prototypes.end(); loop++)
                    newSwitch->CreatePart<Output>(*loop);

                return newSwitch;
            }

            return nullptr;
        }

        template<class Part, class ID>
        Atmos::speech::Part* SetupPrototypes(Part &part, ID id, const std::vector<Output::Prototype> &prototypes)
        {
            if (prototypes.empty())
                return nullptr;
            else if (prototypes.size() == 1)
                return part.CreatePart<Output>(id, *prototypes.begin());
            else if (prototypes.size() > 1)
            {
                auto newSwitch = part.CreatePart<RandomSwitch<unsigned char>>(id);
                for (auto loop = prototypes.begin(); loop != prototypes.end(); loop++)
                    newSwitch->CreatePart<Output>(*loop);

                return newSwitch;
            }

            return nullptr;
        }

        template<class T>
        void SetupShopkeeperEnter(Input &input, const std::string &string, bool buying, std::vector<Output::Prototype> prototypes, T &reset, Branch &outerBranch)
        {
            for (auto loop = prototypes.begin(); loop != prototypes.end(); loop++)
            {
                loop->reset = false;
                loop->useSpeed = loop->speed;
                loop->endingWait = 0;
            }

            auto branch = input.CreatePart<Branch>(string);
            auto simul = branch->CreatePart<Simultaneous>();
            simul->CreatePart<Function>(&Shop::Enter, buying);
            SetupPrototypes(*simul, prototypes);
            // Setup spinlock and exit  
            branch->CreatePart<WaitFunction>(&Shop::IsActive, true);
            // Simultaneously jump back to the asking output and reset it
            simul = branch->CreatePart<Simultaneous>();
            simul->CreatePart<Branch::Jump>(outerBranch, reset);
            simul->CreatePart<Reset>(reset);
        }

        void MakeShopkeeperDialogue(NPC &character, const OutputPrototypes &greetings, const OutputPrototypes &enterQuestions, const std::string &buy, const std::string &sell, const std::string &leave, const OutputPrototypes &leaving, OutputPrototypes onEnterBuy, OutputPrototypes onEnterSell, const OutputPrototypes &buySuccess, const OutputPrototypes &sellSuccess)
        {
            auto &dialogue = character.dialogue;
            SetupPrototypes(dialogue, greetings);
            auto reset = SetupPrototypes(dialogue, enterQuestions);

            // Setup input
            auto input = dialogue.CreatePart<Input>();
            // Buy transaction
            SetupShopkeeperEnter(*input, buy, true, onEnterBuy, *reset, *dialogue);
            // Sell transaction
            SetupShopkeeperEnter(*input, sell, false, onEnterSell, *reset, *dialogue);
            // Leave
            auto branch = input->CreatePart<Branch>(leave);
            SetupPrototypes(*branch, leaving);
        }

        void MakeInnkeeperDialogue(NPC &character, InnkeeperDeterminer::Cost cost, const OutputPrototypes &greetings, const OutputPrototypes &asking, const std::string &yes, const std::string &no, const OutputPrototypes &notEnoughGold, const OutputPrototypes &worked, const OutputPrototypes &onNo)
        {
            auto &dialogue = character.dialogue;
            SetupPrototypes(dialogue, greetings);
            SetupPrototypes(dialogue, asking);

            // Setup input
            auto input = dialogue.CreatePart<Input>();
            // Yes
            auto branch = input->CreatePart<Branch>(yes);
            auto check = branch->CreatePart<Switch<InnkeeperFlags>>(&InnkeeperDeterminer::operator(), InnkeeperDeterminer(cost));
            SetupPrototypes(*check, InnkeeperFlags::NOT_ENOUGH_GOLD, notEnoughGold);
            SetupPrototypes(*check, InnkeeperFlags::OK, worked);
            // No
            branch = input->CreatePart<Branch>(no);
            SetupPrototypes(*branch, onNo);
        }

        void MakeRecruitableDialogue(NPC &character, RecruitableDeterminer::Cost cost, const OutputPrototypes &greetings, const OutputPrototypes &alreadyRecruited, const OutputPrototypes &asking, const std::string &yes, const std::string &no, const OutputPrototypes &onYes, const OutputPrototypes &onNo)
        {
            SetupPrototypes(character.dialogue, greetings);
            // Check if already recruited
            auto check = character.dialogue.CreatePart<Switch<bool>>(&Recruitable::IsAlreadyRecruited, character);
            auto notRecruited = check->CreatePart<Branch>();
            SetupPrototypes(*check, alreadyRecruited);

            SetupPrototypes(*notRecruited, asking);
            auto input = notRecruited->CreatePart<Input>();
            // Yes
            auto branch = input->CreatePart<Branch>(yes);
            branch->CreatePart<Function>(&Recruitable::Recruit, character);
            SetupPrototypes(*branch, onYes);
            // No
            branch = input->CreatePart<Branch>(no);
            SetupPrototypes(*branch, onNo);
        }
    }
}
*/