#include <string>
#include <cctype>
#include <vector>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "sqlite3.h"
#include "resource.h"
#include "main.h"
using namespace std;

int main(){
    vector<slide> slides = {
        {vector<uint8_t>{0},"",false,""},
        {resources::image1,
"The purpose of this program is to guide you through a real life legal case. During the course of the case, you will be prompted to answer various questions about the situation. These questions will gauge and ultimately deepen your understanding of the material. Press \"Enter\" to advance the text in this program.",
        false,""},
        {resources::image1,
"Your name is Sally. You are an associate editor for Paladin Press, a publisher known for pushing boundaryies with \"edgy\" content. Your job is to look over manuscripts and determine whether or not they are fit for publishing, and you are up for a promotion.",
        false,""},
        {resources::image1,
"You turn to the papers on your desk. \"Another day of boring manuscripts\" you think to yourself. If you want to get that promotion, you'll need to bring something good to your boss's office this afternoon.",
        false,""},
        {resources::image2,
"You make your way through the stack and something catches your eye. \"Hit Man: A Technical Manual for Independent Contractors\". This is exactly the type of material that Paladin specializes in, and it could secure that promotion! Then again, is it really alright to publish this kind of material?",
        false,""},
        {resources::image2,
"Which individuals or parties can be considered stakeholders in this case? List any stakeholder you can think of.",
        true,""},
        {resources::image2,"",false,""},
        {resources::image2,
"That concludes this demo. Thank you for your time.",
        false,""}
    };

    vector<slide>::iterator cur = ++ slides.begin();
    vector<slide>::iterator last = -- slides.end();

    char DB[] = "Resources/hitman.db";
    sqlite3 *hitmanDB;
    if (sqlite3_open(DB,&hitmanDB)){
        slides.begin()->txt = "ERROR: Can't open database: " + (string)sqlite3_errmsg(hitmanDB);
        cur = slides.begin();
    }

    sf::RenderWindow window(sf::VideoMode({1280,720}), "Hitman");
    window.setFramerateLimit(60);

    tgui::Gui gui(window);

    sf::Image img;
    if(!img.loadFromMemory(cur->img.data(),cur->img.size())){
        slides.begin()->txt = "ERROR: Image could not be loaded";
        cur = slides.begin();
    }
    sf::Texture bg;
    if(!bg.loadFromImage(img)){
        slides.begin()->txt = "ERROR: Image could not be loaded";
        cur = slides.begin();
    }
    sf::Sprite background(bg);

    tgui::Font arial(resources::arial.data(),resources::arial.size());

    tgui::TextArea::Ptr box = tgui::TextArea::create();
    box->setReadOnly(true);
    box->setFocusable(true);
    box->setSize({1180,100});
    box->getRenderer()->setFont(arial);
    box->getRenderer()->setTextSize(15);
    box->getRenderer()->setBackgroundColor(tgui::Color::Black);
    box->getRenderer()->setTextColor(tgui::Color::White);
    box->getRenderer()->setBorderColor(tgui::Color::White);
    box->getRenderer()->setBorders(1);
    box->getRenderer()->setPadding(10);

    gui.add(box);

    tgui::TextArea::Ptr field = tgui::TextArea::create();
    field->setReadOnly(false);
    field->setFocusable(true);
    field->setSize({1180,100});
    field->setFocusable(true);
    field->getRenderer()->setFont(arial);
    field->getRenderer()->setTextSize(15);
    field->getRenderer()->setBackgroundColor(tgui::Color::White);
    field->getRenderer()->setTextColor(tgui::Color::Black);
    field->getRenderer()->setDefaultTextColor(tgui::Color(165,165,165));
    field->getRenderer()->setBorderColor(tgui::Color(165,165,165));
    field->getRenderer()->setBorders(1);
    field->setDefaultText("Enter response here...");
    field->getRenderer()->setPadding(10);

    gui.add(field);

    tgui::Button::Ptr redo = tgui::Button::create();
    redo->setSize({150,40});
    redo->getRenderer()->setFont(arial);
    redo->getRenderer()->setTextSize(15);
    redo->getRenderer()->setBackgroundColor(tgui::Color::Black);
    redo->getRenderer()->setTextColor(tgui::Color::White);
    redo->getRenderer()->setBorderColor(tgui::Color::White);
    redo->getRenderer()->setBackgroundColorHover(tgui::Color::White);
    redo->getRenderer()->setTextColorHover(tgui::Color::Black);
    redo->getRenderer()->setBorderColorHover(tgui::Color::Black);
    redo->getRenderer()->setBorders(1);
    redo->setText("Go Back");
    redo->onPress([&cur](){cur --;});

    gui.add(redo);

    string input = "";
    vector<string> stakeholders;

    while(window.isOpen()){

        while(optional event = window.pollEvent()){

            gui.handleEvent(*event);

            if(event->is<sf::Event::Closed>()){
                window.close();
            } else if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
                if(key->code == sf::Keyboard::Key::Enter && next(cur) != slides.end() && !(field->isFocused())){
                    if(cur->input){
                        input = input + " " + field->getText().toStdString();
                        field->setText("");
                        stakeholders = stakeholder_check(hitmanDB,input);
                        next(cur)->txt = "You have identified the following stakeholders:\n" + printList(stakeholders) + "\n\nClick \"Go Back\" if you would like to add more. Otherwise, continue.";
                    } else if(prev(cur)->input) input = "";
                    cur ++;
                }
            } else if(const auto* button = event->getIf<sf::Event::MouseButtonPressed>()){
                if(cur->input && button->button == sf::Mouse::Button::Left){}
            }
        }

        if(!img.loadFromMemory(cur->img.data(),cur->img.size())){
            slides.begin()->txt = "ERROR: Image could not be loaded";
            cur = slides.begin();
        }
        if(!bg.loadFromImage(img)){
            slides.begin()->txt = "ERROR: Image could not be loaded";
            cur = slides.begin();
        }
        background.setTexture(bg);

        box->setText(cur->txt);

        if(cur->input){
            box->setPosition({50,470});
            field->setPosition({50,570});
        } else{
            box->setPosition({50,570});
            field->setPosition({50,770});
        }

        if(cur != slides.begin() && prev(cur)->input) redo->setPosition({1080,630});
        else redo->setPosition({50,770});

        window.clear();
        window.draw(background);
        gui.draw();
        window.display();
    }

    sqlite3_close(hitmanDB);

    return 0;
}