#include "oxygine-framework.h"
#include <functional>
using namespace oxygine;

Resources gameResources;

class MainActor: public Actor {
public:
    spTextField _text;
    spSprite _button;
    
    MainActor() {
        //creating simple sprite
        spSprite button = new Sprite();
        
        //setting it up
        button->setResAnim(gameResources.getResAnim(("button")));
        
        //Centering
        Vector2 pos = getStage()->getSize() / 2 - button->getSize() / 2;
        button->setPosition(pos);
        
        //button clicks
        EventCallback cb = CLOSURE(this, &MainActor::buttonClicked);
        button->addEventListener(TouchEvent::CLICK, cb);
        
        addChild(button);
        
        _button = button;
    }
    
    void buttonClicked(Event* event) {
        //button was clicked
        _button->setColor(Color::White);
        _button->addTween(Sprite::TweenColor(Color::Green), 500, 1, true);
    }
};

//Intrusive_ptr stuff
typedef oxygine::intrusive_ptr<MainActor> spMainActor;

void example_preinit(){}

void example_init() {
        gameResources.loadXML("xmls/res.xml");
        
        spMainActor actor = new MainActor;
        
        getStage()->addChild(actor);
    }

    void example_update() {
        
    }

    void example_destroy(){
        gameResources.free();
    }