#include "../../inc/MarlinConfig.h"

#if HAS_MEDIA

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../lcd/marlinui.h"

/**
 * M524: Abort the current SD print job (started with M24)
 */
void GcodeSuite::M524() {

  #if ANY(EXTENSIBLE_UI, DWIN_LCD_PROUI, TJC_DISPLAY)

    ui.abort_print(); // Same abort logic as below

  #else

    if (card.isStillPrinting())
      card.abortFilePrintSoon();
    else if (card.isMounted())
      card.closefile();

  #endif

}

#endif // HAS_MEDIA