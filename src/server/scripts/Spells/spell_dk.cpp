/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

/*
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include <random>

enum DeathKnightSpells
{
    DK_SPELL_RUNIC_POWER_ENERGIZE               = 49088,
    DK_SPELL_ANTI_MAGIC_SHELL_TALENT            = 51052,
    DK_SPELL_CORPSE_EXPLOSION_TRIGGERED         = 43999,
    DK_SPELL_CORPSE_EXPLOSION_VISUAL            = 51270,
    DK_SPELL_GHOUL_EXPLODE                      = 47496,
    DK_SPELL_SCOURGE_STRIKE_TRIGGERED           = 70890,
    DK_SPELL_BLOOD_BOIL_TRIGGERED               = 65658,
    SPELL_DK_ITEM_T8_MELEE_4P_BONUS             = 64736,
    DK_SPELL_MASTERY_DREADBLADE                 = 77515,
    DK_SPELL_BLOOD_PLAGUE                       = 55078,
    DK_SPELL_FROST_FEVER                        = 55095,
    DK_SPELL_MASTER_OF_GHOULS                   = 52143,
    DK_SPELL_GHOUL_AS_GUARDIAN                  = 46585,
    DK_SPELL_GHOUL_AS_PET                       = 52150,
    DK_SPELL_ROILING_BLOOD                      = 108170,
    DK_SPELL_PESTILENCE                         = 50842,
    DK_SPELL_CHILBLAINS                         = 50041,
    DK_SPELL_CHAINS_OF_ICE_ROOT                 = 53534,
    DK_SPELL_PLAGUE_LEECH                       = 123693,
    DK_SPELL_PERDITION                          = 123981,
    DK_SPELL_SHROUD_OF_PURGATORY                = 116888,
    DK_SPELL_PURGATORY_INSTAKILL                = 123982,
    DK_SPELL_BLOOD_RITES                        = 50034,
    DK_SPELL_DEATH_SIPHON_HEAL                  = 116783,
    DK_SPELL_BLOOD_CHARGE                       = 114851,
    DK_SPELL_PILLAR_OF_FROST                    = 51271,
    DK_SPELL_SOUL_REAPER_HASTE                  = 114868,
    DK_SPELL_SOUL_REAPER_DAMAGE                 = 114867,
    DK_SPELL_REMORSELESS_WINTER_STUN            = 115001,
    DK_SPELL_REMORSELESS_WINTER                 = 115000,
    DK_SPELL_CONVERSION							= 119975,
    DK_SPELL_WEAKENED_BLOWS                     = 115798,
    DK_SPELL_SCARLET_FEVER                      = 81132,
    DK_SPELL_SCENT_OF_BLOOD_AURA                = 50421,
    DK_SPELL_CHAINS_OF_ICE                      = 45524,
    DK_SPELL_EBON_PLAGUEBRINGER                 = 51160,
    DK_SPELL_REAPING                            = 56835,
    DK_SPELL_DESECRATED_GROUND                  = 118009,
    DK_SPELL_DESECRATED_GROUND_IMMUNE           = 115018,
    DK_SPELL_ASPHYXIATE                         = 108194,
    DK_SPELL_DARK_INFUSION_STACKS               = 91342,
    DK_SPELL_DARK_INFUSION_AURA                 = 93426,
    DK_NPC_WILD_MUSHROOM                        = 59172,
};

// Might of Ursoc - 113072
class spell_dk_might_of_ursoc : public SpellScriptLoader
{
    public:
        spell_dk_might_of_ursoc() : SpellScriptLoader("spell_dk_might_of_ursoc") { }

        class spell_dk_might_of_ursoc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_might_of_ursoc_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                    if (caster->GetHealthPct() < 15.0f)
                        caster->SetHealth(caster->CountPctFromMaxHealth(15));
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_might_of_ursoc_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH_PERCENT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_might_of_ursoc_AuraScript();
        }
};

// Wild Mushroom : Plague - 113517
class spell_dk_wild_mushroom_plague : public SpellScriptLoader
{
    public:
        spell_dk_wild_mushroom_plague() : SpellScriptLoader("spell_dk_wild_mushroom_plague") { }

        class spell_dk_wild_mushroom_plague_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_wild_mushroom_plague_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (!GetCaster())
                    return;

                std::list<Creature*> tempList;
                std::list<Creature*> mushroomlist;
                std::list<Unit*> tempUnitList;
                std::list<Unit*> targetList;

                if (Player* _player = GetCaster()->ToPlayer())
                {
                    _player->GetCreatureListWithEntryInGrid(tempList, DK_NPC_WILD_MUSHROOM, 500.0f);

                    for (std::list<Creature*>::const_iterator itr = tempList.begin(); itr != tempList.end(); ++itr)
                        mushroomlist.push_back(*itr);

                    // Remove other players mushrooms
                    for (std::list<Creature*>::iterator i = tempList.begin(); i != tempList.end(); ++i)
                    {
                        Unit* owner = (*i)->GetOwner();
                        if (owner && owner == _player && (*i)->isSummon())
                            continue;

                        mushroomlist.remove((*i));
                    }

                    if (!mushroomlist.empty())
                    {
                        for (std::list<Creature*>::const_iterator itr = mushroomlist.begin(); itr != mushroomlist.end(); ++itr)
                        {
                            (*itr)->GetAttackableUnitListInRange(tempUnitList, 10.0f);

                            for (std::list<Unit*>::const_iterator itr2 = tempUnitList.begin(); itr2 != tempUnitList.end(); ++itr2)
                            {
                                if ((*itr2)->GetGUID() == _player->GetGUID())
                                    continue;

                                if ((*itr2)->GetGUID() == (*itr)->GetGUID())
                                    continue;

                                if (!_player->IsValidAttackTarget(*itr2))
                                    continue;

                                targetList.push_back(*itr2);
                            }

                            for (std::list<Unit*>::const_iterator itr2 = targetList.begin(); itr2 != targetList.end(); ++itr2)
                            {
                                _player->CastSpell(*itr2, DK_SPELL_BLOOD_PLAGUE, true);
                                _player->CastSpell(*itr2, DK_SPELL_FROST_FEVER, true);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_wild_mushroom_plague_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_wild_mushroom_plague_AuraScript();
        }
};

// Dark transformation - transform pet spell - 63560
class spell_dk_dark_transformation_form : public SpellScriptLoader
{
    public:
        spell_dk_dark_transformation_form() : SpellScriptLoader("spell_dk_dark_transformation_form") { }

        class spell_dk_dark_transformation_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_dark_transformation_form_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* pet = GetHitUnit())
                        if (pet->HasAura(DK_SPELL_DARK_INFUSION_STACKS))
                        {
                            pet->RemoveAura(DK_SPELL_DARK_INFUSION_STACKS);
                            _player->RemoveAura(DK_SPELL_DARK_INFUSION_STACKS);
                        }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_dark_transformation_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_dark_transformation_form_SpellScript();
        }
};

// Called by Strangulate - 47476
// Asphyxiate - 108194
class spell_dk_asphyxiate : public SpellScriptLoader
{
    public:
        spell_dk_asphyxiate() : SpellScriptLoader("spell_dk_asphyxiate") { }

        class spell_dk_asphyxiate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_asphyxiate_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasSpell(DK_SPELL_ASPHYXIATE))
                        {
                            if (target->HasAura(GetSpellInfo()->Id, _player->GetGUID()))
                            {
                                target->RemoveAura(GetSpellInfo()->Id, _player->GetGUID());
                                _player->CastSpell(target, DK_SPELL_ASPHYXIATE, true);
                            }
                        }
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_asphyxiate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_asphyxiate_SpellScript();
        }
};

// Desecrated ground - 118009
class spell_dk_desecrated_ground : public SpellScriptLoader
{
    public:
        spell_dk_desecrated_ground() : SpellScriptLoader("spell_dk_desecrated_ground") { }

        class spell_dk_desecrated_ground_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_desecrated_ground_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if(Unit* caster = GetCaster())
                    if (DynamicObject* dynObj = caster->GetDynObject(DK_SPELL_DESECRATED_GROUND))
                        if (caster->GetDistance(dynObj) <= 8.0f)
                            caster->CastSpell(caster, DK_SPELL_DESECRATED_GROUND_IMMUNE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_desecrated_ground_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_desecrated_ground_AuraScript();
        }
};

// Necrotic Strike - 73975
class spell_dk_necrotic_strike : public SpellScriptLoader
{
    public:
        spell_dk_necrotic_strike() : SpellScriptLoader("spell_dk_necrotic_strike") { }

        class spell_dk_necrotic_strike_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_necrotic_strike_AuraScript);

            void CalculateAmount(AuraEffect const* aurEff, float & amount, bool & /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    amount = CalculatePct(caster->GetTotalAttackPowerValue(BASE_ATTACK), 225);

                    if (Unit* target = aurEff->GetBase()->GetUnitOwner())
                    {
                        amount = caster->CalcPvPPower(target, amount);

                        if (target->GetTypeId() == TYPEID_PLAYER)
                            caster->ApplyResilience(target, &amount, false);
                    }
                    amount += aurEff->GetOldBaseAmount();
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_necrotic_strike_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_necrotic_strike_AuraScript();
        }
};

// Festering Strike - 85948
class spell_dk_festering_strike : public SpellScriptLoader
{
    public:
        spell_dk_festering_strike() : SpellScriptLoader("spell_dk_festering_strike") { }

        class spell_dk_festering_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_festering_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* BP = target->GetAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID()))
                        {
                            uint32 dur = BP->GetDuration() + 6000;
                            BP->SetDuration(dur);
                        }
                        if (Aura* FF = target->GetAura(DK_SPELL_FROST_FEVER, _player->GetGUID()))
                        {
                            uint32 dur = FF->GetDuration() + 6000;
                            FF->SetDuration(dur);
                        }
                        if (Aura* PV = target->GetAura(81326, _player->GetGUID()))
                        {
                            uint32 dur = PV->GetDuration() + 6000;
                            PV->SetDuration(dur);
                        }
                        if (Aura* COI = target->GetAura(DK_SPELL_CHAINS_OF_ICE, _player->GetGUID()))
                        {
                            uint32 dur = COI->GetDuration() + 6000;
                            COI->SetDuration(dur);
                        }
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_festering_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_festering_strike_SpellScript();
        }
};

// Death Strike heal - 45470
class spell_dk_death_strike_heal : public SpellScriptLoader
{
    public:
        spell_dk_death_strike_heal() : SpellScriptLoader("spell_dk_death_strike_heal") { }

        class spell_dk_death_strike_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_strike_heal_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* scentOfBlood = _player->GetAura(DK_SPELL_SCENT_OF_BLOOD_AURA))
                        {
                            uint8 chg = scentOfBlood->GetStackAmount();
                            int32 hl = GetHitHeal() * 0.2 * chg;
                            SetHitHeal(GetHitHeal() + hl);
                            
                            GetCaster()->RemoveAura(DK_SPELL_SCENT_OF_BLOOD_AURA);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_death_strike_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_strike_heal_SpellScript();
        }
};

// Howling Blast - 49184
class spell_dk_howling_blast : public SpellScriptLoader
{
    public:
        spell_dk_howling_blast() : SpellScriptLoader("spell_dk_howling_blast") { }

        class spell_dk_howling_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_howling_blast_SpellScript);

            uint64 tar;

            void HandleBeforeCast()
            {
                Unit* target = GetExplTargetUnit();
                Unit* caster = GetCaster();

                if (!caster || !target)
                    return;

                tar = target->GetGUID();
            }

            void HandleOnHit()
            {
                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();

                if (!caster || !target || !tar)
                    return;

                if (target->GetGUID() != tar)
                    SetHitDamage(GetHitDamage()/2);

                caster->CastSpell(target, DK_SPELL_FROST_FEVER, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dk_howling_blast_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dk_howling_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_howling_blast_SpellScript();
        }
};

// Remorseless Winter - 115000
class spell_dk_remorseless_winter : public SpellScriptLoader
{
    public:
        spell_dk_remorseless_winter() : SpellScriptLoader("spell_dk_remorseless_winter") { }

        class spell_dk_remorseless_winter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_remorseless_winter_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (Aura* remorselessWinter = target->GetAura(DK_SPELL_REMORSELESS_WINTER))
                            if (remorselessWinter->GetStackAmount() == 5 && !target->HasAura(DK_SPELL_REMORSELESS_WINTER_STUN))
                                _player->CastSpell(target, DK_SPELL_REMORSELESS_WINTER_STUN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_remorseless_winter_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_remorseless_winter_SpellScript();
        }
};

// Soul Reaper - 130736 (unholy) or 130735 (frost) or 114866 (blood)
class spell_dk_soul_reaper : public SpellScriptLoader
{
    public:
        spell_dk_soul_reaper() : SpellScriptLoader("spell_dk_soul_reaper") { }

        class spell_dk_soul_reaper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_AuraScript);

            void OnTick(AuraEffect const* aurEff)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        if (target->GetHealthPct() < 35.0f || (caster->HasAura(138347) && target->GetHealthPct() < 45.0f))
                            caster->CastSpell(target, DK_SPELL_SOUL_REAPER_DAMAGE, true);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_DEATH && GetDuration() > 1000)
                        caster->CastSpell(caster, DK_SPELL_SOUL_REAPER_HASTE, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_soul_reaper_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectApplyFn(spell_dk_soul_reaper_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_soul_reaper_AuraScript();
        }
};

// Pillar of Frost - 51271
class spell_dk_pillar_of_frost : public SpellScriptLoader
{
    public:
        spell_dk_pillar_of_frost() : SpellScriptLoader("spell_dk_pillar_of_frost") { }

        class spell_dk_pillar_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_pillar_of_frost_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_INCAPACITATE, false);
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_INCAPACITATE, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_pillar_of_frost_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_pillar_of_frost_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_pillar_of_frost_AuraScript();
        }
};

// Called by Death Coil - 47541, Rune Strike - 56815 and Frost Strike - 49143
// Blood Charges - 114851 for Blood Tap - 45529
class spell_dk_blood_charges : public SpellScriptLoader
{
    public:
        spell_dk_blood_charges() : SpellScriptLoader("spell_dk_blood_charges") { }

        class spell_dk_blood_charges_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_charges_SpellScript);

            void HandleOnHit()
            {
                if (Unit* _player = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasSpell(45529))
                        {
                            if (_player->HasAura(58677))
                                if (target->IsPet() && target->GetOwnerGUID() == _player->GetGUID())
                                    return;

                            _player->CastSpell(_player, DK_SPELL_BLOOD_CHARGE, true);
                            _player->CastSpell(_player, DK_SPELL_BLOOD_CHARGE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_blood_charges_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_charges_SpellScript();
        }
};

// Blood Tap - 45529
class spell_dk_blood_tap : public SpellScriptLoader
{
    public:
        spell_dk_blood_tap() : SpellScriptLoader("spell_dk_blood_tap") { }

        class spell_dk_blood_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_tap_SpellScript);

            std::vector<uint8> CDrunesList;

            SpellCastResult CheckBloodCharges()
            {
                if (GetCaster())
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Aura* bloodCharges = _player->GetAura(DK_SPELL_BLOOD_CHARGE))
                    {
                        if (bloodCharges->GetStackAmount() < 5)
                        {
                            Spell::SendCastResult(_player, GetSpellInfo(), 1, SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_INSUFFICIENT_BLOOD_CHARGES);
                            return SPELL_FAILED_DONT_REPORT;
                        }
                    }
                    else
                    {
                        Spell::SendCastResult(_player, GetSpellInfo(), 1, SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_INSUFFICIENT_BLOOD_CHARGES);
                        return SPELL_FAILED_DONT_REPORT;
                    }


                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                        if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN)
                            CDrunesList.push_back(i);

                    if (CDrunesList.empty())
                    {
                        Spell::SendCastResult(_player, GetSpellInfo(), 1, SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_NO_DEPLETED_RUNES);
                        return SPELL_FAILED_DONT_REPORT;
                    }
                }

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (Aura* bloodCharges = _player->GetAura(DK_SPELL_BLOOD_CHARGE))
                        {
                            int32 newAmount = bloodCharges->GetStackAmount();

                            if ((newAmount - 5) <= 0)
                                _player->RemoveAura(DK_SPELL_BLOOD_CHARGE);
                            else
                                bloodCharges->SetStackAmount(newAmount - 5);
                        }


                        std::shuffle(CDrunesList.begin(), CDrunesList.end(), std::default_random_engine{});
                        for (auto itr : CDrunesList)
                        {
                            _player->SetRuneCooldown(itr, 0);
                            _player->SetConvertIn(itr, RUNE_DEATH);
                            _player->ConvertRune(itr, RUNE_DEATH);
                            _player->AddRunePower(itr);
                            break;
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_blood_tap_SpellScript::CheckBloodCharges);
                OnHit += SpellHitFn(spell_dk_blood_tap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_tap_SpellScript();
        }
};

// Death Siphon - 108196
class spell_dk_death_siphon : public SpellScriptLoader
{
    public:
        spell_dk_death_siphon() : SpellScriptLoader("spell_dk_death_siphon") { }

        class spell_dk_death_siphon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_siphon_SpellScript);

            void HandleAfterHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        float bp = CalculatePct(float(GetHitDamage()), GetSpellInfo()->Effects[EFFECT_1]->CalcValue(_player));
                        _player->CastCustomSpell(_player, DK_SPELL_DEATH_SIPHON_HEAL, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_dk_death_siphon_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_siphon_SpellScript();
        }
};

// Purgatory - 116888
class spell_dk_purgatory : public SpellScriptLoader
{
    public:
        spell_dk_purgatory() : SpellScriptLoader("spell_dk_purgatory") { }

        class spell_dk_purgatory_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                        _player->CastSpell(_player, DK_SPELL_PURGATORY_INSTAKILL, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_purgatory_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_AuraScript();
        }
};

// Purgatory - 114556
class spell_dk_purgatory_absorb : public SpellScriptLoader
{
    public:
        spell_dk_purgatory_absorb() : SpellScriptLoader("spell_dk_purgatory_absorb") { }

        class spell_dk_purgatory_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_absorb_AuraScript);

            void CalculateAmount(AuraEffect const* /*AuraEffect**/, float& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* /*AuraEffect**/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                if (target->HasAura(146193) && !target->HasAura(148010))
                    return;

                // No damage received under Shroud of Purgatory
                if (target->ToPlayer()->HasAura(DK_SPELL_SHROUD_OF_PURGATORY))
                {
                    absorbAmount = dmgInfo.GetDamage();
                    return;
                }

                if (target->ToPlayer()->HasAura(DK_SPELL_PERDITION))
                    return;

                float bp = dmgInfo.GetDamage();

                target->CastCustomSpell(target, DK_SPELL_SHROUD_OF_PURGATORY, &bp, NULL, NULL, true);
                target->CastSpell(target, DK_SPELL_PERDITION, true);
                target->SetHealth(1);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_purgatory_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_purgatory_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_absorb_AuraScript();
        }
};

