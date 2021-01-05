#ifndef FACTION_INFO_H
#define FACTION_INFO_H

#include "engine.h"

class FactionInfo;
extern std::vector<P<FactionInfo>> factionInfo;

enum EFactionVsFactionState
{
    FVF_Friendly,
    FVF_Neutral,
    FVF_Enemy
};

REGISTER_MULTIPLAYER_ENUM(EFactionVsFactionState);

class FactionInfo : public Updatable, public MultiplayerObject
{
private:
public:
    FactionInfo();

    sf::Color gm_color;

    /*!
     * \brief Set name of faction.
     * \param Name Name of the faction
     */
    void setName(string name) { this->name = name; if (locale_name == "") locale_name = name; }
    void setLocaleName(string name) { this->locale_name = name; }

    /*!
     * \brief Get name of faction.
     * \return String Name of the faction
     */
    string getName() { return this->name; }
    string getLocaleName() { return this->locale_name; }

    /*!
     * \brief Get description of faction.
     * \return String description of the faction
     */
    string getDescription() {return this->description;}
    /*!
     * \brief Set color of faction on GM screen.
     * \param r Red component.
     * \param g Green component.
     * \param b Blue component.
     */
    void setGMColor(int r, int g, int b) { gm_color = sf::Color(r, g, b); }
    /*!
     * \brief Set description of faction.
     * \param description
     */
    void setDescription(string description) { this->description = description; }
    /*!
     * \brief Add another faction that this faction sees as an enemy.
     * \param faction info object.
     */
    EFactionVsFactionState getStateById(unsigned int faction_id);
    void setEnemy(P<FactionInfo> other);
    /*!
     * \brief Add another faction that this faction sees as a friendly.
     * \param faction info object.
     */
    void setFriendly(P<FactionInfo> other);
    static void reset();
    virtual void update(float delta) override;

    static P<FactionInfo> getFactionById(unsigned int faction_id);

    static unsigned int findFactionId(string name);
protected:
    std::vector<EFactionVsFactionState> states;

    unsigned int index;
    string name;
    string locale_name;
    string description;
    static unsigned int next_index;
};

#endif//FACTION_INFO_H
