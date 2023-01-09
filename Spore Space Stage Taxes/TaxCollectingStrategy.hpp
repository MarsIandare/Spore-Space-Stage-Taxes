
#ifndef TAXCOLLECTINGSTRATEGY_HPP
#define TAXCOLLECTINGSTRATEGY_HPP

#include <Spore\BasicIncludes.h>

#include <chrono>
#include <vector>
#include <filesystem>

enum class MessageID : uint32_t
{
    OnPauseToggled = 0x3867294,
};

class TaxCollectingStrategy :
    public App::IUnmanagedMessageListener,
    public Simulator::ISimulatorStrategy
{
private:
    int m_RefCount = 0;
    int m_ha = 0;
    int Colonies = 0;
    int Times = 0;
    bool IsPaused = false;

    std::chrono::time_point<std::chrono::high_resolution_clock> Timer;

public:
    TaxCollectingStrategy(int hi)
        : m_ha(hi)
    {
    }

    int AddRef() override { return ++m_RefCount; }
    int Release() override { if (--m_RefCount == 0) { delete this; return 0; }; return m_RefCount; }

    void Initialize() override { }
    void Dispose() override { }

    const char* GetName() const override { return "TaxCollectingStrategy"; }

    void OnModeExited(uint32_t previousModeID, uint32_t newModeID) override { }
    void OnModeEntered(uint32_t previousModeID, uint32_t newModeID) override;

    uint32_t GetLastGameMode() const override { return 0; }
    uint32_t GetCurrentGameMode() const override { return 0; }

    bool Write(Simulator::ISerializerStream*) override { return false; }
    bool Read(Simulator::ISerializerStream*) override { return false; }

    void OnLoad(const Simulator::cSavedGameHeader& savedGame) override { }
    bool WriteToXML(Simulator::XmlSerializer*) override { return false; }

    bool func24h(uint32_t) override { return false; }

    void Update(int deltaTime, int deltaGameTime) override;
    void PostUpdate(int deltaTime, int deltaGameTime) override { }

    void func40h(uint32_t) override { }
    void func44h(uint32_t) override { }
    void func48h() override { }
    void func4Ch() override { }

    std::vector<uint32_t> GetHandledMessageIDs(void);
    bool HandleMessage(uint32_t messageID, void* message) override;
};

#endif