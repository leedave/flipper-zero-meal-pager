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
    meal_pager_scanner_set_callback(
        app->meal_pager_scanner, meal_pager_scene_scanner_callback, app);
    view_dispatcher_switch_to_view(app->view_dispatcher, Meal_PagerViewIdScanner);
}

bool meal_pager_scene_scanner_on_event(void* context, SceneManagerEvent event) {
    Meal_Pager* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case Meal_PagerCustomEventScannerOk:
        case Meal_PagerCustomEventScannerBack:
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