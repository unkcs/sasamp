#pragma once

#define MAX_SCRIPT_VARS 16
#define MAX_SCRIPT_SIZE	255

struct GAME_SCRIPT_THREAD
{
	uint8_t Pad1[20];			// +00
	uintptr_t dwScriptIP;		// +20
	uint8_t Pad2[36];			// +24
	uint32_t dwLocalVar[32];	// +60
	uint32_t dwTimers[2];		// +188
	uint8_t Pad3[33];			// +196
	uint8_t condResult;			// +229
	uint8_t Pad4[10];			// +230
	uint16_t logicalOp;			// +240
	uint8_t notFlag;			// +242
	uint8_t Pad5[13];			// +243
	// STRUCT SIZE = 256
};

struct SCRIPT_COMMAND
{
	uint16_t OpCode;
	char Params[MAX_SCRIPT_VARS];
};

int ScriptCommand(const SCRIPT_COMMAND* pScriptCommand, ...);

const SCRIPT_COMMAND lock_camera_position = { 0x092F, "i" };
const SCRIPT_COMMAND lock_camera_position1 = { 0x0930, "i" };
const SCRIPT_COMMAND is_player_targetting_char = { 0x0457, "ii" };
const SCRIPT_COMMAND rotate_attached_object_to_ped = { 0x05A1, "ifff" };
const SCRIPT_COMMAND attach_object_to_bone = { 0x070A, "iifffiissi" };
const SCRIPT_COMMAND attach_object_to_ped2 = { 0x09A0, "iifffiissi" };
const SCRIPT_COMMAND create_marker_icon = { 0x02A8, "fffiv" };
//const SCRIPT_COMMAND create_car_generator				= { 0x014B, "iifffiissi" };
const SCRIPT_COMMAND turn_on_car_engine = { 0x0abe,	"i" };
const SCRIPT_COMMAND turn_car_engine = { 0x0918, "ii" };
const SCRIPT_COMMAND set_camera_pos_time_smooth = { 0x0936, "ffffffii" };
const SCRIPT_COMMAND point_camera_transverse = { 0x0920, "ffffffii" };
const SCRIPT_COMMAND restore_camera_to_user = { 0x0925, "" };
const SCRIPT_COMMAND create_arrow_above_actor = { 0x0187, "iv" };
const SCRIPT_COMMAND set_player_skin = { 0x09C7, "ii" };
const SCRIPT_COMMAND request_model = { 0x0247, "i" };		// (CAR_*|BIKE_*|BOAT_*|WEAPON_*|OBJECT_*)
const SCRIPT_COMMAND load_requested_models = { 0x038B, "" };		// -/-
const SCRIPT_COMMAND create_car = { 0x00A5, "ifffv" };	// (CAR_*|BIKE_*|BOAT_*), x, y, z, var_car
const SCRIPT_COMMAND load_special_actor = { 0x023C, "is" };		// SPECIAL_*, MODEL_*
const SCRIPT_COMMAND create_actor = { 0x009A, "iifffv" };	// PEDTYPE_*, #MODEL, x, y, z, var_actor
const SCRIPT_COMMAND GET_CAR_DOOR_LOCK_STATUS = { 0x09B3, "ii" };	//
const SCRIPT_COMMAND destroy_actor_fading = { 0x034F, "i" };		// var_actor
const SCRIPT_COMMAND DELETE_CHAR = { 0x009b, "i" };		// var_actor
const SCRIPT_COMMAND set_weather = { 0x01B6, "i" };		// WEATHER_*
const SCRIPT_COMMAND set_fade_color = { 0x0169, "iii" };	// Red(0-255), Green(0-255), Blue(0-255)
const SCRIPT_COMMAND fade = { 0x016A, "ii" };		// (time in ms), FADE_*
const SCRIPT_COMMAND is_fading = { 0x016B, "" };		// -/-
const SCRIPT_COMMAND set_char_coordinates = { 0x00A1, "ifff" };		// PLAYER_CHAR, X, Y, Z
const SCRIPT_COMMAND name_thread = { 0x03A4, "s" };		// "MAIN"
const SCRIPT_COMMAND set_max_wanted_level = { 0x01F0, "i" };		// MaxLevel
const SCRIPT_COMMAND set_wasted_busted_check = { 0x0111, "i" };		// Check(1/0)
const SCRIPT_COMMAND set_current_time = { 0x00C0, "ii" };		// Hours, Minutes
const SCRIPT_COMMAND get_current_time = { 0x00BF, "vv" };		// Hours, Minutes
const SCRIPT_COMMAND refresh_screen = { 0x04E4, "ff" };		// x, y
const SCRIPT_COMMAND set_camera = { 0x03CB, "fff" };	// x, y, z
const SCRIPT_COMMAND create_player = { 0x0053, "vfffv" };	// 0, x, y, z, PLAYER_CHAR
const SCRIPT_COMMAND create_actor_from_player = { 0x01F5, "vv" };		// PLAYER_CHAR, PLAYER_ACTOR
const SCRIPT_COMMAND set_actor_skin = { 0x0352, "vs" };		// var_actor, MODEL_*
const SCRIPT_COMMAND refresh_actor_skin = { 0x0353, "v" };		// var_actor
const SCRIPT_COMMAND select_interior = { 0x04BB, "i" };		// INTERIOR_*
const SCRIPT_COMMAND make_actor_leave_vehicle = { 0x03E2, "v" };		// var_actor
const SCRIPT_COMMAND clear_actor_objective = { 0x011C, "v" };		// var_actor
const SCRIPT_COMMAND set_car_color = { 0x0229, "iii" };	// var_car col1 col2
const SCRIPT_COMMAND wait = { 0x0001, "i" };		// time_in_ms
const SCRIPT_COMMAND create_thread = { 0x00D7, "iz" };		// StartIP (4F)
const SCRIPT_COMMAND end_thread = { 0x004E, "" };		// -/-
const SCRIPT_COMMAND destroy_car = { 0x00A6, "i" };		// var_car
const SCRIPT_COMMAND set_car_z_angle = { 0x0175, "if" };		// var_car, angle
const SCRIPT_COMMAND car_relative_coordinates = { 0x0407, "vfffvvv" };// var_car, x, y, z, var_x, var_y, var_z
const SCRIPT_COMMAND set_car_max_speed = { 0x00AD, "if" };		// var_car, speed
const SCRIPT_COMMAND is_model_available = { 0x0248, "i" };		// #MODEL
const SCRIPT_COMMAND SET_CHAR_AMMO = { 0x017B, "iii" };	// var_actor, weapon, ammo
const SCRIPT_COMMAND set_camera_behind_player = { 0x0373, "" };		// -/-
const SCRIPT_COMMAND camera_on_player = { 0x0157, "vii" };	// var_player, ukn, ukn
const SCRIPT_COMMAND is_char_playing_anim = { 0x0611, "is" };
const SCRIPT_COMMAND camera_on_vehicle = { 0x0158, "iii" };	// var_car, ukn, ukn
const SCRIPT_COMMAND camera_on_actor = { 0x0159, "iii" };	// var_actor, ukn, ukn
const SCRIPT_COMMAND restore_camera = { 0x015A, "" };		// -/-
const SCRIPT_COMMAND point_camera = { 0x0160, "fffi" };	// x, y, z, type
const SCRIPT_COMMAND restore_camera_jumpcut = { 0x02EB, "" };		// -/-
const SCRIPT_COMMAND set_camera_position = { 0x015F, "ffffff" }; // x, y, z, vx, vy, vz
const SCRIPT_COMMAND tie_actor_to_player = { 0x01DF, "vv" };		// var_actor, PLAYER_CHAR
const SCRIPT_COMMAND tie_marker_to_car = { 0x0161, "iiiv" };	// var_car, ukn, ukn, var_marker
const SCRIPT_COMMAND tie_marker_to_actor = { 0x0162, "iiiv" };	// var_actor, ukn, ukn, var_marker
const SCRIPT_COMMAND disable_marker = { 0x0164, "i" };		// var_marker
const SCRIPT_COMMAND set_marker_color = { 0x0165, "ii" };		// var_marker, color
const SCRIPT_COMMAND set_marker_brightness = { 0x0166, "ii" };		// var_marker, brightness
const SCRIPT_COMMAND create_marker = { 0x0167, "fffiiv" };	// x, y, z, ukn, ukn, var_marker
const SCRIPT_COMMAND create_radar_marker_without_sphere = { 0x04CE, "fffiv" };	// x, y, z, marker_type, var_marker
const SCRIPT_COMMAND show_on_radar = { 0x0168, "ii" };		// var_marker, size
const SCRIPT_COMMAND set_car_driver_behaviour = { 0x00AF, "vi" };		// var_car, behaviour
const SCRIPT_COMMAND set_actor_to_kill_actor = { 0x01C9, "vv" };		// var_actor, var_actor
const SCRIPT_COMMAND set_actor_to_kill_player = { 0x01CA, "vv" };		// var_actor, PLAYER_CHAR
const SCRIPT_COMMAND is_actor_dead = { 0x0118, "v" };		// var_actor
const SCRIPT_COMMAND player_near_point_on_foot = { 0x00F6, "vffffffi" };//	PLAYER_CHAR, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND create_icon_marker_sphere = { 0x02A7, "fffiv" };
const SCRIPT_COMMAND is_player_near_point_3d = { 0x00F5, "iffffffi" };//	PLAYER_CHAR, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND is_actor_near_point_3d = { 0x00FE, "iffffffi" };//	var_actor, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND is_car_near_point_3d = { 0x01AF, "iffffffi" };//	var_car, x, y, z, rx, ry, rz, b
const SCRIPT_COMMAND set_player_z_angle = { 0x0171, "vf" };
const SCRIPT_COMMAND give_player_weapon = { 0x01B1, "iii" };	// PLAYER_CHAR, weapon, ammo
const SCRIPT_COMMAND get_line_of_sight = { 0x06BD, "ffffffiiiii" }; // x1, y1, z1, x2, y2, z2, solid, vehicle, actor, obj, fx

