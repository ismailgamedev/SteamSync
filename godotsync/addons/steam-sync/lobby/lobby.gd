extends Control

@onready var lobby_menu : Control = $CenterContainer/LobbyMain
@onready var join_lobby_menu : Control = $CenterContainer/JoinLobbyMenu
@onready var create_lobby_menu : Control = $CenterContainer/CreateLobbyMenu

func _ready() -> void:
	Steam.join_requested.connect(_on_lobby_join_requested)
	#Steam.lobby_chat_update.connect(_on_lobby_chat_update)
	Steam.lobby_created.connect(_on_lobby_created)
	#Steam.lobby_data_update.connect(_on_lobby_data_update)
	#Steam.lobby_invite.connect(_on_lobby_invite)
	Steam.lobby_joined.connect(_on_lobby_joined)
	#Steam.lobby_match_list.connect(_on_lobby_match_list)
	#Steam.lobby_message.connect(_on_lobby_message)
	Steam.persona_state_change.connect(_on_persona_change)
	Steam.p2p_session_request.connect(_on_p2p_session_request)
	Steam.p2p_session_connect_fail.connect(_on_p2p_session_connect_fail)


	# Check for command line arguments
	check_command_line()

func check_command_line() -> void:
	var these_arguments: Array = OS.get_cmdline_args()

	# There are arguments to process
	if these_arguments.size() > 0:

		# A Steam connection argument exists
		if these_arguments[0] == "+connect_lobby":

			# Lobby invite exists so try to connect to it
			if int(these_arguments[1]) > 0:

				# At this point, you'll probably want to change scenes
				# Something like a loading into lobby screen
				print("Command line lobby ID: %s" % these_arguments[1])


func join_lobby(this_lobby_id: int) -> void:
	print("Attempting to join lobby %s" % this_lobby_id)

	NetworkManager.LOBBY_MEMBERS.clear()

	Steam.joinLobby(this_lobby_id)


#region Steam Callbacks
func _on_lobby_join_requested(lobby_id: int, steam_id: int) -> void:
	pass

func _on_lobby_created( connect: int, lobby_id: int) -> void:
	pass

func _on_lobby_joined( lobby: int, permissions: int, locked: bool, response: int) -> void:
	pass

func _on_persona_change( steam_id: int, flags: int) -> void:
	pass

func _on_p2p_session_request( remote_steam_id: int) -> void:
	pass

func _on_p2p_session_connect_fail(remote_steam_id: int, session_error: int) -> void:
	pass
#endregion





#region SIGNALS
func _on_open_join_lobby_btn_pressed() -> void:
	lobby_menu.visible = false
	join_lobby_menu.visible = true
	create_lobby_menu.visible = false

func _on_create_lobby_btn_pressed() -> void:
	pass

func _on_open_lobby_create_btn_pressed() -> void:
	lobby_menu.visible = false
	join_lobby_menu.visible = false
	create_lobby_menu.visible = true	
func _on_back_btn_pressed() -> void:
	lobby_menu.visible = true
	join_lobby_menu.visible = false
	create_lobby_menu.visible = false	

#endregion
