#if defined __stocksoup_tf_tempents_stocks_included
	#endinput
#endif

#define __stocksoup_tf_tempents_stocks_included

#include <sdktools_stringtables>
#include <sdktools_tempents>

/**
 * Enum values from `src/game/shared/particle_parse.h`.
 */
enum ParticleAttachment_t {
	PATTACH_ABSORIGIN = 0,
	PATTACH_ABSORIGIN_FOLLOW,
	PATTACH_CUSTOMORIGIN,
	PATTACH_POINT,
	PATTACH_POINT_FOLLOW,
	PATTACH_WORLDORIGIN,
	PATTACH_ROOTBONE_FOLLOW
};

/**
 * Sets up a TF2 particle effect.
 * 
 * Attachment points are dependent on the entity; `tf_fx.cpp` normally pulls this index by
 * calling `pEntity->GetBaseAnimating()->LookupAttachment( pszAttachmentName )`.
 * 
 * @param name				Name of particle effect.
 * @param vecOrigin			Position of the particle effect.
 * @param vecStart			Starting position of the particle effect.
 * @param vecAngles			Angle of particle effect.
 * @param entity			Entity that the particle effect is applied to.
 * @param attachType		Attachment type (see ParticleAttachment_t for valid values).
 * @param attachPoint		Attachment point value.
 * @param bResetParticles	Whether or not particles are reset on an entity.
 */
stock void TE_SetupTFParticleEffect(const char[] name, const float vecOrigin[3],
		const float vecStart[3] = NULL_VECTOR, const float vecAngles[3] = NULL_VECTOR,
		int entity = -1, ParticleAttachment_t attachType = PATTACH_ABSORIGIN,
		int attachPoint = -1, bool bResetParticles = false) {
	int particleTable, particleIndex;
	
	if ((particleTable = FindStringTable("ParticleEffectNames")) == INVALID_STRING_TABLE) {
		ThrowError("Could not find string table: ParticleEffectNames");
	}
	
	if ((particleIndex = FindStringIndex(particleTable, name)) == INVALID_STRING_INDEX) {
		ThrowError("Could not find particle index: %s", name);
	}
	
	TE_Start("TFParticleEffect");
	TE_WriteFloat("m_vecOrigin[0]", vecOrigin[0]);
	TE_WriteFloat("m_vecOrigin[1]", vecOrigin[1]);
	TE_WriteFloat("m_vecOrigin[2]", vecOrigin[2]);
	TE_WriteFloat("m_vecStart[0]", vecStart[0]);
	TE_WriteFloat("m_vecStart[1]", vecStart[1]);
	TE_WriteFloat("m_vecStart[2]", vecStart[2]);
	TE_WriteVector("m_vecAngles", vecAngles);
	TE_WriteNum("m_iParticleSystemIndex", particleIndex);
	
	if (entity != -1) {
		TE_WriteNum("entindex", entity);
	}
	
	if (attachType != PATTACH_ABSORIGIN) {
		TE_WriteNum("m_iAttachType", view_as<int>(attachType));
	}
	
	if (attachPoint != -1) {
		TE_WriteNum("m_iAttachmentPointIndex", attachPoint);
	}
	
	TE_WriteNum("m_bResetParticles", bResetParticles ? 1 : 0);
}

/**
 * Sets up a TF2 explosion.
 * 
 * @param vecOrigin				Position of the explosion.
 * @param vecNormal				Normal vector.
 * @param weaponid				Weapon ID (one of the TF_WEAPON_* values).
 * @param entity				Entity that the effect is applied to, 0 for world.
 * 								Clamped to 0x7FF.
 * @param itemDefinitionIndex	Weapon's definition index.
 * @param sound					WeaponSound_t enum value.  See mp/src/game/shared/weapon_parse.h
 * 								in the 2013 Source SDK.
 * @param particleIndex			Custom particle index.
 */
stock void TE_SetupTFExplosion(const float vecOrigin[3],
		const float vecNormal[3] = { 0.0, 0.0, 1.0 }, int weaponid = TF_WEAPON_NONE,
		int entity = 0, int definitionIndex = -1, int sound = -1, int particleIndex = -1) {
	TE_Start("TFExplosion");
	
	TE_WriteFloat("m_vecOrigin[0]", vecOrigin[0]);
	TE_WriteFloat("m_vecOrigin[1]", vecOrigin[1]);
	TE_WriteFloat("m_vecOrigin[2]", vecOrigin[2]);
	
	TE_WriteVector("m_vecNormal", vecNormal);
	
	// CTFWeaponBase::GetWeaponID()
	TE_WriteNum("m_iWeaponID", weaponid);
	
	// entity it is applied to, 0 for world -- if it's on a stickybomb it's 0x7FF
	// (or more likely clamped to maximum edicts)
	TE_WriteNum("entindex", entity);
	
	// item definition index
	TE_WriteNum("m_nDefID", definitionIndex & 0xFFFF);
	
	// WeaponSound_t enum value
	TE_WriteNum("m_nSound", sound & 0xFFFF);
	
	// written as a short, -1 == 0xFFFF
	TE_WriteNum("m_iCustomParticleIndex", particleIndex & 0xFFFF);
}
