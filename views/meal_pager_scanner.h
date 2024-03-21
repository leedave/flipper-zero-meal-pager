#pragma once

#include <gui/view.h>
#include "../helpers/meal_pager_custom_event.h"

typedef struct Meal_PagerScanner Meal_PagerScanner;

typedef void (*Meal_PagerScannerCallback)(Meal_PagerCustomEvent event, void* context);

void meal_pager_scanner_set_callback(
    Meal_PagerScanner* meal_pager_scanner,
    Meal_PagerScannerCallback callback,
    void* context);

View* meal_pager_scanner_get_view(Meal_PagerScanner* meal_pager_static);

Meal_PagerScanner* meal_pager_scanner_alloc();

void meal_pager_scanner_free(Meal_PagerScanner* meal_pager_static);