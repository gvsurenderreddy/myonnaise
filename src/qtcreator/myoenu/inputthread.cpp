#include "inputthread.h"
#include <QTimer>
#include <Windows.h>

InputThread::InputThread(QObject *parent) :
    QThread(parent)
{
    m_keyMap = std::vector<bool>(256);
    m_keyMod = std::set<int>();
}


void
InputThread::run()
{
    //We use a timer to decide how often to poll for key state changes
    //We'll update our key states every 10ms
    QTimer fetchTimer;
    connect(&fetchTimer, SIGNAL(timeout()), this, SLOT(onKeysUpdate()), Qt::DirectConnection);
    fetchTimer.setInterval(10);
    fetchTimer.start();
    exec();
    fetchTimer.stop();
}


void
InputThread::onKeysUpdate()
{
    for (int key=0; key<255; key++) {
        bool state = (GetAsyncKeyState(key) & 1<<16);
        bool shouldSendSignal = false;

        if (m_keyMap[key] == false && state == true)
        {
            //Key press down event
            m_keyMap[key] = true;
            shouldSendSignal = true;
        } else if (m_keyMap[key] == true && state == false)
        {
            //Key press up event
            m_keyMap[key] = false;
            shouldSendSignal = true;
        }

        //Make sure all of the modifier keys are being pressed
        for (auto it = m_keyMod.cbegin(); it != m_keyMod.cend(); it++)
        {
            if (m_keyMap.at(*it) == false)
            {
                shouldSendSignal = false;
            }
        }

        //Send the key pressed signal
        if (shouldSendSignal) {
            emit sigOnKeyPressed(key, state);
        }
    }
}


void
InputThread::addModifierKey(int keyCode)
{
    m_keyMod.insert(keyCode);
}
