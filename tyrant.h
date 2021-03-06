#ifndef TYRANT_H_INCLUDED
#define TYRANT_H_INCLUDED

#define TYRANT_OPTIMIZER_VERSION "2.4.2"

#include <string>
#include <sstream>
#include <unordered_set>
#include <tuple>

enum Faction
{
    allfactions,
    imperial,
    raider,
    bloodthirsty,
    xeno,
    righteous,
    progenitor,
    num_factions
};
extern const std::string faction_names[num_factions];

enum Skill
{
    // Placeholder for no-skill:
    no_skill,
    // Attack:
    attack,
    // Activation:
    BEGIN_ACTIVATION_HARMFUL, // TODO skill traits
    enfeeble, jam, siege, strike, weaken, 
    END_ACTIVATION_HARMFUL,
    BEGIN_ACTIVATION_HELPFUL,
    enhance, heal, overload, protect, rally, 
    END_ACTIVATION_HELPFUL,
    // Defensive:
    BEGIN_DEFENSIVE,
    armor, corrosive, counter, evade, payback, wall,
    END_DEFENSIVE,
    // Combat-Modifier:
    flurry, pierce, valor,
    // Damage-Dependant:
    berserk, inhibit, leech, poison,
    // Triggered:
    legion,
    // Pseudo-Skill for BGE:
    BEGIN_BGE_SKILL,
    bloodlust, reaping, 
    END_BGE_SKILL,
    num_skills
};
extern std::string skill_names[num_skills];

namespace CardType {
enum CardType {
    commander,
    assault,
    structure,
    num_cardtypes
};
}

extern std::string cardtype_names[CardType::num_cardtypes];

extern std::string rarity_names[];

extern unsigned upgrade_cost[];
extern unsigned salvaging_income[][7];

namespace DeckType {
enum DeckType {
    deck,
    mission,
    raid,
    quest,
    custom_deck,
    num_decktypes
};
}

extern std::string decktype_names[DeckType::num_decktypes];

enum Effect {
    none,
    metamorphosis,
    num_effects
};

extern std::string effect_names[Effect::num_effects];

enum gamemode_t
{
    fight,
    surge,
};

//MDJ
enum class OptimizationMode
{
    notset,
    winrate,
    defense,
    war,
	totalwar,
    brawl,
    raid,
};

struct true_ {};

struct false_ {};

template<unsigned>
struct skillTriggersRegen { typedef false_ T; };

template<>
struct skillTriggersRegen<strike> { typedef true_ T; };

template<>
struct skillTriggersRegen<siege> { typedef true_ T; };

enum SkillSourceType
{
    source_hostile,
    source_allied,
    source_global_hostile,
    source_global_allied,
    source_chaos
};

struct SkillSpec
{
    Skill id;
    unsigned x;
    Faction y;
    unsigned n;
    unsigned c;
    Skill s;
    bool all;
};

// --------------------------------------------------------------------------------
// Common functions
template<typename T>
std::string to_string(const T val)
{
    std::stringstream s;
    s << val;
    return s.str();
}

//---------------------- Debugging stuff ---------------------------------------
extern signed debug_print;
extern unsigned debug_cached;
extern bool debug_line;
extern std::string debug_str;
#ifndef NDEBUG
#define _DEBUG_MSG(v, format, args...)                                  \
    {                                                                   \
        if(__builtin_expect(debug_print >= v, false))                   \
        {                                                               \
            if(debug_line) { printf("%i - " format, __LINE__ , ##args); }      \
            else if(debug_cached) {                                     \
                char buf[4096];                                         \
                snprintf(buf, sizeof(buf), format, ##args);             \
                debug_str += buf;                                       \
            }                                                           \
            else { printf(format, ##args); }                            \
            std::cout << std::flush;                                    \
        }                                                               \
    }
#define _DEBUG_SELECTION(format, args...)                               \
    {                                                                   \
        if(__builtin_expect(debug_print >= 2, 0))                       \
        {                                                               \
            _DEBUG_MSG(2, "Possible targets of " format ":\n", ##args); \
            fd->print_selection_array();                                \
        }                                                               \
    }
#else
#define _DEBUG_MSG(v, format, args...)
#define _DEBUG_SELECTION(format, args...)
#endif

#endif
