#pragma once

#include "Script.h"

namespace Atmos
{
    class ScriptController
    {
    private:
        struct Piece
        {
            union U
            {
                Script::Instance *instance;
                Script::Instance owned;
                U();
                U(Script::Instance *set);
                U(Script::Instance &&set);
                ~U();
            };

            U u;
            bool owns;

            Script::SymbolName executeSymbol;
            Script::Instance::ItemVector parameters;

            bool justAdded;
            bool executeMain;

            Piece(Script::Instance &instance);
            Piece(Script::Instance &instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters);
            Piece(Script::Instance &&instance);
            Piece(Script::Instance &&instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters);
            Piece(Piece &&arg);
            Piece& operator=(Piece &&arg);
            Script::Instance* GetInstance();
            const Script::Instance* GetInstance() const;
        };
    private:
        bool isWorking;
        typedef std::vector<Piece> Stack;
        Stack stack;
        Stack::iterator current;
        std::vector<Stack::iterator> deferredRemove;

        ScriptController();
        ScriptController(const ScriptController &arg) = delete;
        ScriptController& operator=(const ScriptController &arg) = delete;

        static Stack::iterator Find(const Script::Instance &find);
        static Stack::const_iterator FindConst(const Script::Instance &find);
    public:
        static ScriptController& Instance();
        static void Add(Script::Instance &instance);
        static void Add(Script::Instance &instance, const Script::SymbolName &executeSymbol, const Script::Instance::ItemVector &parameters);
        static void Remove(Script::Instance &instance);

        static void Work();
        static bool IsRunning(const Script::Instance &check);
        static Script::Instance* Current();
    };
}