// Plague Leech - 123693
class spell_dk_plague_leech : public SpellScriptLoader
{
    public:
        spell_dk_plague_leech() : SpellScriptLoader("spell_dk_plague_leech") { }

        class spell_dk_plague_leech_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plague_leech_SpellScript);

            RuneType GetRuneBan(Player* plr)
            {
                switch (plr->GetSpecializationId(plr->GetActiveSpec()))
                {
                    case SPEC_DK_BLOOD:
                    case SPEC_DK_FROST:  return RUNE_BLOOD;
                    case SPEC_DK_UNHOLY: return RUNE_UNHOLY;
                    default:
                        return NUM_RUNE_TYPES;
                }
            }

            SpellCastResult CheckRunes()
            {
                int32 runesUsed = 0;
                
                if (GetCaster())
                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        RuneType runesBan = GetRuneBan(_player);

                        for (uint8 i = 0; i < MAX_RUNES; ++i)
                            if (_player->GetBaseRune(i) != runesBan)
                                if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN)
                                    runesUsed++;

                        if (!runesUsed)
                        {
                            Spell::SendCastResult(_player, GetSpellInfo(), 1, SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_NO_DEPLETED_RUNES);
                            return SPELL_FAILED_DONT_REPORT;
                        }

                        if (Unit* target = GetExplTargetUnit())
                        {
                            if (!target->HasAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID()) || !target->HasAura(DK_SPELL_FROST_FEVER, _player->GetGUID()))
                            {
                                Spell::SendCastResult(_player, GetSpellInfo(), 1, SPELL_FAILED_CUSTOM_ERROR, SPELL_CUSTOM_ERROR_FEVER_PLAGUE_MUST_BE_PRESENT);
                                return SPELL_FAILED_DONT_REPORT;
                            }
                        }
                        return SPELL_CAST_OK;
                    }

                return SPELL_FAILED_DONT_REPORT;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        target->RemoveAura(DK_SPELL_FROST_FEVER, _player->GetGUID());
                        target->RemoveAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID());

                        int32 runesRestor = 0;
                        RuneType runesBan = GetRuneBan(_player);

                        for (int i = 0; i < MAX_RUNES ; i++)
                        {
                            if (_player->GetBaseRune(i) == runesBan)
                                continue;

                            if (_player->GetRuneCooldown(i) == RUNE_BASE_COOLDOWN && runesRestor < 2)
                            {
                                runesRestor++;
                                _player->SetRuneCooldown(i, 0);
                                _player->AddRunePower(i);
                                if (_player->GetCurrentRune(i) != RUNE_DEATH)
                                {
                                    _player->SetConvertIn(i, RUNE_DEATH);
                                    _player->ConvertRune(i, RUNE_DEATH);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_plague_leech_SpellScript::CheckRunes);
                OnHit += SpellHitFn(spell_dk_plague_leech_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plague_leech_SpellScript();
        }
};

// Unholy Blight - 115994
class spell_dk_unholy_blight : public SpellScriptLoader
{
    public:
        spell_dk_unholy_blight() : SpellScriptLoader("spell_dk_unholy_blight") { }

        class spell_dk_unholy_blight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_unholy_blight_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, DK_SPELL_BLOOD_PLAGUE, true);
                        _player->CastSpell(target, DK_SPELL_FROST_FEVER, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_unholy_blight_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_unholy_blight_SpellScript();
        }
};

