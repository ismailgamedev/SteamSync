extends RigidBody2D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	if Steam.getLobbyOwner(NetworkManager.LOBBY_ID) != NetworkManager.STEAM_ID:
		freeze_mode = FREEZE_MODE_KINEMATIC
