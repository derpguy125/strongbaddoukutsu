#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "Config.h"
#include "File.h"
#include "Tags.h"

static const char* const config_filename = "Config.dat";	// Not the original name
static const char* const config_magic = "CSE2E   20191225";	// Not the original name

BOOL LoadConfigData(CONFIG *conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(CONFIG));

	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s/%s", gModulePath, config_filename);

	// Open file
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;

	// Read the version id and font name
	fread(conf->proof, sizeof(conf->proof), 1, fp);
	fread(conf->font_name, sizeof(conf->font_name), 1, fp);

	// Read control settings
	conf->move_button_mode = fgetc(fp);
	conf->attack_button_mode = fgetc(fp);
	conf->ok_button_mode = fgetc(fp);

	// Read display mode (320x240, 640x480, 24-bit fullscreen, 32-bit fullscreen) TODO: add more things?
	conf->display_mode = fgetc(fp);

	// Read joystick configuration (if enabled, and mappings)
	conf->bJoystick = fgetc(fp);
	for (int button = 0; button < 8; button++)
		conf->joystick_button[button] = fgetc(fp);

	// Read framerate toggle
	conf->b60fps = fgetc(fp);

	// Read vsync toggle
	conf->bVsync = fgetc(fp);

	// Close file
	fclose(fp);

	// Check if version is not correct, and return if it failed
	if (strcmp(conf->proof, config_magic))
	{
		memset(conf, 0, sizeof(CONFIG));
		return FALSE;
	}

	return TRUE;
}

BOOL SaveConfigData(const CONFIG *conf)
{
	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s/%s", gModulePath, config_filename);

	// Open file
	FILE *fp = fopen(path, "wb");
	if (fp == NULL)
		return FALSE;

	// Read the version id and font name
	fwrite(conf->proof, sizeof(conf->proof), 1, fp);
	fwrite(conf->font_name, sizeof(conf->font_name), 1, fp);

	// Read control settings
	fputc(conf->move_button_mode, fp);
	fputc(conf->attack_button_mode, fp);
	fputc(conf->ok_button_mode, fp);

	// Read display mode (320x240, 640x480, 24-bit fullscreen, 32-bit fullscreen) TODO: add more things?
	fputc(conf->display_mode, fp);

	// Read joystick configuration (if enabled, and mappings)
	fputc(conf->bJoystick, fp);
	for (int button = 0; button < 8; button++)
		fputc(conf->joystick_button[button], fp);

	// Read framerate toggle
	fputc(conf->b60fps, fp);

	// Read vsync toggle
	fputc(conf->bVsync, fp);

	// Close file
	fclose(fp);

	return TRUE;
}

void DefaultConfigData(CONFIG *conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(CONFIG));

	strcpy(conf->proof, config_magic);

	// Fun fact: The Linux port added this line:
	// conf->display_mode = 1;

	// Reset joystick settings (as these can't simply be set to 0)
	conf->bJoystick = TRUE;
	conf->joystick_button[0] = 2;
	conf->joystick_button[1] = 1;
	conf->joystick_button[2] = 5;
	conf->joystick_button[3] = 6;
	conf->joystick_button[4] = 3;
	conf->joystick_button[5] = 4;
	conf->joystick_button[6] = 6;
	conf->joystick_button[7] = 3;
}