// Called by Chains of Ice - 45524
// Chilblains - 50041
class spell_dk_chilblains : public SpellScriptLoader
{
    public:
        spell_dk_chilblains() : SpellScriptLoader("spell_dk_chilblains") { }

        class spell_dk_chilblains_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_chilblains_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(DK_SPELL_CHILBLAINS))
                            _player->CastSpell(target, DK_SPELL_CHAINS_OF_ICE_ROOT, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_chilblains_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_chilblains_SpellScript();
        }
};

// Outbreak - 77575
class spell_dk_outbreak : public SpellScriptLoader
{
    public:
        spell_dk_outbreak() : SpellScriptLoader("spell_dk_outbreak") { }

        class spell_dk_outbreak_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_outbreak_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, DK_SPELL_BLOOD_PLAGUE, true);
                        _player->CastSpell(target, DK_SPELL_FROST_FEVER, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_outbreak_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_outbreak_SpellScript();
        }
};

// Raise Dead - 46584
class spell_dk_raise_dead : public SpellScriptLoader
{
    public:
        spell_dk_raise_dead() : SpellScriptLoader("spell_dk_raise_dead") { }

        class spell_dk_raise_dead_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_raise_dead_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(DK_SPELL_MASTER_OF_GHOULS))
                        _player->CastSpell(_player, DK_SPELL_GHOUL_AS_PET, true);
                    else
                        _player->CastSpell(_player, DK_SPELL_GHOUL_AS_GUARDIAN, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_raise_dead_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_raise_dead_SpellScript();
        }
};