const SCRIPT_COMMAND get_ground_z = { 0x02ce, "fffv" };	// x, y, z, var_ground_z

//const SCRIPT_COMMAND is_actor_near_point_3d		= {0x00FE,	"iffffffi"};
const SCRIPT_COMMAND create_racing_checkpoint = { 0x06d5,	"ifffffffv" };	// type, x, y, z, nx, ny. nz, size, $id (n=next checkpoint)
/* 0       = Street Checkpoint with arrow to the next checkpoint
   1 and 2 = Street Checkpoint
   3 and 4 = Flight Checkpoint (circle) */
const SCRIPT_COMMAND destroy_racing_checkpoint = { 0x06d6,	"i" };

// added by kyeman. (I don't use correct conventions so don't copy elsewhere
// without double checking they're how you want them, yalls.

const SCRIPT_COMMAND restart_if_wasted_at = { 0x016C, "ffffi" };
const SCRIPT_COMMAND toggle_player_infinite_run = { 0x0330, "ii" };
const SCRIPT_COMMAND toggle_player_controllable = { 0x01B4, "ii" }; // отключает элементы управления
const SCRIPT_COMMAND toggle_player_fast_reload = { 0x0331, "ii" };
const SCRIPT_COMMAND set_actor_armed_weapon = { 0x01b9, "ii" };
const SCRIPT_COMMAND get_actor_armed_weapon = { 0x0470, "iv" };
const SCRIPT_COMMAND remove_player_weapons = { 0x03b8, "i" };
const SCRIPT_COMMAND TASK_WARP_CHAR_INTO_CAR_AS_DRIVER = { 0x072A, "ii" };
const SCRIPT_COMMAND put_player_at_and_remove_from_car = { 0x012a, "ifff" };
const SCRIPT_COMMAND set_actor_immunities = { 0x02ab, "iiiiii" };
const SCRIPT_COMMAND set_car_immunities = { 0x02AC, "iiiiii" };
const SCRIPT_COMMAND set_actor_can_be_decapitated = { 0x0446, "ii" };
const SCRIPT_COMMAND create_object = { 0x0107, "ifffv" };
const SCRIPT_COMMAND set_object_z_angle = { 0x0177, "if" };
const SCRIPT_COMMAND set_object_rotation = { 0x0453, "ifff" };
const SCRIPT_COMMAND set_object_collision = { 0x0382, "ii" };
const SCRIPT_COMMAND put_object_at = { 0x01Bc, "ifff" };
const SCRIPT_COMMAND move_object = { 0x034e, "iffffffi" };
const SCRIPT_COMMAND make_object_moveable = { 0x0392, "ii" };
const SCRIPT_COMMAND toggle_object_collision = { 0x0382, "ii" };
const SCRIPT_COMMAND play_sound = { 0x018c, "fffi" };
const SCRIPT_COMMAND remove_sound = { 0x018E, "i" };
const SCRIPT_COMMAND preload_beat_track = { 0x0952, "i" };
const SCRIPT_COMMAND start_playing_loaded_soundtrack = { 0x0954, "" }; //empty params
const SCRIPT_COMMAND end_playing_loaded_soundtrack = { 0x0955, "" }; //empty params
const SCRIPT_COMMAND create_sound = { 0x018d,	"fffiv" };
const SCRIPT_COMMAND remove_actor_from_car_and_put_at = { 0x0362, "ifff" };
const SCRIPT_COMMAND TASK_EVERYONE_LEAVE_CAR = { 0x068B, "i"};
const SCRIPT_COMMAND TASK_ENTER_CAR_AS_DRIVER = { 0x05CB, "iii" };
const SCRIPT_COMMAND send_actor_to_car_passenger = { 0x05CA, "iiii" };
const SCRIPT_COMMAND TASK_LEAVE_ANY_CAR = { 0x0633, "i" };
const SCRIPT_COMMAND TASK_LEAVE_CAR_IMMEDIATELY = { 0x0622, "ii" };
const SCRIPT_COMMAND get_car_z_angle = { 0x0174, "iv" };
const SCRIPT_COMMAND get_player_z_angle = { 0x0170, "iv" };
const SCRIPT_COMMAND create_train = { 0x06D8, "ifffiv" };
const SCRIPT_COMMAND destroy_train = { 0x07bd, "i" };
const SCRIPT_COMMAND set_train_acceleration = { 0x06DC, "if" };
const SCRIPT_COMMAND set_train_speed = { 0x06DD, "if" };
const SCRIPT_COMMAND is_train_wrecked = { 0x0981, "i" };
const SCRIPT_COMMAND actor_driving_train = { 0x09AE, "i" };
const SCRIPT_COMMAND enable_train_traffic = { 0x06d7, "i" };
const SCRIPT_COMMAND unknown_train_attr = { 0x0A07, "i" }; // vehicle discardable?
const SCRIPT_COMMAND car_gas_tank_explosion = { 0x09C4, "ii" };
const SCRIPT_COMMAND lock_car = { 0x0519, "ii" };
const SCRIPT_COMMAND put_train_at = { 0x07c7, "ifff" };
const SCRIPT_COMMAND turn_off_car_engine = { 0x02d4,	"i" };
const SCRIPT_COMMAND release_model = { 0x0249,	"i" };
const SCRIPT_COMMAND get_active_interior = { 0x077e, "v" };
const SCRIPT_COMMAND get_actor_z_angle = { 0x0172, "iv" };
const SCRIPT_COMMAND set_actor_z_angle = { 0x0173, "if" };
const SCRIPT_COMMAND actor_set_collision = { 0x0619, "ii" };
const SCRIPT_COMMAND actor_unknown1 = { 0x094f, "i" };
const SCRIPT_COMMAND toggle_player_ignored_by_all = { 0x03bf, "ii" };
const SCRIPT_COMMAND enter_passenger_driveby = { 0x0713, "iiiffffiii" };
const SCRIPT_COMMAND lock_actor = { 0x04d7, "ii" };
const SCRIPT_COMMAND task_set_ignore_weapon_range_flag = { 0x099f, "ii" };
const SCRIPT_COMMAND task_stand_still = { 0x05Ba, "ii" };
//05BA
const SCRIPT_COMMAND refresh_streaming_at = { 0x04E4, "ff" };
const SCRIPT_COMMAND put_actor_in_car2 = { 0x0430, "iii" };
const SCRIPT_COMMAND get_radio_channel = { 0x051E, "i" };

