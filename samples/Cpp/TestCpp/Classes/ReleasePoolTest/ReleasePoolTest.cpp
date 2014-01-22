#include "ReleasePoolTest.h"

using namespace cocos2d;

class TestObject : public Object
{
public:
    TestObject() : _name(""){}
    
    TestObject(std::string name) : _name(name)
    {
        CCLOG("TestObject:%s is created", _name.c_str());
    }
    
    ~TestObject()
    {
        if (_name.size() > 0)
            CCLOG("TestObject:%s is destroyed", _name.c_str());
    }
    
private:
    std::string _name;
};

void ReleasePoolTestScene::runThisTest()
{
    // title
    auto label = LabelTTF::create("AutoreasePool Test", "Arial", 32);
    addChild(label, 9999);
    label->setPosition(Point(VisibleRect::center().x, VisibleRect::top().y - 30));
    
    // reference count should be added when added into auto release pool
    
    TestObject *obj = new TestObject("testobj");
    obj->autorelease();
    assert(obj->retainCount() == 1);
    
    // should retain first before invoking autorelease
    obj->retain();
    obj->autorelease();
    assert(obj->retainCount() == 2);
    
    // create an autorelease pool in stack
    
    {
        AutoreleasePool pool1;
        
        // can invoke autorelease more than once
        obj->retain();
        obj->autorelease();
        assert(obj->retainCount() == 3);
        obj->retain();
        obj->autorelease();
        assert(obj->retainCount() == 4);
        
        // retain, release can work together with autorelease pool
        obj->retain();
        assert(obj->retainCount() == 5);
        obj->release();
        assert(obj->retainCount() == 4);
    }
    
    assert(obj->retainCount() == 2);
    
    // example of using temple autorelease pool
    {
        AutoreleasePool pool2;
        char name[20];
        for (int i = 0; i < 100; ++i)
        {
            snprintf(name, 20, "object%d", i);
            TestObject *tmpObj = new TestObject(name);
            tmpObj->autorelease();
        }
    }
    
    // object in pool2 should be released
    
    Director::getInstance()->replaceScene(this);
}