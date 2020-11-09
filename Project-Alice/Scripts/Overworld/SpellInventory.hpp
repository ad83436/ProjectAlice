#ifndef SPELL_INVENTORY_HPP
#define SPELL_INVENTORY_HPP

#include <Engine/Battle/Spells/SpellList.hpp>

class SpellInventory {
	NON_CONSTRUCTABLE(SpellInventory);
public:
	static void InitData(const string& _filePath);

	static void AddSpell(const string& _spellName);
	static void RemoveSpell(const string& _spellName);

	static void EquipAtkSpell(const string& _spellName, int _index);
	static void EquipDefSpell(const string& _spellName);
	static void UnequipAtkSpell(int _index);
	static void UnequipDefSpell();

	static Spell GetSpell(const string& _spellName);
	static int GetSpellCount(SpellType _type);

	static string GetEquippedAtkSpell(int _index);
	static string GetEquippedDefSpell();
};

#endif