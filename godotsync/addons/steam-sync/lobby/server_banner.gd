extends Panel



@onready var lobby_id : int = 0
func set_server_banner(name : String,max_players : int,current_players : int) -> void:
	
	$HBoxContainer/LobbyNameLbl.text = name
	# CurrentPlayers/MaxPlayers
	$HBoxContainer/MarginContainer2/PlayersLbl.text = "%s/%s" % [current_players,max_players]


func _on_join_to_lobby_pressed() -> void:
	get_tree().current_scene.call("join_lobby",lobby_id)
