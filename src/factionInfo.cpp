#include "factionInfo.h"

REGISTER_SCRIPT_CLASS(FactionInfo)
{
    REGISTER_SCRIPT_CLASS_FUNCTION(FactionInfo, setName);
    REGISTER_SCRIPT_CLASS_FUNCTION(FactionInfo, setLocaleName);
    REGISTER_SCRIPT_CLASS_FUNCTION(FactionInfo, setGMColor);
    REGISTER_SCRIPT_CLASS_FUNCTION(FactionInfo, setDescription);
    REGISTER_SCRIPT_CLASS_FUNCTION(FactionInfo, setEnemy);
    REGISTER_SCRIPT_CLASS_FUNCTION(FactionInfo, setFriendly);
}

// if factionIndex was a PVector then users could call foreach
// which in turn may remove an (check) element?, the indexes of the
// factionInfo are important information, used as faction_id
// this is why it is declared as a std::vector rather than a PVector
std::vector<P<FactionInfo>> factionInfo;

REGISTER_MULTIPLAYER_CLASS(FactionInfo,"FactionInfo");
FactionInfo::FactionInfo()
:MultiplayerObject("FactionInfo")
{
    registerMemberReplication(&index);
    registerMemberReplication(&name);
    registerMemberReplication(&locale_name);
    registerMemberReplication(&states);
    registerMemberReplication(&gm_color);

    for(auto faction : factionInfo)
    {
        if (faction)
        {
            faction->states.push_back(FVF_Neutral);
        }
    }

    if (game_server) {
        index=next_index++;

        assert(factionInfo.size()==index);
        factionInfo.push_back(this);
    }

    for(unsigned int n = 0; n < factionInfo.size(); n++)
        states.push_back(FVF_Neutral);
    for(unsigned int n = 0; n < factionInfo.size(); n++)
        if (factionInfo[n] == this)
            states[n] = FVF_Friendly;
}

void FactionInfo::setEnemy(P<FactionInfo> other)
{
    if (!other)
    {
        LOG(WARNING) << "Tried to set a an undefined faction to enemy with " << name;
        return;
    }

    int id1 = -1;
    int id2 = -1;
    for(unsigned int n = 0; n < factionInfo.size(); n++)
    {
        if (factionInfo[n] == this)
            id1 = n;
        if (factionInfo[n] == other)
            id2 = n;
    }
    if (id1 != -1 && id2 != -1)
    {
        factionInfo[id1]->states[id2] = FVF_Enemy;
        factionInfo[id2]->states[id1] = FVF_Enemy;
    }
}

void FactionInfo::setFriendly(P<FactionInfo> other)
{
    if (!other)
    {
        LOG(WARNING) << "Tried to set a an undefined faction to friendly with " << name;
        return;
    }

    int id1 = -1;
    int id2 = -1;
    for(unsigned int n = 0; n < factionInfo.size(); n++)
    {
        if (factionInfo[n] == this)
            id1 = n;
        if (factionInfo[n] == other)
            id2 = n;
    }
    if (id1 != -1 && id2 != -1)
    {
        factionInfo[id1]->states[id2] = FVF_Friendly;
        factionInfo[id2]->states[id1] = FVF_Friendly;
    }
}

P<FactionInfo> FactionInfo::getFactionById(unsigned int faction_id)
{
    if (faction_id<factionInfo.size())
    {
        return factionInfo[faction_id];
    }
    return nullptr;
}

EFactionVsFactionState FactionInfo::getStateById(unsigned int faction_id)
{
    // when state is replicated from server to client there is a small
    // window where it is possible for faction_id to refer to factions
    // which the state has not replicated from the network
    if (faction_id<states.size())
    {
        return states[faction_id];
    }
    return FVF_Neutral;
}

unsigned int FactionInfo::findFactionId(string name)
{
    for(unsigned int n = 0; n < factionInfo.size(); n++)
        if (factionInfo[n]->name == name)
            return n;
    LOG(ERROR) << "Failed to find faction: " << name;
    return 0;
}

void FactionInfo::update(float delta)
{
    if (index+1>factionInfo.size()) {
        factionInfo.resize(index+1);
    }
    factionInfo[index]=this;
}

void FactionInfo::reset()
{
    next_index=0;
    for (auto faction : factionInfo)
    {
        if (faction)
        {
            faction->destroy();
        }
    }
    factionInfo.clear();
}

unsigned int FactionInfo::next_index=0;
