/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_manager_h
#define game_manager_h

#include "rng.h"
#include "collision.h"
#include "progress_bar.h"
#include "file_io.h"

#include <string>
#include <vector>

class Game_Manager{
public:

    static bool display_scoreboard;
    //If true, a game is currently in progress
    //If false, a game is not in progress
    static bool in_progress;
    //If true, the game is paused
    //If false, the game is not paused
    static bool paused;

    //Current movement state of the camera
    static std::string cam_state;
    static double camera_delta_x;
    static double camera_delta_y;
    static double camera_speed;
    static double camera_zoom;

    static std::string current_music;

    //Holds a string representing each command that is currently in the on state, and is relevant for networking
    //This is updated each frame
    static std::vector<std::string> command_states;

    static Collision_Rect<double> camera;

    static void reset();
    static void reset_camera_dimensions();

    static std::string get_random_direction_cardinal(RNG& rng);
    static std::string get_random_direction_cardinal_ordinal(RNG& rng);

    //Returns true if the number of effects does not exceed the effect limit
    static bool effect_allowed();

    static void manage_music();

    static void toggle_pause();

    static void start();
    static void start_server_lockstep();
    static void start_client();
    static void stop();

    static void center_camera(Collision_Rect<double> box);
    static void center_camera(Collision_Circ<double> circle);

    static void zoom_camera_in(Collision_Rect<double> box);
    static void zoom_camera_in(Collision_Circ<double> circle);

    static void zoom_camera_out(Collision_Rect<double> box);
    static void zoom_camera_out(Collision_Circ<double> circle);

    static void prepare_for_input();

    static void handle_command_states_multiplayer();
    static void handle_game_commands_multiplayer();

    static void handle_input_states_gui();
    static void handle_input_states();
    static bool handle_game_command_gui(std::string command_name);
    static bool handle_game_command(std::string command_name);
    static bool handle_input_events_gui();
    static bool handle_input_events();

    static void set_camera();

    static void handle_drag_and_drop(std::string file);

    static std::string get_game_window_caption();

    static void clear_title();
    static void setup_title();
    static void update_title_background();

    static void render_scoreboard();
    static void render_title_background();
    static void render_pause();
    static void render_fps(int render_rate,double ms_per_frame,int logic_frame_rate);
    static void render_loading_screen(const Progress_Bar& bar,std::string message);

    static void load_data_game(Progress_Bar& bar);
    static void load_data_tag_game(std::string tag,File_IO_Load* load);
    static void unload_data_game();
};

#endif
