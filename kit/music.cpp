#include "music.h"

#define MUSIC_LIMIT 30

int mute_flag = 0;

String music_list[MUSIC_LIMIT];
int music_num = 0;
int music_index = 0;

void music_init()
{
    // Read SD
    music_num = get_music_list(SD, "/", music_list, MUSIC_LIMIT);
    Serial.print("Music file count:");
    Serial.println(music_num);
    Serial.println("All music:");
    for (int i = 0; i < music_num; i++)
    {
        Serial.println(music_list[i]);
    }

    // Init Audio
    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(21);
    // audio.connecttoFS(SD, "/ChildhoodMemory.mp3");
    open_new_song(music_list[music_index]);

    xTaskCreatePinnedToCore(Task_Audio, "Task_Audio", 10240, NULL, 2, NULL, 1);
}

void music_mute()
{
    if (mute_flag == 0)
    {
        mute_flag = 1;
        audio.setVolume(0);
    }
    else
    {
        mute_flag = 0;
        audio.setVolume(21);
    }
}

void Task_Audio(void *pvParameters)
{
    while (1)
    {
        audio.loop();
        vTaskDelay(1);
        // Serial.println(F("Task_Audio"));
    }
}

int get_music_list(fs::FS &fs, const char *dirname, String *list, int length)
{
    Serial.printf("Listing directory: %s\n", dirname);
    int i = 0;

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return i;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return i;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
        }
        else
        {
            String temp = file.name();
            if (temp.endsWith(".wav"))
            {
                list[i] = temp;
                i++;
            }
            else if (temp.endsWith(".mp3"))
            {
                list[i] = temp;
                i++;
            }
            if (i >= MUSIC_LIMIT)
                return i;
        }
        file = root.openNextFile();
    }
    return i;
}

// 开始新歌
void open_new_song(String filename)
{
    audio.pauseResume();
    vTaskDelay(500);
    audio.connecttoFS(SD, filename.c_str());
    Serial.println("**********start a new sound************");
}

// 歌曲结束逻辑
void audio_eof_mp3(const char *info)
{ // end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);
    music_index++;
    if (music_index >= music_num)
    {
        music_index = 0;
    }
    open_new_song(music_list[music_index]);
}

// 切歌
void next_song()
{
    if (music_index < music_num - 1)
        music_index++;
    else
        music_index = 0;
    open_new_song(music_list[music_index]);
    if (mute_flag == 1)
    {
        {
            mute_flag = 0;
            audio.setVolume(21);
        }
    }
}