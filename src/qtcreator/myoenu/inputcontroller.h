#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <Windows.h>
#include <QMainWindow>
#include <QKeyEvent>
#include <functional>
#include <thread>
#include <map>
#include <vector>

typedef void (*Voidfunc)();

class InputController
{
public:
    InputController();
    ~InputController();

    void registerFunctionHotkey( int keyCode, void(*)(void) );

    void startListen();

    void stopListen();

    void handleKeyPress(int key);

    void setKeyModifier1(int keyMod);
    void setKeyModifier2(int keyMod);

private:

    void updateKeyState(int key, bool state);

    int m_keyMod1;
    int m_keyMod2;
    bool m_keepListening;

    std::map<int, Voidfunc > *m_funcMap;
    std::vector<int> m_keyMap;
};

#endif // INPUTCONTROLLER_H