const SCRIPT_COMMAND kill_actor = { 0x0321, "i" };

//const SCRIPT_COMMAND kill_actor					= { 0x5BE, "i" };
const SCRIPT_COMMAND set_actor_animation_set = { 0x0245, "is" };
const SCRIPT_COMMAND request_animation = { 0x04ED, "s" };
const SCRIPT_COMMAND is_animation_loaded = { 0x04EE, "s" };
const SCRIPT_COMMAND release_animation = { 0x04EF, "s" };
//0812
const SCRIPT_COMMAND TASK_CHAR_ARREST_CHAR = { 0x0850, "ii" };
const SCRIPT_COMMAND apply_animation = { 0x0812, "issfiiiii" }; // actor,animation,library,floatunk,bool,bool,bool,bool,int
const SCRIPT_COMMAND is_actor_performing_anim = { 0x0611, "is" };
const SCRIPT_COMMAND set_actor_weapon_droppable = { 0x087e, "ii" };
const SCRIPT_COMMAND set_actor_money = { 0x03fe, "ii" };
const SCRIPT_COMMAND toggle_radar_blank = { 0x0581, "i" }; // адрес верный, не работает
const SCRIPT_COMMAND toggle_hud = { 0x0826, "b" }; // Адрес верный. не работает
const SCRIPT_COMMAND text_clear_all = { 0x00be, "" };
const SCRIPT_COMMAND link_actor_to_interior = { 0x0860, "ii" };
const SCRIPT_COMMAND repair_car = { 0x0A30, "i" };
const SCRIPT_COMMAND SET_PLAYER_ENTER_CAR_BUTTON = { 0x07CC, "ii" };
const SCRIPT_COMMAND set_car_hydraulics = { 0x07FF,	"ii" }; //iCar, i (0=off/1=on)

