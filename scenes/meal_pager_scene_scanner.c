#include "../meal_pager_i.h"
#include "../helpers/meal_pager_custom_event.h"
#include "../views/meal_pager_scanner.h"
//#include "../helpers/meal_pager_led.h"
//#include <dolphin/dolphin.h>

void meal_pager_scene_scanner_callback(Meal_PagerCustomEvent event, void* context) {
    furi_assert(context);
    Meal_Pager* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, event);
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

    meal_pager_scanner_set_callback(
        app->meal_pager_scanner, meal_pager_scene_scanner_callback, app);

    furi_string_free(str_buff);

    subghz->state_notifications = SubGhzNotificationStateRx;
    subghz_txrx_rx_start(subghz->txrx);

    /*furi_check(
        subghz_txrx_load_decoder_by_name_protocol(subghz->txrx, SUBGHZ_PROTOCOL_BIN_RAW_NAME));*/


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