// 50462 - Anti-Magic Shell (on raid member)
class spell_dk_anti_magic_shell_raid : public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_raid() : SpellScriptLoader("spell_dk_anti_magic_shell_raid") { }

        class spell_dk_anti_magic_shell_raid_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_raid_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0]->CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, float & amount, bool & /*canBeRecalculated*/)
            {
                // TODO: this should absorb limited amount of damage, but no info on calculation formula
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_raid_AuraScript();
        }
};

// 48707 - Anti-Magic Shell (on self)
class spell_dk_anti_magic_shell_self : public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_self() : SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

        class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);

            uint32 absorbPct, container;
            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0]->CalcValue(GetCaster());
                container = 0;
                return true;
            }

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_RUNIC_POWER_ENERGIZE))
                    return false;
                return true;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                absorbAmount = std::min(CalculatePct(dmgInfo.GetDamage(), absorbPct), uint32(aurEff->GetAmount()));
            }

            void Trigger(AuraEffect* aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
            {
                if (Unit* target = GetTarget()) // AMS generates 2 Runic Power for every percent of maximum health absorbed
                {
                    uint32 RPCap = target->CountPctFromMaxHealth(1) / 2;
                    float bp = (container + absorbAmount) / RPCap;
                    container = (container + absorbAmount) - (bp * RPCap);
                    bp *= 10;

                    if (bp >= 10)
                        target->CastCustomSpell(target, DK_SPELL_RUNIC_POWER_ENERGIZE, &bp, NULL, NULL, true, NULL, aurEff);
                }
            }

            void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Player* player = GetCaster()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE && player->HasAura(146648)) // Glyph of Regenerative Magic
                    {
                        double cooldown = player->GetSpellCooldownDelay(GetSpellInfo()->Id);
                        int32 percent = int32(float(aurEff->GetAmount() * 100.0f) / player->GetMaxHealth());
                        percent = percent < 50 ? percent : 50;
                        cooldown = (cooldown * percent) / 100;
                        player->ModifySpellCooldown(GetSpellInfo()->Id, -(cooldown * IN_MILLISECONDS));
                    }
                }
            }

            void Register()
            {
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_anti_magic_shell_self_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_self_AuraScript();
        }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone : public SpellScriptLoader
{
    public:
        spell_dk_anti_magic_zone() : SpellScriptLoader("spell_dk_anti_magic_zone") { }

        class spell_dk_anti_magic_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_zone_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0]->CalcValue(GetCaster());
                return true;
            }

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_ANTI_MAGIC_SHELL_TALENT))
                    return false;
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, float & amount, bool & /*canBeRecalculated*/)
            {
                SpellInfo const* talentSpell = sSpellMgr->GetSpellInfo(DK_SPELL_ANTI_MAGIC_SHELL_TALENT);
                amount = 136800;
                if (Player* player = GetCaster()->ToPlayer())
                     amount += int32(player->GetStat(STAT_STRENGTH) * 4);
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_zone_AuraScript();
        }
};

