#include "inputcontroller.h"
#include <future>
#include <QDebug>

InputController::InputController()
    : m_keepListening(false), m_keyMod1(0), m_keyMod2(0)
{
    m_funcMap = new std::map<int, Voidfunc >();
    m_keyMap = std::vector<int>(255);
}

InputController::~InputController()
{
    delete m_funcMap;
}

void
InputController::registerFunctionHotkey(int keyCode, void(*func)())
{
    // Associate the function with the key code
    m_funcMap->insert( std::pair<int, Voidfunc>( keyCode, func ) );
}

void
InputController::startListen()
{
    this->m_keepListening = true;

    std::future<void> test = std::async(std::launch::async, [](InputController *ref) {
        qDebug() << "Starting listening for key presses\n";

        while(ref->m_keepListening)
        {
            Sleep(11); // avoid 100% cpu usage

            for(int key=0; key<255; key++) {
                ref->updateKeyState(key, (GetAsyncKeyState(key) == -32767));
            }
        }
        qDebug() << "Done listening for key presses.\n";

    }, this);

}

void
InputController::stopListen()
{
    //Set our stop flag to false so the listen thread stops its listen loop
    m_keepListening = false;
}

void
InputController::updateKeyState(int key, bool state)
{
    if (key > 254) {
        qDebug() << "Recieved key with code " << key << "\n";
        return;
    }

    // A state of 1 means the key is being held down.
    // 0 means the key is not being pressed.
    // Upon the key state going from 0->1, we send a key pressed event.
    if (m_keyMap[key] == 0 && state == true)
    {
        m_keyMap[key] = 1;
        handleKeyPress(key);
    } else if (m_keyMap[key] == 1 && state == false)
    {
        m_keyMap[key] = 0;
    }
}

void
InputController::handleKeyPress(int key)
{
    qDebug() << "Key pressed: " << key << "\n";

    //Find the function associated with the key, if any, then execute it.
    //If key modifiers were specified, make sure these are being pressed.
    if ((m_keyMod1 < 1 || m_keyMap[m_keyMod1] == 1) && (m_keyMod2 < 1 || m_keyMap[m_keyMod2] == 1))
    {
        qDebug() << "Passed key mod check.\n" ;
        if (m_funcMap->find(key) != m_funcMap->end())
        {
            ((Voidfunc)(m_funcMap->find(key)->second))();
        }
    }
}

void
InputController::setKeyModifier1(int keyMod)
{
    m_keyMod1 = keyMod;
}

void
InputController::setKeyModifier2(int keyMod)
{
    m_keyMod2 = keyMod;
}
