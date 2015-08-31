//
// Copyright (c) 2009, Wei Mingzhi <whistler_wmz@users.sf.net>.
// Portions Copyright (c) 2009, netwan.
//
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "main.h"
#include <fdlibm.h>
#include "touchpad.h"

PALINPUTSTATE            g_InputState;
int TPRelease = 0;
int TPLastRead = 0;
int TPPressed = 0;

#if defined(GPH)
#define MIN_DEADZONE -16384
#define MAX_DEADZONE 16384
#endif

static VOID
PAL_KeyboardEventFilter(
   const SDL_Event       *lpEvent
)
/*++
  Purpose:

    Handle keyboard events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
	int tpzone;
   switch (lpEvent->type)
   {
   case SDL_KEYDOWN:
      //
      // Pressed a key
      //
      if (lpEvent->key.keysym.mod & KMOD_CTRL)
      {
         if (lpEvent->key.keysym.sym == SDLK_ESCAPE)
         {
            //
            // Pressed Alt+F4 (Exit program)...
            //
            PAL_Shutdown();
            exit(0);
         }
      }

      switch (lpEvent->key.keysym.sym)
      {
#ifdef __SYMBIAN32__
      //
      // Symbian-specific stuff
      //
      case SDLK_0:
         VIDEO_ToggleScaleScreen();
         break;
      case SDLK_1:
         SOUND_AdjustVolume(0);
         break;
      case SDLK_3:
         SOUND_AdjustVolume(1);
         break;
#endif

      //case SDLK_UP:
      case SDLK_8:
      case SDLK_9:
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirNorth;
         g_InputState.dwKeyPress |= kKeyUp;
         break;

      //case SDLK_DOWN:
      case SDLK_2:
      case SDLK_1:
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirSouth;
         g_InputState.dwKeyPress |= kKeyDown;
         break;

      //case SDLK_LEFT:
      case SDLK_4:
      case SDLK_7:
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirWest;
         g_InputState.dwKeyPress |= kKeyLeft;
         break;

      //case SDLK_RIGHT:
      case SDLK_6:
      case SDLK_3:
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirEast;
         g_InputState.dwKeyPress |= kKeyRight;
         break;

#if defined(DINGOO)
      case SDLK_SPACE:
		 g_InputState.dwKeyPress = kKeyMenu;
         break;

      case SDLK_LCTRL:
		 g_InputState.dwKeyPress = kKeySearch;
         break;
#else
	  case SDLK_HOME:
		 if (PAL_ConfirmMenu())
         {
           PAL_FadeOut(2);
           PAL_Shutdown();
           exit(0);
         }
      case SDLK_ESCAPE:
      case SDLK_MENU:
         g_InputState.dwKeyPress |= kKeyMenu;
         break;

      case SDLK_RETURN:
      case SDLK_SPACE:
      case SDLK_KP_ENTER:
      case SDLK_LCTRL:
         g_InputState.dwKeyPress |= kKeySearch;
         break;

      case SDLK_PAGEUP:
      case SDLK_KP_MULTIPLY:
         g_InputState.dwKeyPress |= kKeyPgUp;
         break;

      case SDLK_PAGEDOWN:
      case SDLK_KP_DIVIDE:
         g_InputState.dwKeyPress |= kKeyPgDn;
         break;

	  case SDLK_TAB: //for nspire
      case SDLK_r:
         g_InputState.dwKeyPress |= kKeyRepeat;
         break;

      case SDLK_a:
         g_InputState.dwKeyPress |= kKeyAuto;
         break;

      case SDLK_d:
         g_InputState.dwKeyPress |= kKeyDefend;
         break;

      case SDLK_e:
         g_InputState.dwKeyPress |= kKeyUseItem;
         break;

      case SDLK_w:
         g_InputState.dwKeyPress |= kKeyThrowItem;
         break;

      case SDLK_q:
         g_InputState.dwKeyPress |= kKeyFlee;
         break;

      case SDLK_s:
         g_InputState.dwKeyPress |= kKeyStatus;
         break;

      case SDLK_f:
      case SDLK_5: // 5 for mobile device
         g_InputState.dwKeyPress |= kKeyForce;
         break;

      case SDLK_HASH: //# for mobile device
      case SDLK_p:
         VIDEO_SaveScreenshot();
         break;
#endif

      default:
         break;
      }
      break;

   case SDL_KEYUP:
      //
      // Released a key
      //
      switch (lpEvent->key.keysym.sym)
      {
      //case SDLK_UP:
      case SDLK_8:
      case SDLK_9:
         if (g_InputState.dir == kDirNorth)
         {
            g_InputState.dir = g_InputState.prevdir;
         }
         g_InputState.prevdir = kDirUnknown;
         break;

      //case SDLK_DOWN:
      case SDLK_2:
      case SDLK_1:
         if (g_InputState.dir == kDirSouth)
         {
            g_InputState.dir = g_InputState.prevdir;
         }
         g_InputState.prevdir = kDirUnknown;
         break;

      //case SDLK_LEFT:
      case SDLK_4:
      case SDLK_7:
         if (g_InputState.dir == kDirWest)
         {
            g_InputState.dir = g_InputState.prevdir;
         }
         g_InputState.prevdir = kDirUnknown;
         break;

      //case SDLK_RIGHT:
      case SDLK_6:
      case SDLK_3:
         if (g_InputState.dir == kDirEast)
         {
            g_InputState.dir = g_InputState.prevdir;
         }
         g_InputState.prevdir = kDirUnknown;
         break;

      default:
         break;
      }
      break;
   }
   //TP
	if ((!isTPTouched())&&(TPRelease))
	{
		TPRelease = 0;
		TPLastRead = 0;
		TPPressed = 0;
		g_InputState.dir = kDirUnknown;
   }else if(isTPTouched())
   {
	   TPRelease = 1;
	   tpzone = getTouchedZone9();
	   if ((tpzone!=TPLastRead)||((!TPPressed)&&(isTPPressed()))){
			switch (tpzone)
			{
			case 8:
			case 9:
				g_InputState.dir = kDirNorth;
				g_InputState.dwKeyPress |= kKeyUp;
				break;

			case 2:
			case 1:
				g_InputState.dir = kDirSouth;
				g_InputState.dwKeyPress |= kKeyDown;
				break;

			case 4:
			case 7:
				g_InputState.dir = kDirWest;
				g_InputState.dwKeyPress |= kKeyLeft;
				break;

			case 6:
			case 3:
				g_InputState.dir = kDirEast;
				g_InputState.dwKeyPress |= kKeyRight;
				break;
			
			case 5:
				g_InputState.dwKeyPress |= kKeySearch;
				break;
			}
			TPLastRead = tpzone;
		}
		TPPressed = isTPPressed();
   }
	   
}

static VOID
PAL_MouseEventFilter(
   const SDL_Event *lpEvent
)
/*++
  Purpose:

    Handle mouse events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
#ifdef PAL_HAS_MOUSE
   static short hitTest = 0; // Double click detect;	
   const SDL_VideoInfo *vi;

   double       screenWidth, gridWidth;
   double       screenHeight, gridHeight;
   double       mx, my;
   double       thumbx;
   double       thumby;
   INT          gridIndex;
   BOOL			isLeftMouseDBClick = FALSE;
   BOOL			isLeftMouseClick = FALSE;
   BOOL			isRightMouseClick = FALSE;
   static INT   lastReleaseButtonTime, lastPressButtonTime, betweenTime;
   static INT   lastPressx = 0;
   static INT   lastPressy = 0;
   static INT   lastReleasex = 0;
   static INT   lastReleasey = 0;

   if (lpEvent->type!= SDL_MOUSEBUTTONDOWN && lpEvent->type != SDL_MOUSEBUTTONUP)
      return;

   vi = SDL_GetVideoInfo();
   screenWidth = vi->current_w;
   screenHeight = vi->current_h;
   gridWidth = screenWidth / 3;
   gridHeight = screenHeight / 3;
   mx = lpEvent->button.x;
   my = lpEvent->button.y;
   thumbx = ceil(mx / gridWidth);
   thumby = floor(my / gridHeight);
   gridIndex = thumbx + thumby * 3 - 1;
   
   switch (lpEvent->type)
   {
   case SDL_MOUSEBUTTONDOWN:
      lastPressButtonTime = SDL_GetTicks();
      lastPressx = lpEvent->button.x;
      lastPressy = lpEvent->button.y;
      switch (gridIndex)
      {
      case 2:
         g_InputState.prevdir = g_InputState.dir;
         g_InputState.dir = kDirNorth;
         break;
      case 6:
         g_InputState.prevdir = g_InputState.dir;
         g_InputState.dir = kDirSouth;
         break;
      case 0:
         g_InputState.prevdir = g_InputState.dir;
         g_InputState.dir = kDirWest;
         break;
      case 8:
         g_InputState.prevdir = g_InputState.dir;
         g_InputState.dir = kDirEast;
         break;
      case 1:
    	 //g_InputState.prevdir = g_InputState.dir;
    	 //g_InputState.dir = kDirNorth;
         g_InputState.dwKeyPress |= kKeyUp;
         break;
      case 7:
    	 //g_InputState.prevdir = g_InputState.dir;
    	 //g_InputState.dir = kDirSouth; 
         g_InputState.dwKeyPress |= kKeyDown;
         break;
      case 3:
    	 //g_InputState.prevdir = g_InputState.dir;
    	 //g_InputState.dir = kDirWest;
    	 g_InputState.dwKeyPress |= kKeyLeft;
         break;
      case 5:
         //g_InputState.prevdir = g_InputState.dir;
         //g_InputState.dir = kDirEast;
         g_InputState.dwKeyPress |= kKeyRight;
         break;
      }
      break;
   case SDL_MOUSEBUTTONUP:
      lastReleaseButtonTime = SDL_GetTicks();
      lastReleasex = lpEvent->button.x;
      lastReleasey = lpEvent->button.y;
      hitTest ++;
      if (abs(lastPressx - lastReleasex) < 25 &&
                     abs(lastPressy - lastReleasey) < 25)
      {
		  betweenTime = lastReleaseButtonTime - lastPressButtonTime;
		  if (betweenTime >500)
		  {
			  isRightMouseClick = TRUE;
		  }
		  else if (betweenTime >=0)
		  {
			  if((betweenTime < 100) && (hitTest >= 2))
			  {
				  isLeftMouseClick = TRUE;
			  	  hitTest = 0;  
			  }
			  else
			  {  
				  isLeftMouseClick = TRUE;
				  if(betweenTime > 100)
				  {
					  hitTest = 0;
				  }
				  
			  }
		  }
      }
      switch (gridIndex)
      {
      case 2:
    	 if( isLeftMouseDBClick )
		 {
			 SOUND_AdjustVolume(1);
			 break;
		 }
      case 6:
      case 0:
    	 if( isLeftMouseDBClick )
		 {
			 SOUND_AdjustVolume(0);
			 break;
		 }
      case 7:
    	  if (isRightMouseClick) //repeat attack
    	  {
    	     g_InputState.dwKeyPress |= kKeyRepeat;
    	     break;
    	  }
      case 8:
         g_InputState.dir = kDirUnknown;
         g_InputState.prevdir = kDirUnknown;
         break;
      case 1:
    	 if( isRightMouseClick )
		 {
			 g_InputState.dwKeyPress |= kKeyForce;
		 }
    	 break;
      case 3:
    	 if( isRightMouseClick )
		 {
			 g_InputState.dwKeyPress |= kKeyAuto;
		 }
    	 break;
      case 5:
    	 if( isRightMouseClick )
		 {
			 g_InputState.dwKeyPress |= kKeyDefend;
		 }
		 break;
      case 4:
		if (isRightMouseClick) // menu
		{
		   g_InputState.dwKeyPress |= kKeyMenu;
		}
		else if (isLeftMouseClick) // search
		{
		   g_InputState.dwKeyPress |= kKeySearch;
		}
		
        break;
      }
      break;
   }
#endif
}

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION <= 2
static int SDLCALL
PAL_EventFilter(
   const SDL_Event       *lpEvent
)
#else
static int SDLCALL
PAL_EventFilter(
   void                  *userdata,
   const SDL_Event       *lpEvent
)
#endif
/*++
  Purpose:

    SDL event filter function. A filter to process all events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    1 = the event will be added to the internal queue.
    0 = the event will be dropped from the queue.

--*/
{
   switch (lpEvent->type)
   {
   case SDL_VIDEORESIZE:
      //
      // resized the window
      //
      VIDEO_Resize(lpEvent->resize.w, lpEvent->resize.h);
      break;

   case SDL_QUIT:
      //
      // clicked on the close button of the window. Quit immediately.
      //
      PAL_Shutdown();
      exit(0);
   }

   PAL_KeyboardEventFilter(lpEvent);
   PAL_MouseEventFilter(lpEvent);
   //PAL_JoystickEventFilter(lpEvent);

   //
   // All events are handled here; don't put anything to the internal queue
   //
   return 0;
}

