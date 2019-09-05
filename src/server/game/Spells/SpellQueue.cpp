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

#include "SpellQueue.h"
#include "SpellMgr.h"
#include "Unit.h"

SpellQueue::SpellQueue(Unit* caster) : m_caster(caster)
{
}

SpellQueue::~SpellQueue()
{
}

void SpellQueue::AddSpellToQueue(uint32 spellId, uint32 glyphIndex, uint8 castCount, TriggerCastFlags castFlags, SpellCastTargets targets)
{
    // DDos protection
    if (m_queuedSpell.SpellID)
        return;

    m_queuedSpell.SpellID = spellId;
    m_queuedSpell.GlyphIndex = glyphIndex;
    m_queuedSpell.CastCount = castCount;
    m_queuedSpell.TriggerFlags = castFlags;
    m_queuedSpell.CastTargets = targets;

    if ((targets.GetObjectTargetGUID() && targets.GetObjectTargetGUID() != m_caster->GetGUID()) || targets.HasDst())
        m_queuedSpell.Delay = SpellCastDelay;
    else
        m_queuedSpell.Delay = 0; // Spell will be executed on next update tick.
}

void SpellQueue::Update(uint32 diff)
{
    if (!m_queuedSpell.SpellID)
        return;

    m_queuedSpell.Delay -= diff;
    m_queuedSpell.PassedTime += diff;

    // Spell in queue is ready, execute
    if (m_queuedSpell.Delay <= 0)
        ExecuteNextSpell();
}

void SpellQueue::ExecuteNextSpell()
{
    if (!m_queuedSpell.SpellID)
        return;

    SpellQueueData data = m_queuedSpell;
    m_queuedSpell = SpellQueueData();

    SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(data.SpellID);
    Spell* spell = new Spell(m_caster, spellInfo, TriggerCastFlags(data.TriggerFlags), ObjectGuid::Empty, false, data.PassedTime);
    spell->m_cast_count = data.CastCount;
    spell->m_glyphIndex = data.GlyphIndex;
    spell->prepare(&data.CastTargets);
}


void SpellQueue::CancelSpellInQueue()
{
    m_queuedSpell = SpellQueueData();
}
