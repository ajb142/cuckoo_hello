#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <time.h>
#include <stdint.h>
#include "linux/input.h"

enum screen_color
{
   SCREEN_COLOR_BLACK = 0x000000,
   SCREEN_COLOR_WHITE = 0xFFFFFF,
   SCREEN_COLOR_RED   = 0xFF0000,
   SCREEN_COLOR_GREEN = 0x00FF00,
   SCREEN_COLOR_BLUE  = 0x0000FF
};

// Function declarations
void print_event_data(const char *event_name, const struct input_event *event_data);
void poll_input_events(int event1, int event2);
void set_screen_color(int screen_buffer, enum screen_color color);

int main() {
   printf("Cuckoo Hello\n");

   
   int event1 = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
   int event2 = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
   if (event1 < 0 || event2 < 0)
   {
      perror("Failed to open input devices");
      return 1;
   }

   int screen_buffer = open("/dev/fb0", O_RDWR);
   if (screen_buffer < 0)
   {
      perror("Failed to open screen buffer");
      return 1;
   }

   // Array of colors to cycle through
   enum screen_color colors[] = {
      SCREEN_COLOR_RED,
      SCREEN_COLOR_GREEN,
      SCREEN_COLOR_BLUE,
      SCREEN_COLOR_WHITE,
      SCREEN_COLOR_BLACK
   };
   int color_count = sizeof(colors) / sizeof(colors[0]);
   int current_color_index = 0;
   
   time_t last_color_change = time(NULL);
   
   while (1) 
   {
      poll_input_events(event1, event2);
      
      // Check if 1 second has passed
      time_t current_time = time(NULL);
      if (current_time - last_color_change >= 1) {
         set_screen_color(screen_buffer, colors[current_color_index]);
         printf("Changed screen to color: 0x%06X\n", colors[current_color_index]);
         
         current_color_index = (current_color_index + 1) % color_count;
         last_color_change = current_time;
      }
      
      usleep(10 * 1000); // Sleep for 10 milliseconds
   }

   return 0;
}

void poll_input_events(int event1, int event2)
{
   struct input_event event1_data;
   struct input_event event2_data;
   memset(&event1_data, 0, sizeof(event1_data));
   memset(&event2_data, 0, sizeof(event2_data));

   int bytesRead1 = read(event1, &event1_data, sizeof(event1_data));
   int bytesRead2 = read(event2, &event2_data, sizeof(event2_data));

   if (bytesRead1 > 0)
   {
      print_event_data("event1", &event1_data);
   }

   if (bytesRead2 > 0)
   {
      print_event_data("event2", &event2_data);
   }
}

void print_event_data(const char* event_name, const struct input_event* event_data) {
   printf("%s: time_sec=%lu, time_usec=%lu, type=%hu, code=%hu, value=%d | \n",
          event_name,
          event_data->time.tv_sec,
          event_data->time.tv_usec,
          event_data->type,
          event_data->code,
          event_data->value);
}

void set_screen_color(int screen_buffer, enum screen_color color) {
   struct fb_var_screeninfo vinfo;
   struct fb_fix_screeninfo finfo;
   
   // Get screen info
   if (ioctl(screen_buffer, FBIOGET_VSCREENINFO, &vinfo) == -1) {
      perror("Error reading variable screen info");
      return;
   }
   
   if (ioctl(screen_buffer, FBIOGET_FSCREENINFO, &finfo) == -1) {
      perror("Error reading fixed screen info");
      return;
   }
   
   // Calculate screen buffer size
   long screensize = vinfo.yres_virtual * finfo.line_length;
   
   // Map the screen buffer to memory
   char *fbp = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, screen_buffer, 0);
   if (fbp == MAP_FAILED) {
      perror("Error mapping framebuffer to memory");
      return;
   }
   
   // Set all pixels to the specified color
   // Assuming 32-bit color depth (4 bytes per pixel)
   uint32_t *pixel = (uint32_t*)fbp;
   long pixel_count = screensize / 4;
   long i;
   
   for (i = 0; i < pixel_count; i++) {
      pixel[i] = color;
   }
   
   // Unmap the memory
   munmap(fbp, screensize);
}
