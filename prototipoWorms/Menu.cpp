#include "Menu.h"
#include "Application.h"
#include "utilities.h"

Menu::Menu(const sf::Font& f,int charSize,int linePad,int scrollBarWidth) :
rndrTxtre(),spr(),selectionBg(),scrollBar(),view(),font(f),menuList(),active(true),characterSize(charSize),
linePadding(linePad),marker(0),itemsVisible(0),scrolled(0),scroll(false),clicked(-1),bgColor(sf::Color::Transparent)
{
    selectionBg.setFillColor(sf::Color::Red);
    scrollBar.setFillColor(sf::Color::White);
    scrollBar.setSize(sf::Vector2f(scrollBarWidth,0));
    if(linePad == -1)//at least i don't use negative line paddings , so this ok for me,
    {
        linePadding = font.getLineSpacing(characterSize)-characterSize;
    }
}
void Menu::draw(sf::RenderTarget &target)
{
    target.draw(spr);
    if(scroll)
        target.draw(scrollBar);
}
void Menu::clear()
{
    menuList.clear();
}
void Menu::add(const std::string& item,sf::Color textColor)
{
    sfTextPtr newText(new sf::Text(item,font,characterSize));
    newText->setColor(textColor);
    newText->setPosition(0,menuList.size()*(linePadding+characterSize));
    menuList.push_back(std::move(newText));
}
void Menu::create(float width,float height)
{
    if(height == 0)
        height = menuList.size()*(linePadding+characterSize);
    else
        height = std::min(menuList.size()*float(linePadding+characterSize),round(height,linePadding+characterSize));

    itemsVisible = (height/(linePadding+characterSize));
    //max width of the sf::Texts of the menu items
    float maxwidth = (**std::max_element(menuList.begin(),menuList.end(),
                            [](sfTextPtr& a,sfTextPtr &b){ return a->getLocalBounds().width<b->getLocalBounds().width;})).getLocalBounds().width;
    if(size_t(itemsVisible) < menuList.size())
    {
        scroll = true;
    }
    if(width == 0)  width = maxwidth;
    else
        width = std::max(width,maxwidth);
    rndrTxtre.clear(bgColor);
    rndrTxtre.create(width,height);
    view = rndrTxtre.getDefaultView();
    selectionBg.setSize(sf::Vector2f(width,linePadding+characterSize));
    if(scroll)
    {
        scrollBar.setSize(sf::Vector2f( scrollBar.getSize().x,std::max(5.0f,height/(menuList.size()+1-itemsVisible))));
        scrollBar.setPosition(spr.getPosition().x+width,spr.getPosition().y+0);
    }

    rndrTxtre.clear(bgColor);
    rndrTxtre.draw(selectionBg);
    for(const auto &a : menuList)
    {
        rndrTxtre.draw(*a);
    }
    rndrTxtre.display();
    spr.setTexture(rndrTxtre.getTexture(),true);
}
void Menu::step(float dt)
{
    if(!active) return;
    int deltaScroll = 0;
    bool update = false;

    sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(Application::getWindow()));
    sf::Vector2f relMouse = mouse - spr.getPosition();
    sf::Vector2f mouseInView = rndrTxtre.mapPixelToCoords(static_cast<sf::Vector2i>(relMouse));
    if(scroll && sf::Mouse::isButtonPressed(sf::Mouse::Left) && contains(scrollBar.getPosition().x,spr.getPosition().y,scrollBar.getSize().x,view.getSize().y,mouse.x,mouse.y))
    {
        int new_scrolled = (menuList.size()-itemsVisible+1)*relMouse.y/(view.getSize().y);
        if(new_scrolled != scrolled)
        {
            deltaScroll = new_scrolled-scrolled;
            update = true;
        }
    }
    if(update)  updateTexture(deltaScroll);
}
void Menu::passEvent(const sf::Event &event)
{
    if(!active) return;
    bool update = false;
    int deltaScroll = 0;
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
        case sf::Keyboard::Up:
            if(--marker < 0)
            {
                marker = menuList.size()-1;
                deltaScroll = menuList.size()-itemsVisible;
            }
            else if(marker < scrolled)
                deltaScroll = -1;
            update = true;
            break;
        case sf::Keyboard::Down:
            if(size_t(++marker) > menuList.size()-1)
            {
                deltaScroll = -menuList.size()+itemsVisible;
                marker = 0;
            }
            else if(marker-scrolled+1>itemsVisible)
                deltaScroll = 1;
            update = true;
            break;
        case sf::Keyboard::Return:
            clicked = marker;
            break;
        default:
            break;
        }
    }
    else if(event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mouse(event.mouseMove.x,event.mouseMove.y);
        sf::Vector2f mouseInView = rndrTxtre.mapPixelToCoords(sf::Vector2i(mouse) - sf::Vector2i(spr.getPosition()));
        if(spr.getGlobalBounds().contains(mouse) && mouseInView.y/(linePadding+characterSize) != marker && mouseInView.y/(linePadding+characterSize)-scrolled <= menuList.size())
        {
            marker = mouseInView.y/(linePadding+characterSize);
            update = true;
        }
    }
    else if(event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mouse(event.mouseButton.x,event.mouseButton.y);
        sf::Vector2f mouseInView = rndrTxtre.mapPixelToCoords(sf::Vector2i(mouse) - sf::Vector2i(spr.getPosition()));
        if(spr.getGlobalBounds().contains(mouse) && mouseInView.y/(linePadding+characterSize) != marker && mouseInView.y/(linePadding+characterSize)-scrolled <= menuList.size())
        {
            clicked = marker = mouseInView.y/(linePadding+characterSize);
            update = true;
        }
    }
    else if(event.type == sf::Event::MouseWheelMoved)
    {
        if(-event.mouseWheel.delta+scrolled+itemsVisible <= int(menuList.size()) && -event.mouseWheel.delta+scrolled >= 0)
        {
            sf::Vector2f mouse(event.mouseWheel.x,event.mouseWheel.y);
            if(spr.getGlobalBounds().contains(mouse))
            {
                deltaScroll = -event.mouseWheel.delta;
                update = true;
                mouse.y += deltaScroll*(linePadding+characterSize);//adjust for the new scroll
                mouse -= spr.getPosition();
                mouse = rndrTxtre.mapPixelToCoords(static_cast<sf::Vector2i>(mouse));
                if(mouse.y/(linePadding+characterSize) != marker/* && mouse.y/(linePadding+characterSize)-scrolled <= itemsVisible*/)
                    marker = mouse.y/(linePadding+characterSize);
            }
        }
    }

    if(update)  updateTexture(deltaScroll);
}
void Menu::updateTexture(int deltaScroll)
{
    selectionBg.setPosition(0,marker*(linePadding+characterSize));
    if(deltaScroll)
    {
        scrolled += deltaScroll;
        view.move(0,deltaScroll*(linePadding+characterSize));
        scrollBar.setPosition(scrollBar.getPosition().x,spr.getPosition().y+(view.getSize().y-scrollBar.getSize().y)*scrolled/(menuList.size()-itemsVisible));
    }
    rndrTxtre.setView(view);
    rndrTxtre.clear(bgColor);
    rndrTxtre.draw(selectionBg);
    for(const auto &a : menuList)
    {
        rndrTxtre.draw(*a);
    }
    rndrTxtre.display();
}