// 47496 - Explode, Ghoul spell for Corpse Explosion
class spell_dk_ghoul_explode : public SpellScriptLoader
{
    public:
        spell_dk_ghoul_explode() : SpellScriptLoader("spell_dk_ghoul_explode") { }

        class spell_dk_ghoul_explode_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_ghoul_explode_SpellScript);

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_CORPSE_EXPLOSION_TRIGGERED))
                    return false;
                return true;
            }

            void Suicide(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    // Corpse Explosion (Suicide)
                    unitTarget->CastSpell(unitTarget, DK_SPELL_CORPSE_EXPLOSION_TRIGGERED, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_ghoul_explode_SpellScript::Suicide, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_ghoul_explode_SpellScript();
        }
};

// Death Gate - 53822
class spell_dk_death_gate_teleport : public SpellScriptLoader
{
    public:
        spell_dk_death_gate_teleport() : SpellScriptLoader("spell_dk_death_gate_teleport") {}

        class spell_dk_death_gate_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_teleport_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }
                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if(_player->GetZoneId() == 139)
                        _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, 0.0f);
                    else
                        _player->TeleportTo(0, 2355.23f, -5666.4433f, 426.028f, _player->GetOrientation());
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_teleport_SpellScript::CheckClass);
                AfterCast += SpellCastFn(spell_dk_death_gate_teleport_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_teleport_SpellScript();
        }
};

