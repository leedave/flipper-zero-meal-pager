#include "meal_pager_scanner.h"

#include "../meal_pager_i.h"
#include <furi.h>
#include <furi_hal.h>
#include <input/input.h>
#include <gui/elements.h>

struct Meal_PagerScanner {
    View* view;
    Meal_PagerScannerCallback callback;
    void* context;
};

typedef struct {
    int some_value;
} Meal_PagerScannerModel;

void meal_pager_scanner_set_callback(
    Meal_PagerScanner* instance,
    Meal_PagerScannerCallback callback,
    void* context) {
    furi_assert(instance);
    furi_assert(callback);
    instance->callback = callback;
    instance->context = context;
}

void meal_pager_scanner_draw(Canvas* canvas, Meal_PagerScannerModel* model) {
    UNUSED(model);
    char buffer[64];
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignTop, "Restaurant Pager");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 22, AlignCenter, AlignTop, "Trigger Tool");
    snprintf(buffer, sizeof(buffer), "Version: %s", MEAL_PAGER_VERSION);
    canvas_draw_str_aligned(canvas, 64, 42, AlignCenter, AlignTop, buffer);
}

static void meal_pager_scanner_model_init(Meal_PagerScannerModel* const model) {
    model->some_value = 1;
}

bool meal_pager_scanner_input(InputEvent* event, void* context) {
    furi_assert(context);
    Meal_PagerScanner* instance = context;
    if(event->type == InputTypeRelease) {
        switch(event->key) {
        case InputKeyBack:
            with_view_model(
                instance->view,
                Meal_PagerScannerModel * model,
                {
                    UNUSED(model);
                    instance->callback(Meal_PagerCustomEventScannerBack, instance->context);
                },
                true);
            break;
        case InputKeyOk:
            with_view_model(
                instance->view,
                Meal_PagerScannerModel * model,
                {
                    UNUSED(model);
                    instance->callback(Meal_PagerCustomEventScannerOk, instance->context);
                },
                true);
            break;
        default:
            break;
        }
    }
    return true;
}

void meal_pager_scanner_exit(void* context) {
    furi_assert(context);
}

void meal_pager_scanner_enter(void* context) {
    furi_assert(context);
    Meal_PagerScanner* instance = (Meal_PagerScanner*)context;
    with_view_model(
        instance->view,
        Meal_PagerScannerModel * model,
        { meal_pager_scanner_model_init(model); },
        true);
}

Meal_PagerScanner* meal_pager_scanner_alloc() {
    Meal_PagerScanner* instance = malloc(sizeof(Meal_PagerScanner));
    instance->view = view_alloc();
    view_allocate_model(instance->view, ViewModelTypeLocking, sizeof(Meal_PagerScannerModel));
    view_set_context(instance->view, instance); // furi_assert crashes in events without this
    view_set_draw_callback(instance->view, (ViewDrawCallback)meal_pager_scanner_draw);
    view_set_input_callback(instance->view, meal_pager_scanner_input);
    //view_set_enter_callback(instance->view, meal_pager_scanner_enter);
    //view_set_exit_callback(instance->view, meal_pager_scanner_exit);

    with_view_model(
        instance->view,
        Meal_PagerScannerModel * model,
        { meal_pager_scanner_model_init(model); },
        true);

    return instance;
}

void meal_pager_scanner_free(Meal_PagerScanner* instance) {
    furi_assert(instance);

    with_view_model(
        instance->view, Meal_PagerScannerModel * model, { UNUSED(model); }, true);
    view_free(instance->view);
    free(instance);
}

View* meal_pager_scanner_get_view(Meal_PagerScanner* instance) {
    furi_assert(instance);
    return instance->view;
}