const SCRIPT_COMMAND attach_object_to_actor = { 0x069b, "iiffffff" };

const SCRIPT_COMMAND create_pickup = { 0x0213, "iifffv" };
const SCRIPT_COMMAND create_pickup_with_ammo = { 0x032b, "iiifffv" };
const SCRIPT_COMMAND is_pickup_picked_up = { 0x0214, "i" };
const SCRIPT_COMMAND is_pickup_created = { 0x09D1, "i" };
const SCRIPT_COMMAND destroy_pickup = { 0x0215, "i" };

const SCRIPT_COMMAND set_event = { 0x09C6, "iiii" };
const SCRIPT_COMMAND change_car_skin = { 0x06ED,	"ii" };
const SCRIPT_COMMAND add_car_component = { 0x06E7, "iiv" };	// CAR, COMPONENT, COMPONENT VAR NAME
const SCRIPT_COMMAND is_component_available = { 0x06EA, "i" };
const SCRIPT_COMMAND request_car_component = { 0x06E9, "i" };
const SCRIPT_COMMAND remove_component = { 0x06E8, "ii" };

const SCRIPT_COMMAND unknown_arrow = { 0x07e0, "ii" };
const SCRIPT_COMMAND show_on_radar2 = { 0x018b, "ii" };

const SCRIPT_COMMAND attach_object_to_car = { 0x0681, "iiffffff" };
const SCRIPT_COMMAND is_object_attached = { 0x0685, "i" };
const SCRIPT_COMMAND set_char_never_targeted = { 0x0568, "ii" };
const SCRIPT_COMMAND task_stay_in_same_place = { 0x0638, "ii" };
const SCRIPT_COMMAND toggle_actor_cellphone = { 0x0729, "ii" };
const SCRIPT_COMMAND clear_char_tasks = { 0x0792, "i" };
const SCRIPT_COMMAND actor_task_sit = { 0x06b0, "ii" };
const SCRIPT_COMMAND actor_task_handsup = { 0x5c4, "ii" };
const SCRIPT_COMMAND actor_task_use_atm = { 0x5c7, "ii" };
const SCRIPT_COMMAND change_stat = { 0x0629, "ii" };
const SCRIPT_COMMAND set_char_weapon_skill = { 0x081A, "ii" };