// Death Gate - 52751
class spell_dk_death_gate : public SpellScriptLoader
{
    public:
        spell_dk_death_gate() : SpellScriptLoader("spell_dk_death_gate") {}

        class spell_dk_death_gate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, GetEffectValue(), false);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_SpellScript();
        }
};

class spell_dk_death_pact : public SpellScriptLoader
{
    public:
        spell_dk_death_pact() : SpellScriptLoader("spell_dk_death_pact") { }

        class spell_dk_death_pact_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_pact_SpellScript);

            SpellCastResult CheckCast()
            {
                // Check if we have valid targets, otherwise skip spell casting here
                if (Player* player = GetCaster()->ToPlayer())
                    for (Unit::ControlList::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                        if (Creature* undeadPet = (*itr)->ToCreature())
                            if (undeadPet->IsAlive() &&
                                undeadPet->GetOwnerGUID() == player->GetGUID() &&
                                undeadPet->GetCreatureType() == CREATURE_TYPE_UNDEAD &&
                                undeadPet->IsWithinDist(player, 100.0f, false))
                                return SPELL_CAST_OK;

                return SPELL_FAILED_NO_PET;
            }

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                Unit* unit_to_add = NULL;
                for (std::list<WorldObject*>::iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                {
                    if (Unit* unit = (*itr)->ToUnit())
                    if (unit->GetOwnerGUID() == GetCaster()->GetGUID() && unit->GetCreatureType() == CREATURE_TYPE_UNDEAD)
                    {
                        unit_to_add = unit;
                        break;
                    }
                }

                unitList.clear();
                if (unit_to_add)
                    unitList.push_back(unit_to_add);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_pact_SpellScript::CheckCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_death_pact_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_pact_SpellScript();
        }
};

// Scourge Strike - 55090
class spell_dk_scourge_strike : public SpellScriptLoader
{
    public:
        spell_dk_scourge_strike() : SpellScriptLoader("spell_dk_scourge_strike") { }

