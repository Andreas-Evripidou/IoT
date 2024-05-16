// This file contains the code to build the UI components of the application 

#include <lvgl.h>
#include <geolocation.h>
#include <TFT_eSPI.h>
#include <HTTPClient.h> // ESP32 library for making HTTP requests

static lv_style_t border_style;
static lv_style_t popupBox_style;
static lv_obj_t *timeLabel;
static lv_obj_t *settings;
static lv_obj_t *settingBtn;
static lv_obj_t *otaBtn;
static lv_obj_t *settingCloseBtn;
static lv_obj_t *settingWiFiSwitch;
static lv_obj_t *wfList;
static lv_obj_t *settinglabel;
static lv_obj_t *otaBox;
static lv_obj_t *otaLabel;
static lv_obj_t *otaCheckUpdateBtn;
static lv_obj_t *otaCloseBtn;
static lv_obj_t *btnSymbol;
static lv_obj_t *btnLabel;
static lv_obj_t *otaProgress;
static lv_obj_t *mboxConnect;
static lv_obj_t *mboxTitle;
static lv_obj_t *mboxPassword;
static lv_obj_t *mboxConnectBtn;
static lv_obj_t *mboxCloseBtn;
static lv_obj_t *keyboard;
static lv_obj_t *popupBox;
static lv_obj_t *popupBoxCloseBtn;
static lv_timer_t *timer;

static void btn_event_cb(lv_event_t *e);
static void list_event_handler(lv_event_t *e);
static void text_input_event_cb(lv_event_t *e);
static lv_obj_t *panicButton = NULL;
static lv_obj_t *panicLabel = NULL;
static lv_obj_t *bodyLabel = NULL;

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;

TFT_eSPI tft = TFT_eSPI(screenHeight, screenWidth); // the LCD screen
HTTPClient http; // the HTTP client

