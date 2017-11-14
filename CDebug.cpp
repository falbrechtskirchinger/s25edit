#include "CDebug.h"
#include "CGame.h"
#include "CIO/CWindow.h"
#include "CMap.h"
#include "globals.h"

#ifdef _ADMINMODE

CDebug::CDebug(void dbgCallback(int), int quitParam)
{
    dbgWnd = new CWindow(dbgCallback, quitParam, 0, 0, 540, 130, "Debugger", WINDOW_GREEN1,
                         WINDOW_CLOSE | WINDOW_MOVE | WINDOW_MINIMIZE | WINDOW_RESIZE);
    global::s2->RegisterWindow(dbgWnd);
    dbgWnd->addText("Debugger started", 0, 0, fontsize);
    this->dbgCallback_ = dbgCallback;
    FrameCounterText = NULL;
    FramesPerSecText = NULL;
    msWaitText = NULL;
    RegisteredMenusText = NULL;
    RegisteredWindowsText = NULL;
    RegisteredCallbacksText = NULL;
    MouseText = NULL;
    MapNameText = NULL;
    MapSizeText = NULL;
    MapAuthorText = NULL;
    MapTypeText = NULL;
    MapPlayerText = NULL;
    VertexText = NULL;
    VertexDataText = NULL;
    VertexVectorText = NULL;
    FlatVectorText = NULL;
    rsuTextureText = NULL;
    usdTextureText = NULL;
    roadText = NULL;
    objectTypeText = NULL;
    objectInfoText = NULL;
    animalText = NULL;
    unknown1Text = NULL;
    buildText = NULL;
    unknown2Text = NULL;
    unknown3Text = NULL;
    resourceText = NULL;
    shadingText = NULL;
    unknown5Text = NULL;
    editorModeText = NULL;
    fontsize = 9;
    MapObj = global::s2->MapObj;
    map = NULL;
    global::s2->RegisterCallback(dbgCallback);

    // add buttons for in-/decrementing msWait
    dbgWnd->addButton(dbgCallback, DECREMENT_MSWAIT, 75, 30, 15, 15, BUTTON_GREY, "-");
    dbgWnd->addButton(dbgCallback, SETZERO_MSWAIT, 90, 30, 15, 15, BUTTON_GREY, "0");
    dbgWnd->addButton(dbgCallback, INCREMENT_MSWAIT, 105, 30, 15, 15, BUTTON_GREY, "+");

    // we draw a vertical line to separate map data on the right side from things on the left side
    dbgWnd->addText("#", 240, 10, fontsize);
    dbgWnd->addText("#", 240, 20, fontsize);
    dbgWnd->addText("#", 240, 30, fontsize);
    dbgWnd->addText("#", 240, 40, fontsize);
    dbgWnd->addText("#", 240, 50, fontsize);
    dbgWnd->addText("#", 240, 60, fontsize);
    dbgWnd->addText("#", 240, 70, fontsize);
    dbgWnd->addText("#", 240, 80, fontsize);
    dbgWnd->addText("#", 240, 90, fontsize);
    dbgWnd->addText("#", 240, 100, fontsize);
    dbgWnd->addText("#", 240, 110, fontsize);
    dbgWnd->addText("#", 240, 120, fontsize);
    dbgWnd->addText("#", 240, 130, fontsize);
    dbgWnd->addText("#", 240, 140, fontsize);
    dbgWnd->addText("#", 240, 150, fontsize);
    dbgWnd->addText("#", 240, 160, fontsize);
    dbgWnd->addText("#", 240, 170, fontsize);
    dbgWnd->addText("#", 240, 180, fontsize);
    dbgWnd->addText("#", 240, 190, fontsize);
    dbgWnd->addText("#", 240, 200, fontsize);
    dbgWnd->addText("#", 240, 210, fontsize);
    dbgWnd->addText("#", 240, 220, fontsize);
}

CDebug::~CDebug()
{
    global::s2->UnregisterCallback(dbgCallback_);
    dbgWnd->setWaste();
}