        class spell_dk_scourge_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_scourge_strike_SpellScript);
            float multiplier;

            bool Load()
            {
                multiplier = 1.0f;
                return true;
            }

            bool Validate(SpellInfo const* /*SpellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_SCOURGE_STRIKE_TRIGGERED))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    multiplier = (GetEffectValue() * unitTarget->GetDiseasesByCaster(caster->GetGUID()) / 100.f);
                    // Death Knight T8 Melee 4P Bonus
                    if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_DK_ITEM_T8_MELEE_4P_BONUS, EFFECT_0))
                        AddPct(multiplier, aurEff->GetAmount());
                }
            }

            void HandleAfterHit()
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                {
                    float bp = GetHitDamage() * multiplier;

                    if (AuraEffect* aurEff = caster->GetAuraEffect(DK_SPELL_MASTERY_DREADBLADE, EFFECT_0)) // Mastery: Dreadblade
                        AddPct(bp, aurEff->GetAmount());

                    caster->CastCustomSpell(unitTarget, DK_SPELL_SCOURGE_STRIKE_TRIGGERED, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_scourge_strike_SpellScript::HandleDummy, EFFECT_2, SPELL_EFFECT_DUMMY);
                AfterHit += SpellHitFn(spell_dk_scourge_strike_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_scourge_strike_SpellScript();
        }
};

// Blood Boil - 48721
class spell_dk_blood_boil : public SpellScriptLoader
{
    public:
        spell_dk_blood_boil() : SpellScriptLoader("spell_dk_blood_boil") { }

        class spell_dk_blood_boil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_boil_SpellScript);

            bool Load()
            {
                RoilingBloodTargetGuids.clear();
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 damge = GetHitDamage();

                        if (_player->HasAura(DK_SPELL_SCARLET_FEVER))
                        {
                            if (Aura* aura = target->GetAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID()))
                                aura->SetDuration(aura->GetMaxDuration());

                            if (Aura* aura = target->GetAura(DK_SPELL_FROST_FEVER, _player->GetGUID()))
                                aura->SetDuration(aura->GetMaxDuration());
                        }

                        // Deals 50% additional damage to targets infected with Blood Plague or Frost Fever
                        if (target->HasAura(DK_SPELL_FROST_FEVER, _player->GetGUID()))
                            SetHitDamage(int32(damge * 1.5f));
                        else if (target->HasAura(DK_SPELL_BLOOD_PLAGUE, _player->GetGUID()))
                            SetHitDamage(int32(damge * 1.5f));
                        else if (_player->HasAura(146650, _player->GetGUID()))
                            SetHitDamage(int32(damge * 1.5f));
                    }
                }
            }

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                Unit* caster = GetCaster();
                if (!caster || !caster->HasAura(108170))
                    return;

                if (!unitList.empty())
                    caster->CastSpell(caster, DK_SPELL_BLOOD_BOIL_TRIGGERED, true);

                for (std::list<WorldObject*>::iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                    if (Unit* unit = (*itr)->ToUnit())
                        if (unit->HasAura(DK_SPELL_FROST_FEVER, caster->GetGUID()) || unit->HasAura(DK_SPELL_BLOOD_PLAGUE, caster->GetGUID()))
                            RoilingBloodTargetGuids.push_back(unit->GetGUID());
            }

            void HandleAfterCast()
            {
                // Roiling Blood
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (RoilingBloodTargetGuids.empty())
                        return;

                    std::shuffle(RoilingBloodTargetGuids.begin(), RoilingBloodTargetGuids.end(), std::default_random_engine{});
                    for (std::vector<uint64>::const_iterator Itr = RoilingBloodTargetGuids.begin(); Itr != RoilingBloodTargetGuids.end(); ++Itr)
                    {
                        if (Unit* target = _player->GetUnit(*_player, *Itr))
                        {
                            if (target->IsAlive())
                            {
                                _player->CastSpell(target, 116617, true);
                                break;
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dk_blood_boil_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_dk_blood_boil_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dk_blood_boil_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        private:
            std::vector<uint64>RoilingBloodTargetGuids;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_boil_SpellScript();
        }
};

// Death Grip - 49560
class spell_dk_death_grip : public SpellScriptLoader
{
    public:
        spell_dk_death_grip() : SpellScriptLoader("spell_dk_death_grip") { }

        class spell_dk_death_grip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_grip_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 damage = GetEffectValue();
                Position const* pos = GetExplTargetDest();
                if (Unit* target = GetHitUnit())
                {
                    if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS)) // Deterrence
                        target->CastSpell(pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), damage, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_grip_SpellScript();
        }
};

// Death Grip - 49576
class spell_dk_death_grip_dummy : public SpellScriptLoader
{
    public:
        spell_dk_death_grip_dummy() : SpellScriptLoader("spell_dk_death_grip_dummy") { }

        class spell_dk_death_grip_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_grip_dummy_SpellScript);

            SpellCastResult CheckCast()
            {
                if(Unit* caster = GetCaster())
                    if (caster->GetUnitMovementFlags() & MOVEMENTFLAG_FALLING)
                        return SPELL_FAILED_ERROR;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                if (Unit* target = GetHitUnit())
                {
                    if(target->IsWithinMeleeRange(caster))
                        caster->CastSpell(target, 51399, false);
                    else
                        caster->CastSpell(target, 49560, false);
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_grip_dummy_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_grip_dummy_SpellScript();
        }
};

// Gorefiend's Grasp - 108199
class spell_dk_gorefiends_grasp : public SpellScriptLoader
{
    public:
        spell_dk_gorefiends_grasp() : SpellScriptLoader("spell_dk_gorefiends_grasp") { }

        class spell_dk_gorefiends_grasp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_gorefiends_grasp_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetExplTargetUnit();
                Unit* target = GetHitUnit();
                if(caster && target && caster != target)
                {
                    Position const* pos = GetExplTargetDest();
                    caster->CastSpell(target, 114869, true);
                    target->CastSpell(pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), 146599, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_gorefiends_grasp_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }

        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_gorefiends_grasp_SpellScript();
        }
};

// Presence - 48266,48265,48263
class spell_dk_presence : public SpellScriptLoader
{
    public:
        spell_dk_presence() : SpellScriptLoader("spell_dk_presence") { }

        class spell_dk_presence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_presence_SpellScript);

            void HandleOnHit()
            {
                if(Unit* caster = GetCaster())
                {
                    if (Player* _player = GetCaster()->ToPlayer())
                    {
                        int32 power = _player->GetPower(POWER_RUNIC_POWER);
                        if(caster->HasAura(58647)) // Glyph of Shifting Presences
                            power -= int32(power * 0.7f);

                        _player->ModifyPower(POWER_RUNIC_POWER, -(power), true);
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_presence_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_presence_SpellScript();
        }
};

// Unholy Frenzy - 49016
class spell_dk_unholy_frenzy : public SpellScriptLoader
{
    public:
        spell_dk_unholy_frenzy() : SpellScriptLoader("spell_dk_unholy_frenzy") { }

        class spell_dk_unholy_frenzy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_unholy_frenzy_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, float & amount, bool & /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if(caster->HasAura(58616))
                        amount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_unholy_frenzy_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_unholy_frenzy_AuraScript();
        }
};

// Corpse Explosion - 127344
class spell_dk_corpse_explosion : public SpellScriptLoader
{
    public:
        spell_dk_corpse_explosion() : SpellScriptLoader("spell_dk_corpse_explosion") {}

        class spell_dk_corpse_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_corpse_explosion_SpellScript);

            SpellCastResult CheckClass()
            {
                if (Unit* target = GetExplTargetUnit())
                {
                    if(!target->isDead() || target->GetDisplayId() != target->GetNativeDisplayId())
                        return SPELL_FAILED_TARGET_NOT_DEAD;
                }
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_corpse_explosion_SpellScript::CheckClass);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_corpse_explosion_SpellScript();
        }
};

// Soul Reaper - 114868
class spell_dk_soul_reaper_speed : public SpellScriptLoader
{
    public:
        spell_dk_soul_reaper_speed() : SpellScriptLoader("spell_dk_soul_reaper_speed") { }

        class spell_dk_soul_reaper_speed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_speed_AuraScript);

            void CalculateAmount(AuraEffect const* /*AuraEffect**/, float& amount, bool& /*canBeRecalculated*/)
            {
                if(Unit* caster = GetCaster())
                {
                    if(!caster->HasAura(146645))
                        amount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_soul_reaper_speed_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_SPEED_ALWAYS);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_soul_reaper_speed_AuraScript();
        }
};

// Called by - 55050, 56815, 47541, 114866, 48721
// Item - Death Knight T16 Blood 2P Bonus - 144934
class spell_dk_bone_shield : public SpellScriptLoader
{
    public:
        spell_dk_bone_shield() : SpellScriptLoader("spell_dk_bone_shield") { }

        class spell_dk_bone_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_bone_shield_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Aura* setCast = caster->GetAura(144934))
                        setCast->SetScriptData(0, 1);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_bone_shield_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_bone_shield_SpellScript();
        }
};

// Item - Death Knight T16 Blood 2P Bonus - 144934
class spell_dk_blood_2p_bonus : public SpellScriptLoader
{
    public:
        spell_dk_blood_2p_bonus() : SpellScriptLoader("spell_dk_blood_2p_bonus") { }