VOID
PAL_ClearKeyState(
   VOID
)
/*++
  Purpose:

    Clear the record of pressed keys.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   g_InputState.dwKeyPress = 0;
}

VOID
PAL_InitInput(
   VOID
)
/*++
  Purpose:

    Initialize the input subsystem.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   memset(&g_InputState, 0, sizeof(g_InputState));
   g_InputState.dir = kDirUnknown;
   g_InputState.prevdir = kDirUnknown;
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION <= 2
   SDL_SetEventFilter(PAL_EventFilter);
#else
   SDL_SetEventFilter(PAL_EventFilter, NULL);
#endif
	initTP();
	readTP();
   //
   // Check for joystick
   //
}

VOID
PAL_ShutdownInput(
   VOID
)
/*++
  Purpose:

    Shutdown the input subsystem.

  Parameters:

    None.

  Return value:

    None.

--*/
{
	
}

VOID
PAL_ProcessEvent(
   VOID
)
/*++
  Purpose:

    Process all events.

  Parameters:

    None.

  Return value:

    None.

--*/
{
#ifdef PAL_HAS_NATIVEMIDI
   MIDI_CheckLoop();
#endif
	readTP();
	if ((isTPTouched())||(TPRelease)) PAL_KeyboardEventFilter(NULL);
   while (SDL_PollEvent(NULL));
}
