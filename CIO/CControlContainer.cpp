#include "CControlContainer.h"
#include "../CSurface.h"
#include "../globals.h"
#include "CButton.h"
#include "CFont.h"
#include "CPicture.h"
#include "CSelectBox.h"
#include "CTextfield.h"
#include "helpers/containerUtils.h"

CControlContainer::CControlContainer(int pic_background, Point<uint16_t> borderSize)
    : borderSize(borderSize), pic_background(pic_background)
{}

CControlContainer::~CControlContainer() = default;

void CControlContainer::setBackgroundPicture(int pic_background)
{
    this->pic_background = pic_background;
    needRender = true;
}

void CControlContainer::setMouseData(const SDL_MouseMotionEvent motion)
{
    for(const auto& picture : pictures)
    {
        picture->setMouseData(motion);
    }
    for(const auto& button : buttons)
    {
        button->setMouseData(motion);
    }
    for(const auto& selectbox : selectboxes)
    {
        selectbox->setMouseData(motion);
    }
    needRender = true;
}

void CControlContainer::setMouseData(const SDL_MouseButtonEvent button)
{
    for(const auto& picture : pictures)
    {
        picture->setMouseData(button);
    }
    for(const auto& i : buttons)
    {
        i->setMouseData(button);
    }
    for(const auto& textfield : textfields)
    {
        textfield->setMouseData(button);
    }
    for(const auto& selectbox : selectboxes)
    {
        selectbox->setMouseData(button);
    }
    needRender = true;
}

void CControlContainer::setKeyboardData(const SDL_KeyboardEvent& key)
{
    for(const auto& textfield : textfields)
    {
        textfield->setKeyboardData(key);
    }
}

template<class T, class U>
bool CControlContainer::eraseElement(T& collection, const U* element)
{
    const auto it = helpers::find_if(collection, [element](const auto& cur) { return cur.get() == element; });
    if(it != collection.end())
    {
        collection.erase(it);
        needRender = true;
        return true;
    }
    return false;
}

CButton* CControlContainer::addButton(void callback(int), int clickedParam, Uint16 x, Uint16 y, Uint16 w, Uint16 h, int color,
                                      const char* text, int picture)
{
    x += borderSize.x;
    y += borderSize.y;

    buttons.emplace_back(std::make_unique<CButton>(callback, clickedParam, x, y, w, h, color, text, picture));
    needRender = true;
    return buttons.back().get();
}

bool CControlContainer::delButton(CButton* ButtonToDelete)
{
    return eraseElement(buttons, ButtonToDelete);
}

CFont* CControlContainer::addText(std::string string, int x, int y, int fontsize, int color)
{
    x += borderSize.x;
    y += borderSize.y;

    texts.emplace_back(std::make_unique<CFont>(std::move(string), x, y, fontsize, color));
    needRender = true;
    return texts.back().get();
}

bool CControlContainer::delText(CFont* TextToDelete)
{
    return eraseElement(texts, TextToDelete);
}

CPicture* CControlContainer::addPicture(void callback(int), int clickedParam, Uint16 x, Uint16 y, int picture)
{
    x += borderSize.x;
    y += borderSize.y;

    pictures.emplace_back(std::make_unique<CPicture>(callback, clickedParam, x, y, picture));
    needRender = true;
    return pictures.back().get();
}

bool CControlContainer::delPicture(CPicture* PictureToDelete)
{
    return eraseElement(pictures, PictureToDelete);
}

int CControlContainer::addStaticPicture(int x, int y, int picture)
{
    if(picture < 0)
        return -1;
    x += borderSize.x;
    y += borderSize.y;

    unsigned id = static_pictures.empty() ? 0u : static_pictures.back().id + 1u;
    static_pictures.emplace_back(Picture{x, y, picture, id});
    needRender = true;
    return id;
}

bool CControlContainer::delStaticPicture(int picId)
{
    if(picId < 0)
        return false;
    const auto it = helpers::find_if(static_pictures, [picId](const auto& pic) { return static_cast<unsigned>(picId) == pic.id; });
    if(it != static_pictures.end())
    {
        static_pictures.erase(it);
        needRender = true;
        return true;
    }
    return false;
}

CTextfield* CControlContainer::addTextfield(Uint16 x, Uint16 y, Uint16 cols, Uint16 rows, int fontsize, int text_color, int bg_color,
                                            bool button_style)
{
    x += borderSize.x;
    y += borderSize.y;

    textfields.emplace_back(std::make_unique<CTextfield>(x, y, cols, rows, fontsize, text_color, bg_color, button_style));
    needRender = true;
    return textfields.back().get();
}

bool CControlContainer::delTextfield(CTextfield* TextfieldToDelete)
{
    return eraseElement(textfields, TextfieldToDelete);
}

CSelectBox* CControlContainer::addSelectBox(Uint16 x, Uint16 y, Uint16 w, Uint16 h, int fontsize, int text_color, int bg_color)
{
    x += borderSize.x;
    y += borderSize.y;

    selectboxes.emplace_back(std::make_unique<CSelectBox>(x, y, w, h, fontsize, text_color, bg_color));
    needRender = true;
    return selectboxes.back().get();
}

bool CControlContainer::delSelectBox(CSelectBox* SelectBoxToDelete)
{
    return eraseElement(selectboxes, SelectBoxToDelete);
}

void CControlContainer::renderElements()
{
    for(const auto& static_picture : static_pictures)
    {
        CSurface::Draw(surface, global::bmpArray[static_picture.pic].surface, static_picture.x, static_picture.y);
    }
    for(const auto& picture : pictures)
    {
        CSurface::Draw(surface, picture->getSurface(), picture->getX(), picture->getY());
    }
    for(const auto& text : texts)
    {
        CSurface::Draw(surface, text->getSurface(), text->getX(), text->getY());
    }
    for(const auto& textfield : textfields)
    {
        CSurface::Draw(surface, textfield->getSurface(), textfield->getX(), textfield->getY());
    }
    for(const auto& selectbox : selectboxes)
    {
        CSurface::Draw(surface, selectbox->getSurface(), selectbox->getX(), selectbox->getY());
    }
    for(const auto& button : buttons)
    {
        CSurface::Draw(surface, button->getSurface(), button->getX(), button->getY());
    }
}