void CDebug::sendParam(int Param)
{
    switch(Param)
    {
        case CALL_FROM_GAMELOOP: actualizeData(); break;

        case INCREMENT_MSWAIT: global::s2->msWait++; break;

        case DECREMENT_MSWAIT:
            if(global::s2->msWait > 0)
                global::s2->msWait--;
            break;

        case SETZERO_MSWAIT: global::s2->msWait = 0; break;

        default: break;
    }
}

void CDebug::actualizeData()
{
    // del FrameCounterText before drawing new
    if(FrameCounterText != NULL)
        dbgWnd->delText(FrameCounterText);
    // write new FrameCounterText and draw it
    sprintf(puffer1, "Actual Frame:    %lu", global::s2->FrameCounter);
    FrameCounterText = dbgWnd->addText(puffer1, 0, 10, fontsize);

    // Frames per Second
    static Uint32 tmpFrameCtr = 0, tmpTickCtr = SDL_GetTicks();
    if(tmpFrameCtr == 10)
    {
        // del FramesPerSecText before drawing new
        if(FramesPerSecText != NULL)
            dbgWnd->delText(FramesPerSecText);
        // write new FramesPerSecText and draw it
        sprintf(puffer1, "Frames per Sec: %.2f", tmpFrameCtr / (((float)SDL_GetTicks() - tmpTickCtr) / 1000));
        FramesPerSecText = dbgWnd->addText(puffer1, 0, 20, fontsize);
        // set new values
        tmpFrameCtr = 0;
        tmpTickCtr = SDL_GetTicks();
    } else
        tmpFrameCtr++;

    // del msWaitText before drawing new
    if(msWaitText != NULL)
        dbgWnd->delText(msWaitText);
    // write new msWaitText and draw it
    sprintf(puffer1, "Wait: %ums", global::s2->msWait);
    msWaitText = dbgWnd->addText(puffer1, 0, 35, fontsize);

    // del MouseText before drawing new
    if(MouseText != NULL)
        dbgWnd->delText(MouseText);
    // write new MouseText and draw it
    sprintf(puffer1, "Mouse: x=%d y=%d %s", global::s2->Cursor.x, global::s2->Cursor.y,
            (global::s2->Cursor.clicked ?
               (global::s2->Cursor.button.left ? "LMB clicked" : (global::s2->Cursor.button.right ? "RMB clicked" : "clicked")) :
               "unclicked"));
    MouseText = dbgWnd->addText(puffer1, 0, 50, fontsize);

    // del RegisteredMenusText before drawing new
    if(RegisteredMenusText != NULL)
        dbgWnd->delText(RegisteredMenusText);
    // write new RegisteredMenusText and draw it
    sprintf(puffer1, "Registered Menus: %d (max. %d)", global::s2->RegisteredMenus, MAXMENUS);
    RegisteredMenusText = dbgWnd->addText(puffer1, 0, 60, fontsize);

    // del RegisteredWindowsText before drawing new
    if(RegisteredWindowsText != NULL)
        dbgWnd->delText(RegisteredWindowsText);
    // write new RegisteredWindowsText and draw it
    sprintf(puffer1, "Registered Windows: %d (max. %d)", global::s2->RegisteredWindows, MAXWINDOWS);
    RegisteredWindowsText = dbgWnd->addText(puffer1, 0, 70, fontsize);

    // del RegisteredCallbacksText before drawing new
    if(RegisteredCallbacksText != NULL)
        dbgWnd->delText(RegisteredCallbacksText);
    // write new RegisteredCallbacksText and draw it
    sprintf(puffer1, "Registered Callbacks: %d (max. %d)", global::s2->RegisteredCallbacks, MAXCALLBACKS);
    RegisteredCallbacksText = dbgWnd->addText(puffer1, 0, 80, fontsize);

    // we will now write the map data if a map is active
    MapObj = global::s2->MapObj;
    if(MapObj != NULL)
    {
        map = MapObj->map;
        const MapNode& vertex = map->getVertex(MapObj->VertexX_, MapObj->VertexY_);

        if(MapNameText != NULL)
        {
            if(dbgWnd->delText(MapNameText))
                MapNameText = NULL;
        }
        if(MapNameText == NULL)
        {
            sprintf(puffer1, "Map Name: %s", map->name);
            MapNameText = dbgWnd->addText(puffer1, 260, 10, fontsize);
        }
        if(MapSizeText != NULL)
        {
            if(dbgWnd->delText(MapSizeText))
                MapSizeText = NULL;
        }
        if(MapSizeText == NULL)
        {
            sprintf(puffer1, "Width: %d  Height: %d", map->width, map->height);
            MapSizeText = dbgWnd->addText(puffer1, 260, 20, fontsize);
        }
        if(MapAuthorText != NULL)
        {
            if(dbgWnd->delText(MapAuthorText))
                MapAuthorText = NULL;
        }
        if(MapAuthorText == NULL)
        {
            sprintf(puffer1, "Author: %s", map->author);
            MapAuthorText = dbgWnd->addText(puffer1, 260, 30, fontsize);
        }
        if(MapTypeText != NULL)
        {
            if(dbgWnd->delText(MapTypeText))
                MapTypeText = NULL;
        }
        if(MapTypeText == NULL)
        {
            sprintf(puffer1, "Type: %d (%s)", map->type,
                    (map->type == MAP_GREENLAND ?
                       "Greenland" :
                       (map->type == MAP_WASTELAND ? "Wasteland" : (map->type == MAP_WINTERLAND ? "Winterland" : "Unknown"))));
            MapTypeText = dbgWnd->addText(puffer1, 260, 40, fontsize);
        }
        if(MapPlayerText != NULL)
        {
            if(dbgWnd->delText(MapPlayerText))
                MapPlayerText = NULL;
        }
        if(MapPlayerText == NULL)
        {
            sprintf(puffer1, "Player: %d", map->player);
            MapPlayerText = dbgWnd->addText(puffer1, 260, 50, fontsize);
        }
        if(VertexText != NULL)
        {
            if(dbgWnd->delText(VertexText))
                VertexText = NULL;
        }
        if(VertexText == NULL)
        {
            sprintf(puffer1, "Vertex: %d, %d", MapObj->VertexX_, MapObj->VertexY_);
            VertexText = dbgWnd->addText(puffer1, 260, 60, fontsize);
        }
        if(VertexDataText != NULL)
        {
            if(dbgWnd->delText(VertexDataText))
                VertexDataText = NULL;
        }
        if(VertexDataText == NULL)
        {
            sprintf(puffer1, "Vertex Data: x=%d, y=%d, z=%d i=%.2f h=%#04x", vertex.x, vertex.y, vertex.z, ((float)vertex.i) / pow(2, 16),
                    vertex.h);
            VertexDataText = dbgWnd->addText(puffer1, 260, 70, fontsize);
        }
        if(VertexVectorText != NULL)
        {
            if(dbgWnd->delText(VertexVectorText))
                VertexVectorText = NULL;
        }
        if(VertexVectorText == NULL)
        {
            sprintf(puffer1, "Vertex Vector: (%.2f, %.2f, %.2f)", vertex.normVector.x, vertex.normVector.y, vertex.normVector.z);
            VertexVectorText = dbgWnd->addText(puffer1, 260, 80, fontsize);
        }
        if(FlatVectorText != NULL)
        {
            if(dbgWnd->delText(FlatVectorText))
                FlatVectorText = NULL;
        }
        if(FlatVectorText == NULL)
        {
            sprintf(puffer1, "Flat Vector: (%.2f, %.2f, %.2f)", vertex.flatVector.x, vertex.flatVector.y, vertex.flatVector.z);
            FlatVectorText = dbgWnd->addText(puffer1, 260, 90, fontsize);
        }
        if(rsuTextureText != NULL)
        {
            if(dbgWnd->delText(rsuTextureText))
                rsuTextureText = NULL;
        }
        if(rsuTextureText == NULL)
        {
            sprintf(puffer1, "RSU-Texture: %#04x", vertex.rsuTexture);
            rsuTextureText = dbgWnd->addText(puffer1, 260, 100, fontsize);
        }
        if(usdTextureText != NULL)
        {
            if(dbgWnd->delText(usdTextureText))
                usdTextureText = NULL;
        }
        if(usdTextureText == NULL)
        {
            sprintf(puffer1, "USD-Texture: %#04x", vertex.usdTexture);
            usdTextureText = dbgWnd->addText(puffer1, 260, 110, fontsize);
        }
        if(roadText != NULL)
        {
            if(dbgWnd->delText(roadText))
                roadText = NULL;
        }
        if(roadText == NULL)
        {
            sprintf(puffer1, "road: %#04x", vertex.road);
            roadText = dbgWnd->addText(puffer1, 260, 120, fontsize);
        }
        if(objectTypeText != NULL)
        {
            if(dbgWnd->delText(objectTypeText))
                objectTypeText = NULL;
        }
        if(objectTypeText == NULL)
        {
            sprintf(puffer1, "objectType: %#04x", vertex.objectType);
            objectTypeText = dbgWnd->addText(puffer1, 260, 130, fontsize);
        }
        if(objectInfoText != NULL)
        {
            if(dbgWnd->delText(objectInfoText))
                objectInfoText = NULL;
        }
        if(objectInfoText == NULL)
        {
            sprintf(puffer1, "objectInfo: %#04x", vertex.objectInfo);
            objectInfoText = dbgWnd->addText(puffer1, 260, 140, fontsize);
        }
        if(animalText != NULL)
        {
            if(dbgWnd->delText(animalText))
                animalText = NULL;
        }
        if(animalText == NULL)
        {
            sprintf(puffer1, "animal: %#04x", vertex.animal);
            animalText = dbgWnd->addText(puffer1, 260, 150, fontsize);
        }
        if(unknown1Text != NULL)
        {
            if(dbgWnd->delText(unknown1Text))
                unknown1Text = NULL;
        }
        if(unknown1Text == NULL)
        {
            sprintf(puffer1, "unknown1: %#04x", vertex.unknown1);
            unknown1Text = dbgWnd->addText(puffer1, 260, 160, fontsize);
        }
        if(buildText != NULL)
        {
            if(dbgWnd->delText(buildText))
                buildText = NULL;
        }
        if(buildText == NULL)
        {
            sprintf(puffer1, "build: %#04x", vertex.build);
            buildText = dbgWnd->addText(puffer1, 260, 170, fontsize);
        }
        if(unknown2Text != NULL)
        {
            if(dbgWnd->delText(unknown2Text))
                unknown2Text = NULL;
        }
        if(unknown2Text == NULL)
        {
            sprintf(puffer1, "unknown2: %#04x", vertex.unknown2);
            unknown2Text = dbgWnd->addText(puffer1, 260, 180, fontsize);
        }
        if(unknown3Text != NULL)
        {
            if(dbgWnd->delText(unknown3Text))
                unknown3Text = NULL;
        }
        if(unknown3Text == NULL)
        {
            sprintf(puffer1, "unknown3: %#04x", vertex.unknown3);
            unknown3Text = dbgWnd->addText(puffer1, 260, 190, fontsize);
        }
        if(resourceText != NULL)
        {
            if(dbgWnd->delText(resourceText))
                resourceText = NULL;
        }
        if(resourceText == NULL)
        {
            sprintf(puffer1, "resource: %#04x", vertex.resource);
            resourceText = dbgWnd->addText(puffer1, 260, 200, fontsize);
        }
        if(shadingText != NULL)
        {
            if(dbgWnd->delText(shadingText))
                shadingText = NULL;
        }
        if(shadingText == NULL)
        {
            sprintf(puffer1, "shading: %#04x", vertex.shading);
            shadingText = dbgWnd->addText(puffer1, 260, 210, fontsize);
        }
        if(unknown5Text != NULL)
        {
            if(dbgWnd->delText(unknown5Text))
                unknown5Text = NULL;
        }
        if(unknown5Text == NULL)
        {
            sprintf(puffer1, "unknown5: %#04x", vertex.unknown5);
            unknown5Text = dbgWnd->addText(puffer1, 260, 220, fontsize);
        }
        if(editorModeText != NULL)
        {
            if(dbgWnd->delText(editorModeText))
                editorModeText = NULL;
        }
        if(editorModeText == NULL)
        {
            sprintf(puffer1, "Editor --> Mode: %d Content: %#04x Content2: %#04x", MapObj->mode, MapObj->modeContent, MapObj->modeContent2);
            editorModeText = dbgWnd->addText(puffer1, 260, 230, fontsize);
        }
    } else
    {
        // del MapNameText before drawing new
        if(MapNameText != NULL)
        {
            if(dbgWnd->delText(MapNameText))
                MapNameText = NULL;
        }
        if(MapNameText == NULL)
        {
            // write new MapNameText and draw it
            sprintf(puffer1, "No Map loaded!");
            MapNameText = dbgWnd->addText(puffer1, 260, 10, fontsize);
        }
        if(MapSizeText != NULL)
        {
            if(dbgWnd->delText(MapSizeText))
                MapSizeText = NULL;
        }
        if(MapAuthorText != NULL)
        {
            if(dbgWnd->delText(MapAuthorText))
                MapAuthorText = NULL;
        }
        if(MapTypeText != NULL)
        {
            if(dbgWnd->delText(MapTypeText))
                MapTypeText = NULL;
        }
        if(MapPlayerText != NULL)
        {
            if(dbgWnd->delText(MapPlayerText))
                MapPlayerText = NULL;
        }
        if(VertexText != NULL)
        {
            if(dbgWnd->delText(VertexText))
                VertexText = NULL;
        }
        if(VertexDataText != NULL)
        {
            if(dbgWnd->delText(VertexDataText))
                VertexDataText = NULL;
        }
        if(VertexVectorText != NULL)
        {
            if(dbgWnd->delText(VertexVectorText))
                VertexVectorText = NULL;
        }
        if(FlatVectorText != NULL)
        {
            if(dbgWnd->delText(FlatVectorText))
                FlatVectorText = NULL;
        }
        if(rsuTextureText != NULL)
        {
            if(dbgWnd->delText(rsuTextureText))
                rsuTextureText = NULL;
        }
        if(usdTextureText != NULL)
        {
            if(dbgWnd->delText(usdTextureText))
                usdTextureText = NULL;
        }
        if(roadText != NULL)
        {
            if(dbgWnd->delText(roadText))
                roadText = NULL;
        }
        if(objectTypeText != NULL)
        {
            if(dbgWnd->delText(objectTypeText))
                objectTypeText = NULL;
        }
        if(objectInfoText != NULL)
        {
            if(dbgWnd->delText(objectInfoText))
                objectInfoText = NULL;
        }
        if(animalText != NULL)
        {
            if(dbgWnd->delText(animalText))
                animalText = NULL;
        }
        if(unknown1Text != NULL)
        {
            if(dbgWnd->delText(unknown1Text))
                unknown1Text = NULL;
        }
        if(buildText != NULL)
        {
            if(dbgWnd->delText(buildText))
                buildText = NULL;
        }
        if(unknown2Text != NULL)
        {
            if(dbgWnd->delText(unknown2Text))
                unknown2Text = NULL;
        }
        if(unknown3Text != NULL)
        {
            if(dbgWnd->delText(unknown3Text))
                unknown3Text = NULL;
        }
        if(resourceText != NULL)
        {
            if(dbgWnd->delText(resourceText))
                resourceText = NULL;
        }
        if(shadingText != NULL)
        {
            if(dbgWnd->delText(shadingText))
                shadingText = NULL;
        }
        if(unknown5Text != NULL)
        {
            if(dbgWnd->delText(unknown5Text))
                unknown5Text = NULL;
        }
        if(editorModeText != NULL)
        {
            if(dbgWnd->delText(editorModeText))
                editorModeText = NULL;
        }
    }
}

#endif
