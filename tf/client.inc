/**
 * Provides stocks for client (i.e., the game with a human controlling the character) functions.
 */

#if defined __stocksoup_tf_client_included
	#endinput
#endif

#define __stocksoup_tf_client_included

#include <tf2_stocks>

/**
 * Sets a client's overlay to the appropriate team's invulnerability overlay.
 * 
 * @param client		Client to receive the overlay.
 */
stock void TF2_SetClientUberchargeOverlay(int client) {
	TFTeam team = TF2_GetClientTeam(client);
	switch (team) {
		case TFTeam_Red: {
			SetClientScreenOverlay(client, "effects/invuln_overlay_red");
		}
		case TFTeam_Blue: {
			SetClientScreenOverlay(client, "effects/invuln_overlay_blue");
		}
	}
}
