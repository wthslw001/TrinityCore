/*
 * Copyright (C) 2008-2019 TrinityCore <https://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SPELLQUEUE_H
#define _SPELLQUEUE_H

#include "SharedDefines.h"
#include "Util.h"
#include "Spell.h"

class Unit;
enum TriggerCastFlags : uint32;

struct SpellQueueData
{
    uint32 SpellID = 0;
    uint32 GlyphIndex = 0;
    uint32 TriggerFlags = 0;
    int32 Delay = 0;
    uint32 PassedTime = 0;
    uint8 CastCount = 0;
    SpellCastTargets CastTargets;
};

/*
--- Delay Documentation ---
- Self-target spells always will get executed immediately.
- Explicit target spells have a 400ms initial delay.
- Queued spells that are queued before another spell cast been launched will be executed right
  after the previous spell has launched.
*/
static constexpr uint32 const SpellCastDelay = 400;

class SpellQueue
{
    public:
        SpellQueue(Unit* caster);
        ~SpellQueue();

        void AddSpellToQueue(uint32 spellId, uint32 glyphIndex, uint8 castCount, TriggerCastFlags castFlags, SpellCastTargets targets);
        void Update(uint32 diff);
        void ExecuteNextSpell();
        void CancelSpellInQueue();

    private:
        Unit* m_caster;
        SpellQueueData m_queuedSpell;
};


#endif _SPELLQUEUE_H
