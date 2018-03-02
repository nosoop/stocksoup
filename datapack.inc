/**
 * A few stock functions to work with data packs.
 */

#if defined __stocksoup_datapack_included
	#endinput
#endif

#define __stocksoup_datapack_included

/**
 * Packs a fixed-size cell array into a data pack.
 */
stock void WritePackCellArray(DataPack pack, const any[] arr, int size) {
	for (int i = 0; i < size; i++) {
		pack.WriteCell(arr[i]);
	}
}

/**
 * Reads a fixed-size cell array from a data pack.
 */
stock void ReadPackCellArray(DataPack pack, any[] arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = pack.ReadCell();
	}
}

/**
 * Packs a fixed-size float array into a data pack.
 */
stock void WritePackFloatArray(DataPack pack, const float[] arr, int size) {
	for (int i = 0; i < size; i++) {
		pack.WriteFloat(arr[i]);
	}
}

/**
 * Reads a fixed-size float array from a data pack.
 */
stock void ReadPackFloatArray(DataPack pack, float[] arr, int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = pack.ReadFloat();
	}
}

/**
 * Packs a vector (float[3]) into a data pack.
 */
stock void WritePackVector(DataPack pack, const float vec[3]) {
	WritePackFloatArray(pack, vec, sizeof(vec));
}

/**
 * Reads a vector (float[3]) from a data pack.
 */
stock void ReadPackVector(DataPack pack, float vec[3]) {
	ReadPackFloatArray(pack, vec, sizeof(vec));
}

/**
 * Packs a variable-length list of clients into a data pack.  Clients are added by their serial.
 */
stock void WritePackClientList(DataPack pack, const int[] clients, int nClients) {
	pack.WriteCell(nClients);
	for (int i = 0; i < nClients; i++) {
		pack.WriteCell(GetClientSerial(clients[i]));
	}
}

/**
 * Reads a variable-length list of client indices from a data pack.  Any now-invalid clients are
 * discarded.
 * 
 * @return Number of valid clients.
 */
stock int ReadPackClientList(DataPack pack, int[] clients, int size) {
	int nPackedClients = pack.ReadCell(), nAvailableClients;
	for (int i = 0; i < nPackedClients; i++) {
		int client = GetClientFromSerial(pack.ReadCell());
		
		if (client) {
			clients[nAvailableClients++] = client;
		}
	}
	return nAvailableClients;
}
