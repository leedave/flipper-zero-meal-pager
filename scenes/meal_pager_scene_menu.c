#include "../meal_pager_i.h"
#include "../helpers/meal_pager_led.h"
#include "../helpers/meal_pager_storage.h"
#include "../views/meal_pager_transmit.h"

enum SubmenuIndex {
    SubmenuIndexTransmit = 10,
    SubmenuIndexSetFirstStation,
    SubmenuIndexSetLastStation,
    SubmenuIndexSetFirstPager,
    SubmenuIndexSetLastPager,
    SubmenuIndexScanner,
    SubmenuIndexScene4,
    SubmenuIndexScene5,
    SubmenuIndexSettings,
};

void meal_pager_scene_menu_submenu_callback(void* context, uint32_t index) {
    Meal_Pager* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void meal_pager_scene_menu_on_enter(void* context) {
    Meal_Pager* app = context;

    meal_pager_set_max_values(app);

    submenu_add_item(
        app->submenu,
        "Send Data",
        SubmenuIndexTransmit,
        meal_pager_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Set First Station",
        SubmenuIndexSetFirstStation,
        meal_pager_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Set Last Station",
        SubmenuIndexSetLastStation,
        meal_pager_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Set First Pager",
        SubmenuIndexSetFirstPager,
        meal_pager_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Set Last Pager",
        SubmenuIndexSetLastPager,
        meal_pager_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Station Scanner",
        SubmenuIndexScanner,
        meal_pager_scene_menu_submenu_callback,
        app);
    submenu_add_item(
        app->submenu,
        "Settings",
        SubmenuIndexSettings,
        meal_pager_scene_menu_submenu_callback,
        app);

    submenu_set_selected_item(
        app->submenu, scene_manager_get_scene_state(app->scene_manager, Meal_PagerSceneMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, Meal_PagerViewIdMenu);
}

bool meal_pager_scene_menu_on_event(void* context, SceneManagerEvent event) {
    Meal_Pager* app = context;
    UNUSED(app);
    if(event.type == SceneManagerEventTypeBack) {
        //exit app
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubmenuIndexTransmit) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneMenu, SubmenuIndexTransmit);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneTransmit);
            return true;
        } else if(event.event == SubmenuIndexSettings) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneMenu, SubmenuIndexSettings);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneSettings);
            return true;
        } else if(event.event == Meal_PagerCustomEventViewTransmitterSendStop) {
            app->state_notifications = SubGhzNotificationStateIDLE;
            subghz_txrx_stop(app->subghz->txrx);
            FURI_LOG_D(TAG, "Stop Event from Menu");
            return true;
        } else if(event.event == SubmenuIndexSetFirstStation) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneSetFirstStation, SubmenuIndexSetFirstStation);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneSetFirstStation);
            return true;
        } else if(event.event == SubmenuIndexSetLastStation) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneSetLastStation, SubmenuIndexSetLastStation);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneSetLastStation);
            return true;
        } else if(event.event == SubmenuIndexSetFirstPager) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneSetFirstPager, SubmenuIndexSetFirstPager);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneSetFirstPager);
            return true;
        } else if(event.event == SubmenuIndexSetLastPager) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneSetLastPager, SubmenuIndexSetLastPager);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneSetLastPager);
            return true;
        } else if(event.event == SubmenuIndexScanner) {
            scene_manager_set_scene_state(
                app->scene_manager, Meal_PagerSceneScanner, SubmenuIndexScanner);
            scene_manager_next_scene(app->scene_manager, Meal_PagerSceneScanner);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->state_notifications == SubGhzNotificationStateTx) {
            app->state_notifications = SubGhzNotificationStateIDLE;
            subghz_txrx_stop(app->subghz->txrx);
            meal_pager_blink_stop(app);
            meal_pager_transmit_model_set_sending(app->meal_pager_transmit, 0);
        }
        return true;
    }
    return false;
}

void meal_pager_scene_menu_on_exit(void* context) {
    Meal_Pager* app = context;
    submenu_reset(app->submenu);
}