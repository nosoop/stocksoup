#if defined __stocksoup_tempents_stocks_included
	#endinput
#endif

#define __stocksoup_tempents_stocks_included

#include <sdktools_tempents>

/**
 * Sets up a player animation event.
 * Note that due to multiple factors, this is not guaranteed to work, especially on the client.
 */
stock void TE_SetupPlayerAnimEvent(int client, int iEvent, int nData) {
	TE_Start("PlayerAnimEvent");
	
	TE_WriteNum("m_iPlayerIndex", client);
	TE_WriteNum("m_iEvent", iEvent);
	TE_WriteNum("m_nData", nData);
}
