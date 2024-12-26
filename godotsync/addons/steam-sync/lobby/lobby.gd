extends Control

@onready var lobby_main_menu : Control = $CenterContainer/LobbyMain
@onready var join_lobby_menu : Control = $CenterContainer/JoinLobbyMenu
@onready var create_lobby_menu : Control = $CenterContainer/CreateLobbyMenu
@onready var lobby_visibility : OptionButton = $CenterContainer/CreateLobbyMenu/MarginContainer/VBoxContainer/VisibilityLobbyOptBtn
@onready var max_players : OptionButton = $CenterContainer/CreateLobbyMenu/MarginContainer/VBoxContainer/LobbyMaxPlayerList
@onready var lobby_name_txt : TextEdit = $CenterContainer/CreateLobbyMenu/MarginContainer/VBoxContainer/LobbyNameTxt
@onready var lobby_distance_filter : OptionButton = $CenterContainer/JoinLobbyMenu/MarginContainer/VBoxContainer/SettingsPanel/VBoxContainer1/MarginContainer/DistanceFilterOpt
@onready var server_banner : PackedScene = preload("res://addons/steam-sync/lobby/server_banner.tscn")
@onready var lobby_lists_cont : Control = $CenterContainer/JoinLobbyMenu/MarginContainer/VBoxContainer/LobbyListPanel/MarginContainer/ScrollContainer/LobbyListContainer
@onready var lobby_name_lbl : Label = $CenterContainer/LobbyMenu/MarginContainer/HBoxContainer/VBoxContainer/LobbyNameLbl
@onready var lobby_menu : Control = $CenterContainer/LobbyMenu
@onready var lobby_member_cont : Control = $CenterContainer/LobbyMenu/MarginContainer/HBoxContainer/VBoxContainer/PanelContainer/ScrollContainer/MarginContainer/MemberCont
@onready var lobby_member_panel : PackedScene = preload("res://addons/steam-sync/lobby/member_panel.tscn")

enum MENU_VISIBILITY {LOBBY_MAIN,CREATE_LOBBY_MENU,JOIN_LOBBY_MENU,LOBBY_MENU}

func _ready() -> void:
	Steam.join_requested.connect(_on_lobby_join_requested)
	#Steam.lobby_chat_update.connect(_on_lobby_chat_update)
	Steam.lobby_created.connect(_on_lobby_created)
	#Steam.lobby_data_update.connect(_on_lobby_data_update)
	#Steam.lobby_invite.connect(_on_lobby_invite)
	Steam.lobby_joined.connect(_on_lobby_joined)
	Steam.lobby_match_list.connect(_on_lobby_match_list)
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
				join_lobby(int(these_arguments[1]))

func join_lobby(this_lobby_id: int) -> void:
	print("Attempting to join lobby %s" % this_lobby_id)

	NetworkManager.LOBBY_MEMBERS.clear()

	Steam.joinLobby(this_lobby_id)

func get_lobbies() -> void:
	# Set distance to worldwide
	Steam.addRequestLobbyListDistanceFilter(lobby_distance_filter.get_selected_id())

	print("Requesting a lobby list")
	Steam.requestLobbyList()
	
func leave_lobby() -> void:
	# If in a lobby, leave it
	if NetworkManager.LOBBY_ID != 0:
		# Send leave request to Steam
		Steam.leaveLobby(NetworkManager.LOBBY_ID)

		# Wipe the Steam lobby ID then display the default lobby ID and player list title
		NetworkManager.LOBBY_ID = 0

		# Close session with all users
		for this_member in NetworkManager.LOBBY_MEMBERS:
			# Make sure this isn't your Steam ID
			if this_member['steam_id'] != NetworkManager.STEAM_ID:

				# Close the P2P session
				Steam.closeP2PSessionWithUser(this_member['steam_id'])

		# Clear the local lobby list
		NetworkManager.LOBBY_MEMBERS.clear()

func get_lobby_members() -> void:
	NetworkManager.LOBBY_MEMBERS.clear()
	for MEMBER in lobby_member_cont.get_children():
		MEMBER.hide()
		MEMBER.queue_free()
	# Get the number of members from this lobby from Steam
	var num_of_members: int = Steam.getNumLobbyMembers(NetworkManager.LOBBY_ID)

	# Get the data of these players from Steam
	for this_member in range(0, num_of_members):
		# Get the member's Steam ID
		var member_steam_id: int = Steam.getLobbyMemberByIndex(NetworkManager.LOBBY_ID, this_member)
		
		# Get the member's Steam name
		var member_steam_name: String = Steam.getFriendPersonaName(member_steam_id)

		# Add them to the list
		add_member_to_list(member_steam_id,member_steam_name)

func add_member_to_list(steam_id: int, steam_name: String):
	print("Adding new player to the list: "+str(steam_id)+" / "+str(steam_name))
	# Add them to the list
	NetworkManager.LOBBY_MEMBERS.append({"steam_id":steam_id, "steam_name":steam_name })
	# Instance the lobby member object
	var THIS_MEMBER: Object = lobby_member_panel.instantiate()
	# Add their Steam name and ID
	THIS_MEMBER.name = str(steam_id)
	THIS_MEMBER.set_member_panel(steam_id, steam_name)
	# Add the child node
	lobby_member_cont.add_child(THIS_MEMBER)
func make_p2p_handshake() -> void:
	pass
	
