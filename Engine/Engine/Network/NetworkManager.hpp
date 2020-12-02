#ifndef NETWORK_NETWORK_MANAGER_HPP
#define NETWORK_NETWORK_MANAGER_HPP
#include "../General/Macros.hpp"
#include "../General/Types.hpp"

struct LobbyData {
	// if this currently running program is the owner of the lobby
	bool thisIsOwner;
	// the lobby ID
	uint32 ID;
};

enum class NetStatus : uint8 {
	Failed,
	Connected,
	Disconnected,
	Loading,
	CreatingLobby,
	JoiningLobby
};

inline string NetStatusToString(NetStatus e) {
	switch (e) {
		case NetStatus::Failed:
			return "Failed";
		case NetStatus::Connected:
			return "Connected";
		case NetStatus::Disconnected:
			return "Disconnected";
		case NetStatus::Loading:
			return "Loading";
		case NetStatus::CreatingLobby:
			return "CreatingLobby";
		case NetStatus::JoiningLobby:
			return "JoiningLobby";
		default:
			return std::to_string(static_cast<std::underlying_type<NetStatus>::type>(e));
	}
}

class NetworkManager {
public:

	// initializes 
	static void Init();

	// cleans up
	static void Exit();

	// starts a lobby
	static void CreateLobby();

	// joins a lobby with a given code
	static void JoinLobby(const uint32 lobbyID);

	// exits the lobby
	// closes it if this is the owner
	static void ExitLobby();

	// gets the given status of of the network manager
	static NetStatus GetStatus();

	// gets the lobby data if connected to a lobby
	static const LobbyData* GetLobby();

	static uint32 GetUserID();

};

#endif // !NETWORK_NETWORK_MANAGER_HPP