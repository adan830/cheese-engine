/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "sound_manager.h"
#include "engine_math.h"
#include "file_io.h"
#include "log.h"
#include "options.h"
#include "engine_data.h"
#include "music_manager.h"

#include <boost/algorithm/string.hpp>

using namespace std;

double Sound_Manager::listener_x=0.0;
double Sound_Manager::listener_y=0.0;
double Sound_Manager::listener_zoom=1.0;

void Sound_Manager::add_sound(const Custom_Sound& sound){
    sounds.push_back(Sound_Data());

    sounds.back().create_custom_sound(sound);

    sound_names.push_back(sound.name);
}

void Sound_Manager::add_sound(string name,Mix_Chunk* chunk){
    sounds.push_back(Sound_Data());

    sounds.back().load_sound(chunk);

    sound_names.push_back(name);
}

void Sound_Manager::remove_sound(string name){
    for(int i=0;i<sound_names.size();i++){
        if(sound_names[i]==name){
            sound_names.erase(sound_names.begin()+i);

            sounds[i].unload_sound();
            sounds.erase(sounds.begin()+i);

            break;
        }
    }
}

void Sound_Manager::load_sounds(){
    //Look through all of the files in the directory
    for(File_IO_Directory_Iterator it("data/sounds");it.evaluate();it.iterate()){
        //If the file is not a directory
        if(it.is_regular_file()){
            string file_name=it.get_file_name();

            boost::algorithm::trim(file_name);

            sound_names.push_back(file_name);
        }
    }

    for(int i=0;i<sound_names.size();i++){
        sounds.push_back(Sound_Data());

        sounds.back().load_sound("data/sounds/"+sound_names[i]);

        sound_names[i].erase(sound_names[i].end()-4,sound_names[i].end());
    }
}

void Sound_Manager::unload_sounds(){
    for(int i=0;i<sounds.size();i++){
        sounds[i].unload_sound();
    }

    sounds.clear();
    sound_names.clear();
}

void Sound_Manager::set_listener(double x,double y,double camera_zoom){
    listener_x=x;
    listener_y=y;
    listener_zoom=camera_zoom;
}

int Sound_Manager::modify_sound_volume(int volume){
    //If either the global mute or the sound effects mute is enabled
    if(Options::mute_global || Options::mute_sound){
        return 0;
    }

    double new_volume=volume;

    new_volume*=Options::volume_sound;

    new_volume*=Options::volume_global;

    return (int)new_volume;
}

int Sound_Manager::get_free_channel(){
    int allocated_channels=Mix_AllocateChannels(-1);

    for(int i=0;i<allocated_channels;i++){
        //Check through all music tracks currently loaded
        bool channel_taken_by_music_track=false;

        for(size_t n=0;n<Music_Manager::tracks.size();n++){
            //If this channel is equal to this track's channel, then this channel is taken
            if(i==Music_Manager::tracks[n].channel){
                channel_taken_by_music_track=true;

				break;
            }
        }

        //If the channel is already playing
        if(Mix_Playing(i)==1){
        }
        //If this channel is already reserved by a music track
        else if(channel_taken_by_music_track){
        }
        //All tests were passed, and this channel is currently free, so return it
        else{
            return i;
        }
    }

    return -1;
}

void Sound_Manager::_play_sound(Sound_Data* ptr_sound,int volume){
    if(ptr_sound!=0 && ptr_sound->chunk!=0){
        if(volume!=0){
            int channel=get_free_channel();

            if(channel!=-1){
                Mix_Volume(channel,volume);
                Mix_PlayChannel(channel,ptr_sound->chunk,0);
            }
        }
    }
}

void Sound_Manager::play_sound(string sound_name){
    Sound_Data* ptr_sound=get_sound(sound_name);

    int volume=128;

    volume=modify_sound_volume(volume);

    _play_sound(ptr_sound,volume);
}

void Sound_Manager::play_sound(string sound_name,double sound_x,double sound_y,double falloff){
    Sound_Data* ptr_sound=get_sound(sound_name);

    int volume=128;

    if(falloff<0.0){
        falloff=Engine_Data::sound_falloff;
    }

    falloff*=listener_zoom;

    double sound_center_x=listener_x;
    double sound_center_y=listener_y;

    double sound_position_x=sound_x*listener_zoom;
    double sound_position_y=sound_y*listener_zoom;

    double sound_distance=Math::distance_between_points(sound_center_x,sound_center_y,sound_position_x,sound_position_y);
    if(sound_distance<1.0){
        sound_distance=0.0;
    }

    if(sound_distance==0.0){
        volume=128;
    }
    else{
        volume=(int)floor((falloff/sound_distance)*falloff);
    }

    if(volume>128){
        volume=128;
    }

    volume=modify_sound_volume(volume);

    _play_sound(ptr_sound,volume);
}

void Sound_Manager::stop_sounds(){
    int allocated_channels=Mix_AllocateChannels(-1);

    for(int i=0;i<allocated_channels;i++){
        //Check through all music tracks currently loaded
        bool channel_taken_by_music_track=false;

        for(size_t n=0;n<Music_Manager::tracks.size();n++){
            //If this channel is equal to this track's channel, then this channel is taken
            if(i==Music_Manager::tracks[n].channel){
                channel_taken_by_music_track=true;

                break;
            }
        }

        if(!channel_taken_by_music_track){
            Mix_HaltChannel(i);
        }
    }
}

Sound_Data* Sound_Manager::get_sound(string name){
    Sound_Data* ptr_object=0;

    for(size_t i=0;i<sound_names.size();i++){
        if(sound_names[i]==name){
            ptr_object=&sounds[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing sound '"+name+"'");
    }

    return ptr_object;
}