void showPanicButton() {
  lv_obj_clear_flag(panicButton, LV_OBJ_FLAG_HIDDEN);
  lv_obj_clear_flag(panicLabel, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(bodyLabel, LV_OBJ_FLAG_HIDDEN);
}

void hidePanicButton() {
  if (panicButton) {
    lv_obj_add_flag(panicButton, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(panicLabel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(bodyLabel, LV_OBJ_FLAG_HIDDEN);
  }
}

static void popupMsgBox(String title, String msg) {

  if (popupBox != NULL) {
    lv_obj_del(popupBox);
  }

  popupBox = lv_obj_create(lv_scr_act());
  lv_obj_add_style(popupBox, &popupBox_style, 0);
  lv_obj_set_size(popupBox, tft.width() * 2 / 3, tft.height() / 2);
  lv_obj_center(popupBox);

  lv_obj_t *popupTitle = lv_label_create(popupBox);
  lv_label_set_text(popupTitle, title.c_str());
  lv_obj_set_width(popupTitle, tft.width() * 2 / 3 - 50);
  lv_obj_align(popupTitle, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *popupMSG = lv_label_create(popupBox);
  lv_obj_set_width(popupMSG, tft.width() * 2 / 3 - 50);
  lv_label_set_text(popupMSG, msg.c_str());
  lv_obj_align(popupMSG, LV_ALIGN_TOP_LEFT, 0, 40);

  popupBoxCloseBtn = lv_btn_create(popupBox);
  lv_obj_add_event_cb(popupBoxCloseBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_align(popupBoxCloseBtn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_t *btnLabel = lv_label_create(popupBoxCloseBtn);
  lv_label_set_text(btnLabel, "Okay");
  lv_obj_center(btnLabel);
}

static void setStyle() {
  lv_style_init(&border_style);
  lv_style_set_border_width(&border_style, 2);
  lv_style_set_border_color(&border_style, lv_color_black());

  lv_style_init(&popupBox_style);
  lv_style_set_radius(&popupBox_style, 10);
  lv_style_set_bg_opa(&popupBox_style, LV_OPA_COVER);
  lv_style_set_border_color(&popupBox_style, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_border_width(&popupBox_style, 5);
}

static void buildStatusBar() {

  static lv_style_t style_btn;
  lv_style_init(&style_btn);
  lv_style_set_bg_color(&style_btn, lv_color_hex(0xC5C5C5));
  lv_style_set_bg_opa(&style_btn, LV_OPA_50);

  lv_obj_t *statusBar = lv_obj_create(lv_scr_act());
  lv_obj_set_size(statusBar, tft.width() - 20, 68);
  lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, 0);

  // lv_obj_remove_style(statusBar, NULL, LV_PART_SCROLLBAR | LV_STATE_ANY);
  // create a container for the time label
  lv_obj_t *timeLabelContainer = lv_obj_create(statusBar);
  lv_obj_set_size(timeLabelContainer, tft.width() - 60, 32);
  lv_obj_align(timeLabelContainer, LV_ALIGN_LEFT_MID, 0, 0);
  
  timeLabel = lv_label_create(timeLabelContainer);
  // lv_obj_set_size(timeLabel, tft.width() - 60, 30);

  lv_label_set_text(timeLabel, "WiFi Not Connected! " LV_SYMBOL_CLOSE);
  lv_obj_align(timeLabel, LV_ALIGN_LEFT_MID, 0, 0);

  settingBtn = lv_btn_create(statusBar);
  lv_obj_set_size(settingBtn, 30, 30);
  lv_obj_align(settingBtn, LV_ALIGN_RIGHT_MID, 0, 0);

  lv_obj_add_event_cb(settingBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_t *label = lv_label_create(settingBtn); /*Add a label to the button*/
  lv_label_set_text(label, LV_SYMBOL_SETTINGS);  /*Set the labels text*/
  lv_obj_center(label);

  otaBtn = lv_btn_create(statusBar);
  lv_obj_set_size(otaBtn, 30, 30);
  lv_obj_align(otaBtn, LV_ALIGN_RIGHT_MID, -40, 0);
  lv_obj_add_event_cb(otaBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_t *label2 = lv_label_create(otaBtn); /*Add a label to the button*/
  lv_label_set_text(label2, LV_SYMBOL_DOWNLOAD);  /*Set the labels text*/
  lv_obj_center(label2);
}


void panicButtonEventCallback(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED) {
        // Make the API call when the panic button is clicked
        lv_label_set_text(panicLabel, "Getting Location...");
        lv_refr_now(NULL);

        int wlResult = getGeoLocation();

        if (wlResult == WL_OK) {
            // Send the SOS message
            lv_label_set_text(panicLabel, "Sending SOS...");
            lv_refr_now(NULL);
            
            // Send the SOS message to the server
            http.begin("http://" FIRMWARE_SERVER_IP_ADDR ":" FIRMWARE_SERVER_PORT "/sos");
            http.addHeader("Content-Type", "application/json");
            String payload = "{\"lat\": " + String(loc.lat, 7) + ", \"lon\": " + String(loc.lon, 7) + "}";
            int httpCode = http.POST(payload);
            http.end();
            
            popupMsgBox("Success", "SOS Sent!");
        } else {
            // Show the error message
            popupMsgBox("Error", "Could not get location");
        }

        lv_label_set_text(panicLabel, "Panic!");
    
    }
}

static void buildPWMsgBox() {

  mboxConnect = lv_obj_create(lv_scr_act());
  lv_obj_add_style(mboxConnect, &border_style, 0);
  lv_obj_set_size(mboxConnect, tft.width() * 2 / 3, tft.height() / 2);
  lv_obj_center(mboxConnect);

  mboxTitle = lv_label_create(mboxConnect);
  lv_label_set_text(mboxTitle, "Selected WiFi SSID: ThatProject");
  lv_obj_align(mboxTitle, LV_ALIGN_TOP_LEFT, 0, 0);

  mboxPassword = lv_textarea_create(mboxConnect);
  lv_obj_set_size(mboxPassword, tft.width() / 2, 40);
  lv_obj_align_to(mboxPassword, mboxTitle, LV_ALIGN_TOP_LEFT, 0, 30);
  lv_textarea_set_placeholder_text(mboxPassword, "Password?");
  lv_obj_add_event_cb(mboxPassword, text_input_event_cb, LV_EVENT_ALL, keyboard);

  mboxConnectBtn = lv_btn_create(mboxConnect);
  lv_obj_add_event_cb(mboxConnectBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_align(mboxConnectBtn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  lv_obj_t *btnLabel = lv_label_create(mboxConnectBtn);
  lv_label_set_text(btnLabel, "Connect");
  lv_obj_center(btnLabel);

  mboxCloseBtn = lv_btn_create(mboxConnect);
  lv_obj_add_event_cb(mboxCloseBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_align(mboxCloseBtn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_t *btnLabel2 = lv_label_create(mboxCloseBtn);
  lv_label_set_text(btnLabel2, "Cancel");
  lv_obj_center(btnLabel2);
}

static void buildBody() {
  lv_obj_t *bodyScreen = lv_obj_create(lv_scr_act());
  lv_obj_add_style(bodyScreen, &border_style, 0);
  lv_obj_set_size(bodyScreen, tft.width() - 20, tft.height() - 65);
  lv_obj_align(bodyScreen, LV_ALIGN_BOTTOM_MID, 0, 0);

  bodyLabel = lv_label_create(bodyScreen);
  lv_label_set_text(bodyLabel, "Please Connect To The Internet");
  lv_obj_center(bodyLabel);

  panicButton = lv_btn_create(bodyScreen);
  lv_obj_set_size(panicButton, 220, 220);
  lv_obj_align(panicButton, LV_ALIGN_CENTER, 0, 0);
  // lv_label_set_text(panicButton, "Panic Button")

  // Set button style to make it round and red
  static lv_style_t panicButtonStyle;
  lv_style_init(&panicButtonStyle);
  lv_style_set_radius(&panicButtonStyle, LV_RADIUS_CIRCLE); // Make it round
  lv_style_set_bg_color(&panicButtonStyle, lv_color_make(255, 0, 0)); // Make it red
  lv_obj_add_style(panicButton, &panicButtonStyle, 0);

  panicLabel = lv_label_create(panicButton);
  lv_label_set_text(panicLabel, "Panic!");
  lv_obj_center(panicLabel);

  lv_obj_add_event_cb(panicButton, panicButtonEventCallback, LV_EVENT_CLICKED, NULL);

  hidePanicButton();
}

static void buildSettings() {
  settings = lv_obj_create(lv_scr_act());
  lv_obj_add_style(settings, &border_style, 0);
  lv_obj_set_size(settings, tft.width() - 100, tft.height() - 40);
  lv_obj_align(settings, LV_ALIGN_TOP_RIGHT, -20, 20);

  settinglabel = lv_label_create(settings);
  lv_label_set_text(settinglabel, "Settings " LV_SYMBOL_SETTINGS);
  lv_obj_align(settinglabel, LV_ALIGN_TOP_LEFT, 0, 0);

  settingCloseBtn = lv_btn_create(settings);
  lv_obj_set_size(settingCloseBtn, 30, 30);
  lv_obj_align(settingCloseBtn, LV_ALIGN_TOP_RIGHT, 0, -10);
  lv_obj_add_event_cb(settingCloseBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_t *btnSymbol = lv_label_create(settingCloseBtn);
  lv_label_set_text(btnSymbol, LV_SYMBOL_CLOSE);
  lv_obj_center(btnSymbol);

  settingWiFiSwitch = lv_switch_create(settings);
  lv_obj_add_event_cb(settingWiFiSwitch, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_align_to(settingWiFiSwitch, settinglabel, LV_ALIGN_TOP_RIGHT, 60, -10);
  lv_obj_add_flag(settings, LV_OBJ_FLAG_HIDDEN);

  wfList = lv_list_create(settings);
  lv_obj_set_size(wfList, tft.width() - 140, 210);
  lv_obj_align_to(wfList, settinglabel, LV_ALIGN_TOP_LEFT, 0, 30);
}

static void buildOTA() {
  otaBox = lv_obj_create(lv_scr_act());
  lv_obj_add_style(otaBox, &border_style, 0);
  lv_obj_set_size(otaBox, tft.width() - 100, tft.height() - 40);
  lv_obj_align(otaBox, LV_ALIGN_TOP_RIGHT, -20, 20);

  otaLabel =  lv_label_create(otaBox);
  lv_label_set_text(otaLabel, "No Internet" LV_SYMBOL_DOWNLOAD);
  lv_obj_align(otaLabel, LV_ALIGN_TOP_LEFT, 0, 0);
  

  otaCloseBtn = lv_btn_create(otaBox);
  lv_obj_set_size(otaCloseBtn, 30, 30);
  lv_obj_align(otaCloseBtn, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_add_event_cb(otaCloseBtn, btn_event_cb, LV_EVENT_ALL, NULL);
  btnSymbol = lv_label_create(otaCloseBtn);
  lv_label_set_text(btnSymbol, LV_SYMBOL_CLOSE);
  lv_obj_center(btnSymbol);

  otaCheckUpdateBtn = lv_btn_create(otaBox);
  lv_obj_set_size(otaCheckUpdateBtn, 150, 50);
  lv_obj_align(otaCheckUpdateBtn, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(otaCheckUpdateBtn, btn_event_cb, LV_EVENT_ALL, NULL);

  btnLabel = lv_label_create(otaCheckUpdateBtn);
  lv_label_set_text(btnLabel, "Check For Update");
  lv_obj_center(btnLabel);

  otaProgress = lv_bar_create(otaBox);
  lv_obj_set_size(otaProgress, 200, 20);
  lv_obj_align(otaProgress, LV_ALIGN_CENTER, 0, 0);
  lv_bar_set_value(otaProgress, 0, LV_ANIM_ON);

  lv_obj_add_flag(otaBox, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(otaProgress, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(otaCheckUpdateBtn, LV_OBJ_FLAG_HIDDEN);
}

static void text_input_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *ta = lv_event_get_target(e);

  if (code == LV_EVENT_FOCUSED) {
    lv_obj_move_foreground(keyboard);
    lv_keyboard_set_textarea(keyboard, ta);
    lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
  }

  if (code == LV_EVENT_DEFOCUSED) {
    lv_keyboard_set_textarea(keyboard, NULL);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
  }
}

static void makeKeyboard() {
  keyboard = lv_keyboard_create(lv_scr_act());
  lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}

static void initUI() {  

  setStyle();
  makeKeyboard();
  buildStatusBar();
  buildPWMsgBox();
  buildBody();
  buildSettings();
  buildOTA();

}