#include "../meal_pager_i.h"
#include "../helpers/meal_pager_custom_event.h"
#include "../views/meal_pager_scanner.h"
//#include "../helpers/meal_pager_led.h"
//#include <dolphin/dolphin.h>
#define SUBGHZ_PROTOCOL_BIN_RAW_NAME "BinRAW"

static void meal_pager_scene_scanner_update_statusbar(void* context) {
    Meal_Pager* app = context;
    SubGhz* subghz = app->subghz;
    FuriString* history_stat_str = furi_string_alloc();
    if(!subghz_history_get_text_space_left(subghz->history, history_stat_str)) {
        FuriString* frequency_str = furi_string_alloc();
        FuriString* modulation_str = furi_string_alloc();

        subghz_txrx_get_frequency_and_modulation(subghz->txrx, frequency_str, modulation_str);

        //subghz_view_receiver_add_data_statusbar(
        //    subghz->subghz_receiver,
        //    furi_string_get_cstr(frequency_str),
        //    furi_string_get_cstr(modulation_str),
        //    furi_string_get_cstr(history_stat_str));

        furi_string_free(frequency_str);
        furi_string_free(modulation_str);
    } else {
        //subghz_view_receiver_add_data_statusbar(
        //    subghz->subghz_receiver, furi_string_get_cstr(history_stat_str), "", "");
        subghz->state_notifications = SubGhzNotificationStateIDLE;
    }
    furi_string_free(history_stat_str);

    //subghz_view_receiver_set_radio_device_type(
    //    subghz->subghz_receiver, subghz_txrx_radio_device_get(subghz->txrx));
}

void meal_pager_scene_scanner_callback(Meal_PagerCustomEvent event, void* context) {
    furi_assert(context);
    Meal_Pager* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
}

static void meal_pager_add_to_history_callback(
    SubGhzReceiver* receiver,
    SubGhzProtocolDecoderBase* decoder_base,
    void* context) {
    furi_assert(context);
    Meal_Pager* app = context;
    SubGhz* subghz = app->subghz;
    SubGhzHistory* history = subghz->history;
    FuriString* str_buff = furi_string_alloc();

    SubGhzRadioPreset preset = subghz_txrx_get_preset(subghz->txrx);

    if(subghz_history_add_to_history(history, decoder_base, &preset)) {
        furi_string_reset(str_buff);

        subghz->state_notifications = SubGhzNotificationStateRxDone;
        uint16_t item_history = subghz_history_get_item(history);
        subghz_history_get_text_item_menu(history, str_buff, item_history - 1);
        //subghz_view_receiver_add_item_to_menu(
        //    subghz->subghz_receiver,
        //    furi_string_get_cstr(str_buff),
        //    subghz_history_get_type_protocol(history, item_history - 1));

       meal_pager_scene_scanner_update_statusbar(subghz);
    }
    subghz_receiver_reset(receiver);
    //UNUSED(scanner);
    furi_string_free(str_buff);
    subghz_rx_key_state_set(subghz, SubGhzRxKeyStateAddKey);
}

void meal_pager_scene_scanner_on_enter(void* context) {
    furi_assert(context);
    Meal_Pager* app = context;
    SubGhz* subghz = app->subghz;
    SubGhzHistory* history = subghz->history;

    FuriString* str_buff;
    str_buff = furi_string_alloc();

    if(subghz_rx_key_state_get(subghz) == SubGhzRxKeyStateIDLE) {
        subghz_set_default_preset(subghz);
        subghz_history_reset(history);
        subghz_rx_key_state_set(subghz, SubGhzRxKeyStateStart);
    }
    furi_string_free(str_buff);

    meal_pager_scanner_set_callback(
        app->meal_pager_scanner, meal_pager_scene_scanner_callback, app);
    subghz_txrx_set_rx_calback(subghz->txrx, meal_pager_add_to_history_callback, subghz);

    subghz->state_notifications = SubGhzNotificationStateRx;
    subghz_txrx_rx_start(subghz->txrx);

    /*furi_check(
        subghz_txrx_load_decoder_by_name_protocol(subghz->txrx, SUBGHZ_PROTOCOL_BIN_RAW_NAME));*/

    meal_pager_scene_scanner_update_statusbar(app);

    view_dispatcher_switch_to_view(app->view_dispatcher, Meal_PagerViewIdScanner);
}

bool meal_pager_scene_scanner_on_event(void* context, SceneManagerEvent event) {
    Meal_Pager* app = context;
    SubGhz* subghz = app->subghz;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case Meal_PagerCustomEventScannerOk:
        case Meal_PagerCustomEventScannerBack:
            subghz->state_notifications = SubGhzNotificationStateIDLE;
            subghz_txrx_stop(subghz->txrx);
            subghz_txrx_hopper_set_state(subghz->txrx, SubGhzHopperStateOFF);
            subghz_txrx_set_rx_calback(subghz->txrx, NULL, subghz);
            subghz_rx_key_state_set(subghz, SubGhzRxKeyStateIDLE);
            subghz_set_default_preset(subghz);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneMenu);
            consumed = true;
            break;
        default:
            break;
        }
    }

    return consumed;
}

void meal_pager_scene_scanner_on_exit(void* context) {
    Meal_Pager* app = context;
    UNUSED(app);
}