const SCRIPT_COMMAND toggle_car_tires_vulnerable = { 0x053f, "ii" };
const SCRIPT_COMMAND set_fighting_style = { 0x07fe, "iii" };

const SCRIPT_COMMAND link_vehicle_to_interior = { 0x0840, "ii" };
const SCRIPT_COMMAND delete_object_in_coords = { 0x0363, "ffffii" };
const SCRIPT_COMMAND swap_nearest_building_model = { 0x03B6, "ffffii" };
const SCRIPT_COMMAND destroy_object = { 0x0108, "i" };
const SCRIPT_COMMAND create_radar_marker_icon = { 0x0570, "fffii" };
const SCRIPT_COMMAND put_trailer_on_cab = { 0x893, "ii" };
const SCRIPT_COMMAND detach_trailer_from_cab = { 0x7AC, "ii" };
const SCRIPT_COMMAND is_trailer_on_cab = { 0x7AB, "ii" };
const SCRIPT_COMMAND create_explosion_with_radius = { 0x0948, "fffii" };
const SCRIPT_COMMAND set_car_numberplate = { 0x0674, "is" };
const SCRIPT_COMMAND enable_zone_names = { 0x09BA, "i" };

const SCRIPT_COMMAND destroy_particle = { 0x650, "i" };
//opcode_066a('PETROLCAN', lhActor47, 0.0, 0.116, 0.048, lf05f, lf060, lf061, 1, l0089);
const SCRIPT_COMMAND attach_particle_to_actor = { 0x0669, "sifffiv" };
const SCRIPT_COMMAND make_particle_visible = { 0x64c, "i" };

const SCRIPT_COMMAND create_property_pickup = { 0x518, "i" };

const SCRIPT_COMMAND SET_PLAYER_DRUNKENNESS = { 0x052C, "ii" }; // player, severity (0-255)
const SCRIPT_COMMAND handling_responsiveness = { 0x03fd, "ii" }; // player, severity (0-255)
const SCRIPT_COMMAND load_shop_menu = { 0x075E, "s" };

const SCRIPT_COMMAND FORCE_CAR_LIGHTS = { 0x067F, "ii" };

const SCRIPT_COMMAND BURST_CAR_TYRE = { 0x04FE, "ii" };
const SCRIPT_COMMAND FIX_CAR_TYRE = { 0x0699, "ii" };
const SCRIPT_COMMAND TASK_LOOK_AT_COORD = { 0x06A9, "ifffi" };

//opcode_066a('PETROLCAN', lhActor47, 0.0, 0.116, 0.048, lf05f, lf060, lf061, 1, l0089);
const SCRIPT_COMMAND attach_particle_to_actor2			= { 0x066a, "siffffffiv" };