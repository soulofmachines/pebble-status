#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_epoch_layer;
static TextLayer *s_date_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_weather_layer;

static int s_battery_level;

//static BitmapLayer *s_background_layer;
//static GBitmap *s_background_bitmap;

static BitmapLayer *s_bt_icon_layer;
static GBitmap *s_bt_icon_bitmap;

static void battery_callback(BatteryChargeState state) {
    s_battery_level = state.charge_percent;
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[8];
  static char s_battery_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

    static char s_epoch_buffer[16];
    static char s_date_buffer[16];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%Y/%m/%d %a", tick_time);

    snprintf(s_epoch_buffer,sizeof(s_epoch_buffer), "%d", (int) temp);
    snprintf(s_battery_buffer,sizeof(s_battery_buffer), "%d%%", s_battery_level);
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_epoch_layer, s_epoch_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
  text_layer_set_text(s_battery_layer, s_battery_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
update_time();
    if(tick_time->tm_min % 30 == 0) {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}
}

static void bluetooth_callback(bool connected) {

     layer_set_hidden(bitmap_layer_get_layer(s_bt_icon_layer), !connected);

  if(!connected) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
    
}

static void main_window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
    
    // Create GBitmap
//s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BG);

// Create BitmapLayer to display the GBitmap
//s_background_layer = bitmap_layer_create(bounds);

// Set the bitmap onto the layer and add to the window
//bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
//layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, 63, bounds.size.w, 42));
    s_epoch_layer = text_layer_create(
    GRect(0, 105, bounds.size.w, 18));

    s_date_layer = text_layer_create(
    GRect(0, 123, bounds.size.w, 18));

    s_battery_layer = text_layer_create(
    GRect(0, 0, bounds.size.w, 18));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  //text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

    
  text_layer_set_background_color(s_epoch_layer, GColorClear);
  text_layer_set_text_color(s_epoch_layer, GColorBlack);
  //text_layer_set_text(s_epoch_layer, "00:00");
  text_layer_set_font(s_epoch_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_epoch_layer, GTextAlignmentCenter);
    
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  //text_layer_set_text(s_epoch_layer, "00:00");
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, GColorBlack);
  //text_layer_set_text(s_epoch_layer, "00:00");
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
    
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_epoch_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
    
    s_bt_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BT_ICON);

// Create the BitmapLayer to display the GBitmap
s_bt_icon_layer = bitmap_layer_create(GRect(0, 0, 8, 17));
    bitmap_layer_set_background_color(s_bt_icon_layer, GColorWhite);
bitmap_layer_set_bitmap(s_bt_icon_layer, s_bt_icon_bitmap);
layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bt_icon_layer));
    
    s_weather_layer = text_layer_create(
    GRect(0, 20, bounds.size.w, 25));

// Style the text
text_layer_set_background_color(s_weather_layer, GColorClear);
text_layer_set_text_color(s_weather_layer, GColorBlack);
text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
    text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
text_layer_set_text(s_weather_layer, "Loading...");
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
    
}

static void main_window_unload(Window *window) {
    text_layer_destroy(s_time_layer);
    text_layer_destroy(s_epoch_layer);
    text_layer_destroy(s_date_layer);
    text_layer_destroy(s_battery_layer);
    //gbitmap_destroy(s_background_bitmap);

// Destroy BitmapLayer
//bitmap_layer_destroy(s_background_layer);
    
    gbitmap_destroy(s_bt_icon_bitmap);
bitmap_layer_destroy(s_bt_icon_layer);
    text_layer_destroy(s_weather_layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Unload");
}


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
    //static char temperature_buffer[8];
//static char conditions_buffer[32];
//static char weather_layer_buffer[32];
    static char s_weather_buffer[32];
    // Read tuples for data
//Tuple *temp_tuple = dict_find(iterator, MESSAGE_KEY_TEMPERATURE);
//Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);
Tuple *weather_tuple = dict_find(iterator, MESSAGE_KEY_WEATHER);
    
    snprintf(s_weather_buffer, sizeof(s_weather_buffer), "%s", weather_tuple->value->cstring);
    text_layer_set_text(s_weather_layer, s_weather_buffer);
    
// If all data is available, use it
//if(temp_tuple && conditions_tuple) {
//  snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)temp_tuple->value->int32);
//  snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
//}
//snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "%s, %s", temperature_buffer, conditions_buffer);
//text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
    s_main_window = window_create();
    
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload
    });
    
    connection_service_subscribe((ConnectionHandlers) {
  .pebble_app_connection_handler = bluetooth_callback
});
    
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    window_stack_push(s_main_window, true);
    
    battery_state_service_subscribe(battery_callback);
    battery_callback(battery_state_service_peek());
    
    bluetooth_callback(connection_service_peek_pebble_app_connection());
    
    update_time();
 
    app_message_register_inbox_received(inbox_received_callback);
    const int inbox_size = 128;
const int outbox_size = 128;
app_message_open(inbox_size, outbox_size);
app_message_register_inbox_dropped(inbox_dropped_callback);
app_message_register_outbox_failed(outbox_failed_callback);
app_message_register_outbox_sent(outbox_sent_callback);
    
}

static void deinit() {
    window_destroy(s_main_window);
  APP_LOG(APP_LOG_LEVEL_INFO, "Destroyed");
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}