        class spell_dk_blood_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_blood_2p_bonus_AuraScript);

            uint32 castCount;

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                castCount = 0;
            }

            void SetData(uint32 /*type*/, uint32 count)
            {
                castCount += count;
                if (castCount >= 10)
                {
                    castCount -= 10;

                    if (Unit* caster = GetCaster())
                        caster->CastSpell(caster, 144948, true);
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_dk_blood_2p_bonus_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_blood_2p_bonus_AuraScript();
        }
};

// Called by - 49028
// Item - Death Knight T16 Blood 4P Bonus - 144950
class spell_dk_dancing_rune_weapon : public SpellScriptLoader
{
    public:
        spell_dk_dancing_rune_weapon() : SpellScriptLoader("spell_dk_dancing_rune_weapon") { }

        class spell_dk_dancing_rune_weapon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_dancing_rune_weapon_SpellScript);

            void HandleOnCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasAura(144950))
                    {
                        for (int i = 0; i < MAX_RUNES ; i++)
                        {
                            if (_player->GetCurrentRune(i) == RUNE_DEATH)
                                continue;

                            if (_player->GetCurrentRune(i) == RUNE_FROST || _player->GetCurrentRune(i) == RUNE_UNHOLY)
                            {
                                _player->SetConvertIn(i, RUNE_DEATH);
                                _player->ConvertRune(i, RUNE_DEATH);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_dk_dancing_rune_weapon_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_dancing_rune_weapon_SpellScript();
        }
};

// Death Shroud - 144901
class spell_dk_death_shroud : public SpellScriptLoader
{
    public:
        spell_dk_death_shroud() : SpellScriptLoader("spell_dk_death_shroud") { }

        class spell_dk_death_shroud_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_shroud_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, float & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    uint32 mastery = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_MASTERY);
                    uint32 haste = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_HASTE_MELEE);

                    if(haste < mastery)
                        amount = 0;
                }
            }

            void CalculateAmount1(AuraEffect const* /*aurEff*/, float & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    uint32 mastery = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_MASTERY);
                    uint32 haste = _player->GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + CR_HASTE_MELEE);

                    if(haste > mastery)
                        amount = 0;
                }
            }

            void CalculateMaxDuration(int32 & duration)
            {
                Unit* caster = GetCaster();
                if(!caster)
                    return;

                if (Player* _player = caster->ToPlayer())
                {
                    if (_player->HasSpell(63560))
                        duration = 15000;
                    else if(_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_DK_FROST)
                        duration = 4000 + (_player->IsTwoHandUsed() ? 4000 : 2000);
                }
            }

            void Register()
            {
                DoCalcMaxDuration += AuraCalcMaxDurationFn(spell_dk_death_shroud_AuraScript::CalculateMaxDuration);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_shroud_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_RATING);
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_shroud_AuraScript::CalculateAmount1, EFFECT_1, SPELL_AURA_MOD_RATING);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_shroud_AuraScript();
        }
};

//Blood Gorged 81277
class spell_dk_blood_gorged : public SpellScriptLoader
{
public:
    spell_dk_blood_gorged() : SpellScriptLoader("spell_dk_blood_gorged") { }

    class spell_dk_blood_gorged_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_blood_gorged_AuraScript);

        void OnStackChange(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if(!target)
                return;

            if (Unit* owner = target->GetOwner())
            {
                if (owner->IsFullHealth())
                    return;

                uint32 chance = GetStackAmount() * 2;
                
                if (roll_chance_i(chance))
                    Remove(AURA_REMOVE_BY_DEFAULT);
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            if (!target)
                return;

            if (AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode())
                if (removeMode != AURA_REMOVE_BY_DEATH)
                {
                    float damage = CalculatePct(target->GetMaxHealth(), GetStackAmount() * GetSpellInfo()->Effects[EFFECT_1]->BasePoints);
                    target->CastCustomSpell(target, 81280, &damage, NULL, NULL, true);
                }

        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(spell_dk_blood_gorged_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectRemoveFn(spell_dk_blood_gorged_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_blood_gorged_AuraScript();
    }
};

void AddSC_deathknight_spell_scripts()
{
    new spell_dk_might_of_ursoc();
    new spell_dk_wild_mushroom_plague();
    new spell_dk_dark_transformation_form();
    new spell_dk_asphyxiate();
    new spell_dk_desecrated_ground();
    new spell_dk_necrotic_strike();
    new spell_dk_festering_strike();
    new spell_dk_death_strike_heal();
    new spell_dk_howling_blast();
    new spell_dk_remorseless_winter();
    new spell_dk_soul_reaper();
    new spell_dk_pillar_of_frost();
    new spell_dk_blood_charges();
    new spell_dk_blood_tap();
    new spell_dk_death_siphon();
    new spell_dk_purgatory();
    new spell_dk_purgatory_absorb();
    new spell_dk_plague_leech();
    new spell_dk_unholy_blight();
    new spell_dk_chilblains();
    new spell_dk_outbreak();
    new spell_dk_raise_dead();
    new spell_dk_anti_magic_shell_raid();
    new spell_dk_anti_magic_shell_self();
    new spell_dk_anti_magic_zone();
    new spell_dk_ghoul_explode();
    new spell_dk_death_gate_teleport();
    new spell_dk_death_gate();
    new spell_dk_death_pact();
    new spell_dk_scourge_strike();
    new spell_dk_blood_boil();
    new spell_dk_death_grip();
    new spell_dk_death_grip_dummy();
    new spell_dk_gorefiends_grasp();
    new spell_dk_presence();
    new spell_dk_unholy_frenzy();
    new spell_dk_corpse_explosion();
    new spell_dk_soul_reaper_speed();
    new spell_dk_bone_shield();
    new spell_dk_blood_2p_bonus();
    new spell_dk_dancing_rune_weapon();
    new spell_dk_death_shroud();
    new spell_dk_blood_gorged();
}