/**
 * Provides stocks that deal with user messages.
 */

#if defined __stocksoup_tf_hud_notify_included
	#endinput
#endif

#define __stocksoup_tf_hud_notify_included

#include <tf2>

/**
 * Displays a hintbox-like notification to a single client.  This notification isn't displayed
 * to players that have `cl_hud_minmode` on.
 * 
 * @param client		A single client to display to.  Use TF2_ShowHudNotification if you want
 * 						to send notifications to multiple clients.
 * @param icon			The name of the icon to display.  A list of valid icon names is
 * 						available in `tf/scripts/mod_textures.txt`.
 * @param team			The team color to use as the panel background.  If a playable team isn't
 * 						specified, a neutral color is used.
 */
stock void TF2_ShowHudNotificationToClient(int client, const char[] icon,
		TFTeam team = TFTeam_Unassigned, const char[] format, any ...) {
	SetGlobalTransTarget(client);
	
	char message[256];
	VFormat(message, sizeof(message), format, 5);
	
	__stocksoup_TF2_ShowHudNotificationInternal(client, icon, team, message);
	
	SetGlobalTransTarget(LANG_SERVER);
}

/**
 * Displays a hintbox-like notification to all clients.  This notification isn't displayed
 * to players that have `cl_hud_minmode` on.
 * 
 * @param icon			The name of the icon to display.  A list of valid icon names is
 * 						available in `tf/scripts/mod_textures.txt`.
 * @param team			The team color to use as the panel background.  If a playable team isn't
 * 						specified, a neutral color is used.
 */
stock void TF2_ShowHudNotificationToAll(const char[] icon, TFTeam team = TFTeam_Unassigned,
		const char[] format, any ...) {
	char message[256];
	for (int i = 1; i <= MaxClients; i++) {
		if (!IsClientInGame(i)) {
			continue;
		}
		SetGlobalTransTarget(i);
		
		VFormat(message, sizeof(message), format, 4);
		
		__stocksoup_TF2_ShowHudNotificationInternal(i, icon, team, message);
	}
	SetGlobalTransTarget(LANG_SERVER);
}

/**
 * Displays a hintbox-like notification to multiple clients.  This notification isn't displayed
 * to players that have `cl_hud_minmode` on.
 * 
 * @param clients		An array of client indices to display to.
 * @param numClients	The number of clients in the array.
 * @param icon			The name of the icon to display.  A list of valid icon names is
 * 						available in `tf/scripts/mod_textures.txt`.
 * @param team			The team color to use as the panel background.  If a playable team isn't
 * 						specified, a neutral color is used.
 */
stock void TF2_ShowHudNotification(int[] clients, int numClients, const char[] icon,
		TFTeam team = TFTeam_Unassigned, const char[] format, any ...) {
	char message[256];
	for (int i = 0; i < numClients; i++) {
		int client = clients[i];
		SetGlobalTransTarget(client);
		
		VFormat(message, sizeof(message), format, 6);
		
		__stocksoup_TF2_ShowHudNotificationInternal(client, icon, team, message);
	}
	SetGlobalTransTarget(LANG_SERVER);
}

/**
 * This is an internally-called function.
 * 
 * Sends a custom HUD notification with a raw string.  This is used instead of wrapping one of
 * the public-facing functions around the other, which would cause `VFormat` to be invoked in
 * each function.
 * 
 * By default, this will generate the notification on the next frame.  To override this and
 * notify immediately, define STOCKSOUP_USE_IMMEDIATE_HUD_NOTIFY.
 */
static stock void __stocksoup_TF2_ShowHudNotificationInternal(int client, const char[] icon,
		TFTeam team = TFTeam_Unassigned, const char[] message) {
#if !defined STOCKSOUP_USE_IMMEDIATE_HUD_NOTIFY
	/**
	 * If STOCKSOUP_USE_IMMEDIATE_HUD_NOTIFY is defined, bypass queueing the notification to run
	 * on the next frame.  This makes it run immediately, but you do run the risk of crashing
	 * the server if the HUD notification is created in a HookUserMessage callback.
	 */
	DataPack data = CreateDataPack();
	data.WriteCell(GetClientSerial(client));
	data.WriteString(icon);
	data.WriteCell(team);
	data.WriteString(message);
	
	RequestFrame(__internal_stocksoup_ShowHudNotification, data);
}

stock void __internal_stocksoup_ShowHudNotification(DataPack data) {
	data.Reset();
	
	char icon[64], message[256];
	int client = GetClientFromSerial(data.ReadCell());
	data.ReadString(icon, sizeof(icon));
	TFTeam team = data.ReadCell();
	data.ReadString(message, sizeof(message));
	delete data;
	
	if (!client) {
		return;
	}
#endif
	BfWrite buffer = view_as<BfWrite>(StartMessageOne("HudNotifyCustom", client));
	
	buffer.WriteString(message);
	buffer.WriteString(icon);
	buffer.WriteByte(view_as<int>(team));
	
	EndMessage();
}

/**
 * Deprecated.
 */
#pragma deprecated Use TF2_ShowHudNotificationToClient instead
stock void TF_HudNotifyCustom(int client, const char[] icon, TFTeam team, const char[] format,
		any ...) {
	if (client <= 0 || client > MaxClients) {
		ThrowError("Invalid client index %d", client);
	} else if (!IsClientInGame(client)) {
		ThrowError("Client %d is not in game", client);
	}
	
	char buffer[256];
	VFormat(buffer, sizeof(buffer), format, 5);
	
	TF_HudNotifyCustomParams(view_as<BfWrite>(StartMessageOne("HudNotifyCustom", client)),
			buffer, icon, team);
}

#pragma deprecated Use TF2_ShowHudNotification instead
stock void TF_HudNotifyCustomParams(BfWrite bitbuf, const char[] message, const char[] icon,
		TFTeam team) {
	bitbuf.WriteString(message);
	bitbuf.WriteString(icon);
	bitbuf.WriteByte(view_as<int>(team));
	
	EndMessage();
}