func change_lobby_ui(visibility : MENU_VISIBILITY):
	match visibility:
		MENU_VISIBILITY.LOBBY_MAIN:
			lobby_main_menu.visible = true
			join_lobby_menu.visible = false
			create_lobby_menu.visible = false
			lobby_menu.visible = false
		MENU_VISIBILITY.CREATE_LOBBY_MENU:
			lobby_main_menu.visible = false
			join_lobby_menu.visible = false
			create_lobby_menu.visible = true
			lobby_menu.visible = false
		MENU_VISIBILITY.JOIN_LOBBY_MENU:
			lobby_main_menu.visible = false
			join_lobby_menu.visible = true
			create_lobby_menu.visible = false
			lobby_menu.visible = false
		MENU_VISIBILITY.LOBBY_MENU:
			lobby_main_menu.visible = false
			join_lobby_menu.visible = false
			create_lobby_menu.visible = false
			lobby_menu.visible = true
			
			
#region Steam Callbacks
func _on_lobby_join_requested(lobby_id: int, steam_id: int) -> void:
	pass

func _on_lobby_created( connect: int, lobby_id: int) -> void:
	if connect == 1:
		# Set the lobby ID
		NetworkManager.LOBBY_ID = lobby_id
		print("Created a lobby: %s" % NetworkManager.LOBBY_ID )
		# Set this lobby as joinable, just in case, though this should be done by default
		Steam.setLobbyJoinable(NetworkManager.LOBBY_ID, true)
		# Set some lobby data
		if lobby_name_txt.text == null or lobby_name_txt.text == "":
			lobby_name_txt.text = str(Steam.getFriendPersonaName(NetworkManager.STEAM_ID) + "'s Lobby") 
		Steam.setLobbyData(NetworkManager.LOBBY_ID, "name", lobby_name_txt.text)
		Steam.setLobbyData(NetworkManager.LOBBY_ID, "mode", "SteamSyncLobby")

		# Allow P2P connections to fallback to being relayed through Steam if needed
		var set_relay: bool = Steam.allowP2PPacketRelay(true)
		
		get_lobby_members()


func _on_lobby_joined( lobby: int, permissions: int, locked: bool, response: int) -> void:
	# If joining succeed, this will be 1
	if response == 1:
		# Set this lobby ID as your lobby ID
		NetworkManager.LOBBY_ID = lobby
		# Print the lobby ID to a label
		lobby_name_lbl.set_text(Steam.getLobbyData(NetworkManager.LOBBY_ID,"name"))
		# Append to output
		print("[STEAM] Joined lobby "+str(NetworkManager.LOBBY_ID)+".\n")
		# Get the lobby members
		
		# Make the initial handshake
		make_p2p_handshake()
		get_lobby_members()
		change_lobby_ui(MENU_VISIBILITY.LOBBY_MENU)
		
	# Else it failed for some reason
	else:
		# Get the failure reason
		var FAIL_REASON: String
		match response:
			2:	FAIL_REASON = "This lobby no longer exists."
			3:	FAIL_REASON = "You don't have permission to join this lobby."
			4:	FAIL_REASON = "The lobby is now full."
			5:	FAIL_REASON = "Uh... something unexpected happened!"
			6:	FAIL_REASON = "You are banned from this lobby."
			7:	FAIL_REASON = "You cannot join due to having a limited account."
			8:	FAIL_REASON = "This lobby is locked or disabled."
			9:	FAIL_REASON = "This lobby is community locked."
			10:	FAIL_REASON = "A user in the lobby has blocked you from joining."
			11:	FAIL_REASON = "A user you have blocked is in the lobby."
		print("[STEAM] Failed joining lobby "+str(lobby)+": "+str(FAIL_REASON)+"\n")
		# Reopen the server list
		#_on_Open_Lobby_List_pressed()

func _on_persona_change( steam_id: int, flags: int) -> void:
	pass

func _on_p2p_session_request( remote_steam_id: int) -> void:
	pass

func _on_p2p_session_connect_fail(remote_steam_id: int, session_error: int) -> void:
	pass

func _on_lobby_match_list(lobbies: Array):
	for this_lobby in lobbies:
		var server_banner_instance = server_banner.instantiate()
		var lobby_name : String = Steam.getLobbyData(this_lobby, "name")
		var lobby_num_members: int = Steam.getNumLobbyMembers(this_lobby)
		var max_num_members : int = Steam.getLobbyMemberLimit(this_lobby)
		server_banner_instance.set_server_banner(lobby_name,lobby_num_members,max_num_members)
		lobby_lists_cont.add_child(server_banner_instance)
#endregion





#region SIGNALS

func _on_open_join_lobby_btn_pressed() -> void:
	get_lobbies()
	change_lobby_ui(MENU_VISIBILITY.JOIN_LOBBY_MENU)

func _on_open_lobby_create_btn_pressed() -> void:
	change_lobby_ui(MENU_VISIBILITY.CREATE_LOBBY_MENU)
func _on_back_btn_pressed() -> void:
	if NetworkManager.LOBBY_ID != 0:
		leave_lobby()
	change_lobby_ui(MENU_VISIBILITY.LOBBY_MAIN)


func _on_create_lobby_btn_pressed() -> void:
	if NetworkManager.LOBBY_ID == 0:
		var max_player : int = 4
		if max_players.get_selected_id() == 0:
			max_player = 2
		elif max_players.get_selected_id() == 1:
			max_player = 4	
		elif max_players.get_selected_id() == 2:
			max_player = 8

		Steam.createLobby(lobby_visibility.get_selected_id(), max_player)

func _on_refresh_btn_pressed() -> void:
	get_lobbies()
	
func _on_start_btn_pressed() -> void:
	pass # Replace with function body.


func _on_ready_check_box_toggled(toggled_on: bool) -> void:
	pass # Replace with function body.

#endregion
